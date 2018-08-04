#ifndef MC_VarInt
#define MC_VarInt

#include "circular_buffer.hpp"

union varint
{
    unsigned int integer;

    varint() : integer(-1) {};
	varint(int val) : integer(val) {};

    int len();
    int operator=( int val ) { return integer = val; };
    operator int() { return integer; }
};

union varlong
{
    unsigned long long integer;

    varlong() : integer(-1) {};
	varlong(long long val) : integer(val) {};

	int len();
    long long operator= ( long long val ) { return integer = val; };
    operator long long() { return integer; }
};

circullar_buffer& operator<< ( circullar_buffer& buffer, varint varlen );
circullar_buffer& operator>> ( circullar_buffer& buffer, varint& varlen );

circullar_buffer& operator<< ( circullar_buffer& buffer, varlong varlen );
circullar_buffer& operator>> ( circullar_buffer& buffer, varlong& varlen );

// int varint_proposal_lenght( unsigned int to_encode );
// int varlong_proposal_lenght( unsigned int to_encode );

#endif // MC_VarInt

