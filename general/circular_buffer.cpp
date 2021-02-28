#ifndef CIRCULLAR_BUFFER_CPP
#define CIRCULLAR_BUFFER_CPP

#include "circular_buffer.hpp"
#include "byteswap.hpp"
#include <cstring>

circullar_buffer::circullar_buffer():
    memory_size( 0x10000 ), memory( new char[memory_size] ), writer(memory), reader(memory),
    edge( memory + memory_size - 256 ), behind_edge(nullptr) {} /* default 64KB */

circullar_buffer::~circullar_buffer() { delete[] memory; } /* frees memory */

circullar_buffer::circullar_buffer (int mem_size, char* memory_pointer, int edge_size ):
    memory_size(mem_size), memory(memory_pointer), writer(memory), reader(memory),
    edge(memory_pointer + mem_size - edge_size), behind_edge(nullptr) {} /* pointer from new[] #reowning */

void circullar_buffer::data_was_written_into_buffer(char* ptr, unsigned int amount)
{
    // advance_writer_pointer

    if( ptr != writer ) throw "unspecified writing was made";
    if( !is_empty() && writer == reader )
        throw "overflow (buffer overwrite, while full)";

    if( reader < writer )
    {
        if( writer + amount > memory + memory_size )
            throw "buffer overflow (branch: r<w)";
        if( writer + amount < edge ) writer += amount;
        if( writer + amount >= edge )
        {
            behind_edge = writer + amount;
            writer = memory;
        }
    }
    else if( reader > writer )
    {
        if( writer + amount < reader ) writer += amount;
        else if ( writer + amount == reader ) writer += amount; // circ buf is full
        else throw "buffer overflow (branch: r>w)";
    }
    else if( reader == writer )
    {
        if( amount < memory_size ) writer += amount;
        else if( amount == memory_size ) writer = memory;
        else if( amount > memory_size ) throw "buffer overflow (branch: r=w)";
        if( memory + amount > edge ) behind_edge = memory + amount, writer = memory;
    }
}

void circullar_buffer::data_was_readen_from_buffer(char* ptr, unsigned int amount){
    // advance_reader_pointer

    if( ptr != reader ) throw "unspecified reading was made";
    if( is_empty() && amount != 0 ) throw "underflow (on empty)";

    if( reader < writer )
    {
        if( reader + amount < writer ) reader += amount;
        else if( reader + amount == writer ) reset();
        else throw "underflow";
    }
    else
    {
        if( reader + amount < behind_edge ) reader += amount;
        else if( reader + amount == behind_edge ) reader = memory, behind_edge = nullptr;
        else if( reader + amount > behind_edge ) throw "outside of buffer was readen";
    }
}

unsigned int circullar_buffer::get_linear_writable_buffer_size(){
    if( writer < reader ) return reader - writer;
    if( writer > reader ) return memory_size - ( writer - memory );

    if( is_empty() ) return memory_size;
    else return 0; // circ buf is full
}

unsigned int circullar_buffer::get_linear_readable_buffer_size(){
    if( reader < writer ) return writer - reader;
    if( reader > writer ) return behind_edge - reader;

    if( is_empty() ) return 0;
    return behind_edge - reader;
}

unsigned int circullar_buffer::size(){
    if(reader < writer) return writer - reader;
    else if(reader > writer) return (behind_edge - reader) + (writer - memory);
    else if( is_empty() ) return 0;
    else return behind_edge - memory;
}

unsigned int circullar_buffer::get_total_writable_area_size()
{
    // size + free might be different than memory_size due "edge" mechanism
    if(reader < writer) return memory_size - (writer - reader);
    else if(reader > writer) return reader - writer;
    else if( is_empty() ) return memory_size;
    else return 0;
}

unsigned int circullar_buffer::putn( const char* str, unsigned int amount )
{
    char* buf = get_linear_writable_buffer_pointer();
    unsigned int buf_size = get_linear_writable_buffer_size();

    unsigned int count = 0;
    while( buf_size != 0 && ( amount - count != 0 ) )
    {
        if ( amount - count <= buf_size )
        {
            memcpy( buf, str, amount - count );
            data_was_written_into_buffer( buf, amount - count );
            return amount;
        }
        else if ( amount - count > buf_size )
        {
            memcpy( buf, str, buf_size );
            data_was_written_into_buffer( buf, buf_size );
            str += buf_size;
            count += buf_size;

            buf = get_linear_writable_buffer_pointer();
            buf_size = get_linear_writable_buffer_size();
        }
    }
    return count;
}

unsigned int circullar_buffer::getn( char* str, unsigned int amount )
{
    char* buf = get_linear_readable_buffer_pointer();
    unsigned int buf_size = get_linear_readable_buffer_size();

    unsigned int count = 0;
    while( buf_size != 0 && ( amount - count != 0 ) )
    {
        if ( amount - count <= buf_size )
        {
            memcpy( str, buf, amount - count );
            data_was_readen_from_buffer( buf, amount - count );
            return amount;
        }
        else if ( amount - count > buf_size )
        {
            memcpy( str, buf, buf_size );
            data_was_readen_from_buffer( buf, buf_size );
            str += buf_size;
            count += buf_size;

            buf = get_linear_readable_buffer_pointer();
            buf_size = get_linear_readable_buffer_size();
        }
    }
    return count;
}

circullar_buffer& circullar_buffer::putc( char ch ){
    if( writer != reader || is_empty() ) *writer++ = ch;
    else throw "overflow in putc";
    if( writer == memory + memory_size ) 
        writer = memory, behind_edge = memory + memory_size;
    return *this;
}

unsigned char circullar_buffer::getc()
{
    if( is_empty() ) throw "underflow in getc";
    char ret = *((char*)reader++);
    if( reader == behind_edge ) reader = memory, behind_edge = nullptr;
    if( reader == writer ) reset();
    return ret;
}

circullar_buffer& circullar_buffer::operator<< ( const char* str )
{
    putn( str, strlen(str) );
    return *this;
}

circullar_buffer& circullar_buffer::operator<< ( bool  boolean )
{
    putc( boolean );
    return *this;
}

circullar_buffer& circullar_buffer::operator>> ( bool& boolean )
{
	boolean = getc() ? true : false;
    return *this;
}

circullar_buffer& circullar_buffer::operator<< ( char ch )
{
    putc( ch );
    return *this;
}

circullar_buffer& circullar_buffer::operator>> ( char& ch )
{
    ch = getc();
    return *this;
}

#endif // CIRCULLAR_BUFFER_CPP
