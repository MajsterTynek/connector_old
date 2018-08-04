#ifndef BYTESWAP_HPP
#define BYTESWAP_HPP

#ifdef __GNUC__
#define __BSWAP__
#define bswap32 __builtin_bswap32
#define bswap64 __builtin_bswap64
#endif // __GNUC__

#ifdef _MSC_VER
#define __BSWAP__
#include <intrin.h>
#define bswap16 _byteswap_ushort
#define bswap32 _byteswap_ulong
#define bswap64 _byteswap_uint64
#endif // _MSC_VER

#ifndef __BSWAP__
#error NOT IMPLEMENTED! No code written for your compiler!
#endif // __BSWAP__

// inline void byte_swap_on_variable(char & _int8) {}
// inline void byte_swap_on_variable(unsigned char & _int8) {}

#ifndef bswap16
#ifdef __GNUC__
static inline short bswap16(short val){ asm("xchg %%al, %%ah;" : "=a"(val) : "a"(val) ); return val; }
static inline unsigned short bswap16(unsigned short val){ asm("xchg %%al, %%ah;" : "=a"(val) : "a"(val) ); return val; }
#define bswap16 bswap16
#endif // __GNUC__
#endif // bswap16

#ifndef bswap16
#error bswap16 is not implemented for this compiler!
#endif // bswap16

static inline short byte_swap(short val) { return bswap16(val); }
static inline int byte_swap(int val) { return bswap32(val); }
static inline long long byte_swap(long long val) { return bswap64(val); }
static inline unsigned short byte_swap(unsigned short val) { return bswap16(val); }
static inline unsigned int byte_swap(unsigned int val) { return bswap32(val); }
static inline unsigned long long byte_swap(unsigned long long val) { return bswap64(val); }
static inline float byte_swap(float val) { *(int*)&val = bswap32( *(int*)&val); return val; }
static inline double byte_swap(double val) { *(long long*)&val = bswap64( *(long long*)&val); return val;}

#define byte_swap_on_variable(var) (var = byte_swap(var))

#ifdef __LITTLE_ENDIAN__
#define __ENDIANNESS__
#define hton(val) byte_swap( val )
#define ntoh(val) byte_swap( val )
#define hton_on_variable(var) byte_swap_on_variable( var )
#define ntoh_on_variable(var) byte_swap_on_variable( var )
#endif // __LITTLE_ENDIAN__

#ifdef __BIG_ENDIAN__
#define __ENDIANNESS__
#define hton(val) val
#define ntoh(val) val
#define hton_on_variable(var)
#define ntoh_on_variable(var)
#endif // __BIG_ENDIAN__

#ifndef __ENDIANNESS__
#error Define "__LITTLE_ENDIAN__" or "__BIG_ENDIAN__" in project properties or before every header!
#endif // __ENDIANNESS__

#endif // BYTESWAP_HPP
