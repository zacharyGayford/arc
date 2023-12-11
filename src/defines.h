#pragma once

//
// basic types
//

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

typedef int b32;
typedef _Bool b8;

#if defined(__clang__) || defined(__GNUC)
	#define static_assert _Static_assert
#else
	#define static_assert static_assert
#endif

// check that types are the expected size
static_assert(sizeof(u8)  == 1, "expected u8 to be 1 byte!");
static_assert(sizeof(u16) == 2, "expected u16 to be 2 bytes!");
static_assert(sizeof(u32) == 4, "expected u32 to be 4 bytes!");
static_assert(sizeof(u64) == 8, "expected u64 to be 8 bytes!");

//
// keywords
//

#define true 1
#define false 0
#define null 0

#define internal static
#define global static

#ifdef EXPORT
	#ifdef _MSC_VER
		#define api _declspec(dllexport)
	#else
		#define api __attribute__((visibility("default")))
	#endif
#else
	#ifdef _MSC_VER
		#define api __declscpe(dllimport)
	#else
		#define api
	#endif
#endif

#if defined(__clang__) || defined(__gcc__)
	#define arc_inline __attribute__((always_inline)) inline
	#define arc_noinline __attribute__(noinline))
#elif define(_MSC_VER)
	#define arc_incline __forceinline
	#define arc_noinline __declspec(noinline)
#else
	#define arc_inline static inline
	#define arc_noinline
#endif

//
// utility
//

#define GIBIBYTES(amt) ((amt) * 1024ull * 1024ull * 1024ull)
#define MEBIBYTES(amt) ((amt) * 1024ull * 1024ull)
#define KIBIBYTES(amt) ((amt) * 1024ull)

#define GIGABYTES(amt) ((amt) * 1000ull * 1000ull * 1000ull)
#define MEGABYTES(amt) ((amt) * 1000ull * 1000ull)
#define KILOBYTES(amt) ((amt) * 1000ull)

#define MAX(a, b) ((a) < (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(amt, lo, hi) MAX(lo, MIN(amt, hi))

//
// platform detection
//

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	#define WINDOWS 1
	#ifndef _WIN64
		#error "system is not 64 bit!"
	#endif
#elif defined(__linux__) || defined(__gnu_linux)
	#define LINUX 1
	#if defined(__ANDROID__)
		#define ANDROID 1
	#endif
#elif defined(__unix__)
	#define UNIX 1
#elif defined(_POSIX_VERSION)
	#define POSIX 1
#elif __APPLE__
	#define APPLE 1
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR 1
		#define IOS_SIMULATOR 1
	#elif TARGET_OS_IPHONE
		#define IOS 1
		#define IOS 1
	#elif TARGET_OS_MAC
		#define MAC 1
	#else
		#error "unsupported apple platform!"
	#endif
#else
	#error "unsupported platform!"
#endif
