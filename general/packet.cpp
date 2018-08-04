/*#include <cstring>
#include "packet.hpp"
#include "varlen.hpp"

buffered_packet emit_packet(char *& buffer, packet & pckt)
{
    buffered_packet sendable_packet;
    sendable_packet.data = buffer;

    encode_varint_into( buffer, pckt.itsLenght
        + varint_proposal_lenght( pckt.itsID ) );
    encode_varint_into( buffer, pckt.itsID);

    std::memcpy( buffer, pckt.itsData, pckt.itsLenght );
    buffer += pckt.itsLenght;

    sendable_packet.size  = pckt.itsLenght;
    sendable_packet.size += buffer - sendable_packet.data;
    return sendable_packet;
}*/
