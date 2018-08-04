#ifndef VARLEN_CPP
#define VARLEN_CPP

#include <iostream>
using std::cout;

#include "varlen.hpp"
#include "circular_buffer.hpp"

int varint::len()
{
    if( (unsigned int)      integer < 128 ) return 1;
    else if( (unsigned int) integer < 128*128 ) return 2;
    else if( (unsigned int) integer < 128*128*128 ) return 3;
    else if( (unsigned int) integer < 128*128*128*128 ) return 4;
    else return 5;
}
int varlong::len()
{
    int byte_count = 1;
    auto val = integer;
    while(val>>= 7) byte_count++;
    return byte_count;
}

circullar_buffer& operator<< ( circullar_buffer& buffer, varint varlen )
{
    auto integer = varlen.integer;

    if ( (unsigned int) integer < 128 )
    {
        buffer.putc( integer );
        return buffer;
    }

    do {
        char byte = integer & 0x7F;
        integer >>= 7;
        if ( integer != 0 ) byte |= 0x80;
        buffer.putc( byte );
    } while ( integer != 0 );

    return buffer;
}

circullar_buffer& operator>> ( circullar_buffer& buffer, varint& varlen )
{
    auto byte = buffer.getc();
    varlen.integer = byte & 0x7F;
    unsigned char shift = 1;
    while(byte & 0x80)
    {
        if ( shift + 1 > 5 ) throw "varint +5 bytes";
        byte = buffer.getc();
        varlen.integer |= ( byte & 0x7F ) << ( 7 * shift++ );
    }
    return buffer;
}

circullar_buffer& operator<< ( circullar_buffer& buffer, varlong varlen )
{
    auto integer = varlen.integer;

    if ( integer < 128 )
    {
        buffer.putc( (char)integer );
        return buffer;
    }

    do {
        char byte = integer & 0x7F;
        integer >>= 7;
        if( integer != 0 ) byte |= 0x80;
        buffer.putc( (char)byte );
    } while ( integer != 0 );

    return buffer;
}

circullar_buffer& operator>> ( circullar_buffer& buffer, varlong& varlen )
{
    auto byte = buffer.getc();
    varlen.integer = byte & 0x7F;
    unsigned char shift = 1;
    while(byte & 0x80)
    {
        if ( shift + 1 > 10 ) throw "varint +10 bytes";
        byte = buffer.getc();
        varlen.integer |= ( byte & 0x7F ) << ( 7 * shift++ );
    }
    return buffer;
}

#endif // VARLEN_CPP
