#ifndef CIRCULAR_BUFFER_HPP
#define CIRCULAR_BUFFER_HPP

struct circullar_buffer
{
    unsigned int memory_size;
    char* memory;

    char* writer;
    char* reader;

    char* edge;
    char* behind_edge;

    // void(*callback)(void);

    circullar_buffer ();  // default 64KB
    circullar_buffer ( int memory_size, char* memory_pointer, int edge_size = 0x400 ); // pointer from new[] #reowning

    circullar_buffer ( const circullar_buffer & ) = delete;
    circullar_buffer & operator= ( const circullar_buffer & ) = delete;

    ~circullar_buffer (); // frees memory

    void reset() { writer = memory, reader = memory; }
    bool is_empty() { return reader == memory && writer == memory; }
    unsigned int get_total_writable_area_size (); // only as hint
    unsigned int size ();

    // using these requires notifying buffer about usage:
    char*        get_linear_writable_buffer_pointer () { return writer; }
    char*        get_linear_readable_buffer_pointer () { return reader; }
    unsigned int get_linear_writable_buffer_size    ();
    unsigned int get_linear_readable_buffer_size    ();

    // notification form user that buffer was used:
    void data_was_written_into_buffer (char* ptr, unsigned int amount);
    void data_was_readen_from_buffer  (char* ptr, unsigned int amount);

    // stream INs and OUTs:
    template <class Type> // primitive types only
    circullar_buffer& operator<< ( Type value );

    template <class Type> // primitive types only
    circullar_buffer& operator>> ( Type& value );

    unsigned int putn ( const char* str, unsigned int amount );
    unsigned int getn ( char* str, unsigned int amount );

//  circullar_buffer& operator<< ( char* str ) { puts(str); }
//  circullar_buffer& operator>> ( char* str ) { gets(str); }

    circullar_buffer& operator<< ( const char* ch );
    circullar_buffer& operator>> ( char* ch );

    circullar_buffer& operator<< ( char  ch );
    circullar_buffer& operator>> ( char& ch );

    circullar_buffer& operator<< ( bool  _bool );
    circullar_buffer& operator>> ( bool& _bool );
    circullar_buffer& putc (char ch );
    unsigned char getc ();


//  unsigned int get_total_free_space ();
//  // for optimization purpose:
//  char* get_2nd_linear_free_space_pointer();
//  int   get_2nd_linear_free_space_size();
//
//  roll_back_to_begining()
//  get_linear_buffer(); // for simplicity
//  void  flush(); // ?
//  "update/free" version with callback // ?
//  make_outsider() // ?
};

#include "byteswap.hpp"

template <class Type>
circullar_buffer& circullar_buffer::operator<<( Type value )
// primitive types only // limited by byte_swap()
{
    /*if( sizeof(Type) == 1 )
    {
        putc(value);
        return *this;
    }*/ // some compilers are too dumb to understand

    if( sizeof(Type) <= get_linear_writable_buffer_size() )
    {
        hton_on_variable(value);
        *(Type*)writer = value;
        data_was_written_into_buffer( writer, sizeof(Type) );
        return *this;
    }

    if( sizeof(Type) > get_linear_writable_buffer_size() )
    {
        hton_on_variable(value);
        unsigned int count = putn( (char*)&value, sizeof(Type) );
        if ( count < sizeof(Type) ) throw "overflow protection (buf<<var)";
        return *this;
    }
    return *this;
}

template <class Type>
circullar_buffer& circullar_buffer::operator>>( Type& value )
// primitive types only // limited by byte_swap()
{
    /*if( sizeof(Type) == 1 )
    {
        *(unsigned char*)&value = getc();
        return *this;
    }*/ // some compilers are too dumb to understand

    if( sizeof(Type) <= get_linear_readable_buffer_size() )
    {
        value = *(Type*)reader;
        ntoh_on_variable(value);

        data_was_readen_from_buffer( reader, sizeof(Type) );
        return *this;
    }

    if( sizeof(Type) > get_linear_readable_buffer_size() )
    {
        unsigned int count = getn( (char*)&value, sizeof(Type) );
        if ( count < sizeof(Type) ) throw "underflow protection (buf>>var)";
        ntoh_on_variable(value);
        return *this;
    }
    return *this;
}

#endif // CIRCULLAR_BUFFER_HPP
