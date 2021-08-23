// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Platform.h
* @author       Clement Berthaud
* @brief        This file provides platform definitions for the ULIS2 library.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <cstring>
#include <iostream>
#include <memory>
#include <cstdint>

/////////////////////////////////////////////////////
// SIMD PATCH FOR GNU < 9
#ifdef __GNUC__
#if __GNUC__ < 9
// unaligned load and store functions
#define _mm_loadu_si16(p) _mm_cvtsi32_si128(*(unsigned short const*)(p))
#define _mm_storeu_si16(p, a) (void)(*(short*)(p) = (short)_mm_cvtsi128_si32((a)))
#define _mm_loadu_si32(p) _mm_cvtsi32_si128(*(unsigned int const*)(p))
#define _mm_storeu_si32(p, a) (void)(*(int*)(p) = _mm_cvtsi128_si32((a)))
#define _mm_loadu_si64(p) _mm_loadl_epi64((__m128i const*)(p))
#define _mm_storeu_si64(p, a) (_mm_storel_epi64((__m128i*)(p), (a)))
#endif
#endif

#include <immintrin.h>

/////////////////////////////////////////////////////
// Detect Build Configuration
#ifdef NDEBUG
    #define ULIS2_RELEASE
#else // !NDEBUG
    #define ULIS2_DEBUG
#endif // !NDEBUG

/////////////////////////////////////////////////////
// Detect Compiler
#if defined(__clang__)
    #define ULIS2_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS2_GCC
#elif defined(_MSC_VER)
    #define ULIS2_MSVC
#else
    #define ULIS2_UNKNOWN_COMPILER
#endif

#ifdef ULIS2_MSVC
    #define _CRT_SECURE_NO_WARNINGS 1
#endif // ULIS2_MSVC

/////////////////////////////////////////////////////
// Detect Platform
#ifdef _WIN32
    #define ULIS2_WIN
   #ifdef _WIN64
      #define ULIS2_WIN64
   #else
      #define ULIS2_WIN32
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
         #define ULIS2_IOS_SIMULATOR
    #elif TARGET_OS_IPHONE
        #define ULIS2_IOS_DEVICE
    #elif TARGET_OS_MAC
        #define ULIS2_MACOS
    #else
        #error "Unknown Apple platform"
    #endif
#elif __linux__
    #define ULIS2_LINUX
#elif __unix__
    #define ULIS2_UNIX
#elif defined(_POSIX_VERSION)
    #define ULIS2_POSIX
#else
    #error "Unknown Platform"
#endif

/////////////////////////////////////////////////////
// Force Inline Utility
#define ULIS2_ENABLE_FORCEINLINE
#define ULIS2_ENABLE_VECTORCALL

#ifdef ULIS2_ENABLE_FORCEINLINE
    #if defined(__clang__)
    #define ULIS2_FORCEINLINE inline __attribute__ ((always_inline))
    #elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS2_FORCEINLINE inline __attribute__ ((always_inline))
    #elif defined(_MSC_VER)
    #define ULIS2_FORCEINLINE __forceinline
    #else
    #define ULIS2_FORCEINLINE inline
    #endif
#else
    #define ULIS2_FORCEINLINE inline
#endif // ULIS2_ENABLE_FORCEINLINE

#ifdef ULIS2_ENABLE_VECTORCALL
    #if defined(__clang__)
    #define ULIS2_VECTORCALL __vectorcall
    #elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS2_VECTORCALL
    #elif defined(_MSC_VER)
    #define ULIS2_VECTORCALL __vectorcall
    #else
    #define ULIS2_VECTORCALL __vectorcall
    #endif
#else
    #define ULIS2_VECTORCALL
#endif // ULIS2_ENABLE_FORCEINLINE

/////////////////////////////////////////////////////
// Export utility macros
#ifdef ULIS2_WIN
    #ifdef ULIS2_BUILD_SHARED
        #define ULIS2_SHARED
        #define ULIS2_API __declspec( dllexport )
        #define ULIS2_API_TEMPLATE template ULIS2_API
    #elif defined ULIS2_DYNAMIC_LIBRARY
        #define ULIS2_SHARED
        #define ULIS2_API __declspec( dllimport )
        #define ULIS2_API extern template ULIS2_API
    #else
        #define ULIS2_STATIC
        #define ULIS2_API
        #define ULIS2_API_TEMPLATE template
    #endif
#else
    #define ULIS2_API
    #define ULIS2_API_TEMPLATE template
#endif

/////////////////////////////////////////////////////
// Erors
#pragma warning(disable : 4251)     // Shut down dll interface warnings.
#pragma warning(disable : 26812)    // Shut non-class enum warnings.

/////////////////////////////////////////////////////
// Define Namespaces
#define ULIS2_NAMESPACE_NAME        ULIS2
#define ULIS2_SHORT_NAMESPACE_NAME  ul2
#define ULIS2_NAMESPACE_BEGIN       namespace ULIS2_NAMESPACE_NAME {
#define ULIS2_NAMESPACE_END         }
#define ULIS2_FDECL_CLASS( i )      ULIS2_NAMESPACE_BEGIN class i ; ULIS2_NAMESPACE_END
#define ULIS2_FDECL_STRUCT( i )     ULIS2_NAMESPACE_BEGIN struct i ; ULIS2_NAMESPACE_END
namespace ULIS2_NAMESPACE_NAME {}
namespace ULIS2_SHORT_NAMESPACE_NAME = ULIS2_NAMESPACE_NAME;

/////////////////////////////////////////////////////
// Version Specification
#define ULIS2_VERSION_MAJOR      2
#define ULIS2_VERSION_MINOR      0
#define ULIS2_VERSION_MAJOR_STR  "2"
#define ULIS2_VERSION_MINOR_STR  "0"

/////////////////////////////////////////////////////
// Crash Behaviours
#define ULIS2_CRASH             *((volatile int*)0) = 0

/////////////////////////////////////////////////////
// Assert Behaviours
#ifdef ULIS2_DEBUG
    #define ULIS2_ASSERT( cond, log )  if( !( cond ) ) { std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "Assertion failed: " << log << std::endl; ULIS2_CRASH; }
#else
    #define ULIS2_ASSERT( cond, log )
#endif
