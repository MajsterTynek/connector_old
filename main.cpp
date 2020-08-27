// -----------------------------
#include <string>
#include <string_view>
#include <cstring>
#include <iostream>
#include <iomanip>
#include "asio.hpp"
// #include <cstdlib>
#include <thread>
#include <chrono>
#include <system_error>
// #include "usage.hpp"
#include "general\circular_buffer.hpp"
#include "general\varlen.hpp"
// -----------------------------

int main(int argc, char* argv[])
{

	try
	{
		using asio::ip::tcp;
		using asio::buffer;

		std::string host, port;
		bool single_send = true;
		switch ( argc )
        {
            case 2: host = argv[1], port  = "25565"; break;
            case 3: host = argv[1], port  = argv[2]; break;
            case 4: host = argv[1], port  = argv[2], single_send = false; break;
            default: std::cout << "Usage: Connector <host> [port [unpack]]"; return 1;
        }

		// buffers
		circullar_buffer send_buf;
		circullar_buffer recv_buf;

		int bytes_sent = 0;
		int bytes_readen = 0;

		// constructing packets
		// handshake
		varint handshake_len;
		varint handshake_ID = 0;
		varint protocol_ver = 340; // 340 = 1.12.2
		// should be -1 for noclient convention 
		// not every server supports that 
		// and causes connection hangs and closes

		varint hostname_len = strlen( host.data() ); // argv[1]
		char* hostname_ptr = host.data();
		unsigned short port_ = atoi( port.data() ); // argv[2]
		varint nxt_state = 0x01;

		handshake_len = 0;
		handshake_len = handshake_ID.len() + protocol_ver.len() +
			hostname_len.len() + hostname_len + sizeof(port_) + nxt_state.len();

		send_buf << handshake_len << handshake_ID << protocol_ver << hostname_len;
		send_buf.putn(hostname_ptr, hostname_len);
		send_buf << port_ << nxt_state;

		if ( single_send ) 
		{
			// request : len, ID, nodata
			send_buf << varint(1) << varint(0);
			// ping pong : len, ID, payload
			send_buf << varint(9) << varint(1) << (long long)0;
		}

		// connection
		asio::io_service io_service;
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(host, port);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::socket socket(io_service);
		asio::connect(socket, endpoint_iterator);
		std::error_code error;

		char* data = send_buf.get_linear_readable_buffer_pointer();
		int   size = send_buf.get_linear_readable_buffer_size();
		asio::write(socket, asio::buffer(data, size), error);
		send_buf.data_was_readen_from_buffer(data, size);
		bytes_sent += size;

		if ( !single_send ) 
		{
			asio::write(socket, asio::buffer("\x01\x00", 2), error);
			bytes_sent += 2; // request
			std::this_thread::sleep_for( std::chrono::milliseconds(200) );
			asio::write(socket, asio::buffer("\x09\x01\0\0\0\0\0\0\0\0", 10), error);
			bytes_sent += 10; // ping
		}

		int bytes_prev;

		// for (;;)
		// {
		    std::this_thread::sleep_for( std::chrono::milliseconds(800) );
			bytes_prev = bytes_readen;

			data = recv_buf.get_linear_writable_buffer_pointer();
			size = recv_buf.get_linear_writable_buffer_size();
			auto buf = asio::buffer(data, size);

			size = socket.read_some(buf, error);
			bytes_readen += size;
			recv_buf.data_was_written_into_buffer(data, size);

			if (error == asio::error::eof)
            {
                std::cerr << "EOF has been met!" << '\n';
                return 1;
            }
			else if (error) throw std::system_error(error);
		// }

		// packet parsing not done yet
		varint packet_len, packet_ID;
		recv_buf >> packet_len >> packet_ID;

		if (packet_ID != 0)
		{
			std::cerr << "Something different than Response packet recieved!\n";
			return 1;
		}

		char* JSON;
		varint json_len;
		recv_buf >> json_len;
		JSON = new char[json_len + 1];
		JSON[json_len] = '\0';
		recv_buf.getn(JSON, json_len);
		std::string_view JSON_(JSON, json_len);

		long long pong;
		bool ping_pong_OK = false;
		recv_buf >> packet_len >> packet_ID >> pong;
		if (pong == 0) ping_pong_OK = true;

		// displaying results
		std::cout << JSON_ << std::endl;
		std::cerr << '\n' // idk even why I use both types of newlines lol
		/*	<< "\tping pong is:\t" << (ping_pong_OK ? "OK!\n" : "broken!\n") */
			<< "\tbytes sent:\t" << bytes_sent << '\n'
			<< "\tbytes readen:\t" << bytes_readen << '\n';

		if ( !ping_pong_OK ) std::cerr << "\tping pong is:\tbroken!\n";

		int rem = recv_buf.size();
		if ( rem > 0 ) 
		{	// there should be nothing to read after pong packet
			std::cerr << "\tremaining data in recv_buf: " << rem << ' ' << "bytes\n\n";
			
			char* remaining = new char[ rem ];
			recv_buf.getn( remaining, rem );

			std::cerr << std::hex << std::setfill('0');
			for (int i = 0; i < rem; ++i )
			{
				std::cerr <<  std::setw( 2 ) << (int)(remaining[i]);
				if ( i%16 == 15 ) std::cerr << std::endl;
			}
			std::cerr << std::endl;
			delete[] remaining;
		}
		delete[] JSON;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const char* str) // TO-DO: rework exceptions
	{
	    std::cerr << str;
	}

	return 0;
}
