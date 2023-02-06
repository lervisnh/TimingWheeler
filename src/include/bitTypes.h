#pragma once

// https://stackoverflow.com/questions/1505582/determining-32-vs-64-bit-in-c

#if _WIN32 || _WIN64 // Check windows
#if _WIN64
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

#if __GNUC__ // Check GCC
#if __x86_64__ || __ppc64__
#define ENV64BIT
#else
#define ENV32BIT
#endif
#endif

namespace _bitTypes {

typedef unsigned char u_char;
typedef unsigned short int u_short;
typedef unsigned int u_int;
typedef unsigned long int u_long;

typedef signed char int8_t;
typedef u_char uint8_t;
typedef signed short int int16_t;
typedef u_short uint16_t;
typedef signed int int32_t;
typedef u_int uint32_t;

#ifdef ENV64BIT
typedef signed long int int64_t;
typedef u_long uint64_t;
#endif

}; // namespace _bitTypes

namespace _timer {

using time_t = _bitTypes::uint64_t;
using index_t = _bitTypes::uint64_t;
using counter_t = _bitTypes::int64_t;

}; // namespace _timer
