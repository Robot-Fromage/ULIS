// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Platform.h
* @author       Clement Berthaud
* @brief        This file provides platform definitions for the ULIS library.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <iostream>

/////////////////////////////////////////////////////
// Macros detect def version
#ifdef  ULIS_DEF_VERSION_MAJOR
#define ULIS_VERSION_MAJOR ULIS_DEF_VERSION_MAJOR
#else
#define ULIS_VERSION_MAJOR 0
#endif //  ULIS_DEF_VERSION_MAJOR

#ifdef  ULIS_DEF_VERSION_MINOR
#define ULIS_VERSION_MINOR ULIS_DEF_VERSION_MINOR
#else
#define ULIS_VERSION_MINOR 0
#endif //  ULIS_DEF_VERSION_MINOR

#ifdef  ULIS_DEF_VERSION_PATCH
#define ULIS_VERSION_PATCH ULIS_DEF_VERSION_PATCH
#else
#define ULIS_VERSION_PATCH 0
#endif //  ULIS_DEF_VERSION_PATCH

#ifdef  ULIS_DEF_GIT_COMMIT_HASH
#define ULIS_GIT_COMMIT_HASH ULIS_DEF_GIT_COMMIT_HASH
#else
#define ULIS_GIT_COMMIT_HASH 0
#endif //  ULIS_DEF_GIT_COMMIT_HASH

#ifdef  ULIS_DEF_GIT_COMMIT_ABBREVIATED_HASH
#define ULIS_GIT_COMMIT_ABBREVIATED_HASH ULIS_DEF_GIT_COMMIT_ABBREVIATED_HASH
#else
#define ULIS_GIT_COMMIT_ABBREVIATED_HASH 0
#endif //  ULIS_DEF_GIT_COMMIT_ABBREVIATED_HASH

#ifdef  ULIS_DEF_GIT_BRANCH_NAME
#define ULIS_GIT_BRANCH_NAME ULIS_DEF_GIT_BRANCH_NAME
#else
#define ULIS_GIT_BRANCH_NAME 0
#endif //  ULIS_DEF_GIT_BRANCH_NAME

#define ULIS_XSTRINGIFY( X ) #X
#define ULIS_STRINGIFY( X ) ULIS_XSTRINGIFY( X )
#define ULIS_VERSION_MAJOR_STR ULIS_STRINGIFY( ULIS_VERSION_MAJOR )
#define ULIS_VERSION_MINOR_STR ULIS_STRINGIFY( ULIS_VERSION_MINOR )
#define ULIS_VERSION_PATCH_STR ULIS_STRINGIFY( ULIS_VERSION_PATCH )
#define ULIS_GIT_COMMIT_HASH_STR                ULIS_STRINGIFY( ULIS_GIT_COMMIT_HASH )
#define ULIS_GIT_COMMIT_ABBREVIATED_HASH_STR    ULIS_STRINGIFY( ULIS_GIT_COMMIT_ABBREVIATED_HASH )
#define ULIS_GIT_BRANCH_NAME_STR                ULIS_STRINGIFY( ULIS_GIT_BRANCH_NAME )

/////////////////////////////////////////////////////
// Detect Build Configuration
#ifndef ULIS_RELWITHDEBINFO
    #ifdef NDEBUG
        #define ULIS_RELEASE
    #else // !NDEBUG
        #define ULIS_DEBUG
    #endif // !NDEBUG
#endif

/////////////////////////////////////////////////////
// Detect Compiler
#if defined(__clang__)
    #define ULIS_CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS_GCC
#elif defined(_MSC_VER)
    #define ULIS_MSVC
#elif defined(__MINGW64__)
    #define ULIS_MINGW64
#else
    #define ULIS_UNKNOWN_COMPILER
#endif

/////////////////////////////////////////////////////
// Disable CRT Secure non standard msvc versions of functions such as strcpy_s
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

/////////////////////////////////////////////////////
// Detect Platform
#ifdef __EMSCRIPTEN__
    #define ULIS_EMSCRIPTEN
#else
    #ifdef _WIN32
        #define ULIS_WIN
       #ifdef _WIN64
          #define ULIS_WIN64
       #else
          #define ULIS_WIN32
       #endif
    #elif __APPLE__
        #include "TargetConditionals.h"
        #if TARGET_IPHONE_SIMULATOR
             #define ULIS_IOS_SIMULATOR
        #elif TARGET_OS_IPHONE
            #define ULIS_IOS_DEVICE
        #elif TARGET_OS_MAC
            #define ULIS_MACOS
        #else
            #error "Unknown Apple platform"
        #endif
    #elif __linux__
        #define ULIS_LINUX
    #elif __unix__
        #define ULIS_UNIX
    #elif defined(_POSIX_VERSION)
        #define ULIS_POSIX
    #else
        #error "Unknown Platform"
    #endif
#endif

/////////////////////////////////////////////////////
// Disable thread and SIMD for Emscripten target
#ifdef ULIS_EMSCRIPTEN
#define ULIS_NO_THREAD_SUPPORT
#define ULIS_NO_SIMD_SUPPORT
#endif

/////////////////////////////////////////////////////
// Force Inline Utility
#define ULIS_ENABLE_FORCEINLINE
#define ULIS_ENABLE_VECTORCALL
#define ULIS_ENABLE_RESTRICT

#ifdef ULIS_ENABLE_FORCEINLINE
    #if defined(__clang__)
    #define ULIS_FORCEINLINE inline __attribute__ ((always_inline))
    #elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS_FORCEINLINE inline __attribute__ ((always_inline))
    #elif defined(_MSC_VER)
    #define ULIS_FORCEINLINE __forceinline
    #else
    #define ULIS_FORCEINLINE inline
    #endif
#else
    #define ULIS_FORCEINLINE inline
#endif // ULIS_ENABLE_FORCEINLINE

#ifdef ULIS_ENABLE_VECTORCALL
    #if defined(__clang__)
    #define ULIS_VECTORCALL __vectorcall
    #elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS_VECTORCALL
    #elif defined(_MSC_VER)
    #define ULIS_VECTORCALL __vectorcall
    #else
    #define ULIS_VECTORCALL __vectorcall
    #endif
#else
    #define ULIS_VECTORCALL
#endif // ULIS_ENABLE_FORCEINLINE

#ifdef ULIS_ENABLE_RESTRICT
    #if defined(__clang__)
    #define ULIS_RESTRICT __restrict
    #elif defined(__GNUC__) || defined(__GNUG__)
    #define ULIS_RESTRICT __restrict
    #elif defined(_MSC_VER)
    #define ULIS_RESTRICT __restrict
    #else
    #define ULIS_RESTRICT __restrict
    #endif
#else
    #define ULIS_RESTRICT
#endif // ULIS_ENABLE_RESTRICT

/////////////////////////////////////////////////////
// Export utility macros
#ifdef ULIS_WIN
    #ifdef ULIS_STATIC_LIBRARY
        #define ULIS_BUILT_AS_STATIC_LIBRARY
        #define ULIS_API
    #else
        #ifdef ULIS_BUILD_SHARED
            #define ULIS_BUILT_AS_SHARED_LIBRARY
            #define ULIS_API __declspec( dllexport )
        #else
            #define ULIS_BUILT_AS_SHARED_LIBRARY
            #define ULIS_API __declspec( dllimport )
        #endif
    #endif
#endif

/////////////////////////////////////////////////////
// Macros for Thread and SIMD activation, for embeded targets or WASM
#ifndef ULIS_NO_THREAD_SUPPORT
#define ULIS_COMPILED_WITH_THREAD_SUPPORT
#endif // !ULIS_NO_THREAD_SUPPORT

#ifndef ULIS_NO_SIMD_SUPPORT
#define ULIS_COMPILED_WITH_SIMD_SUPPORT
#endif // !ULIS_NO_SIMD_SUPPORT

#ifndef ULIS_NO_FILESYSTEM_SUPPORT
#define ULIS_COMPILED_WITH_FILESYSTEM_SUPPORT
#endif // !ULIS_NO_SIMD_SUPPORT

/////////////////////////////////////////////////////
// Macros for features support
#ifndef ULIS_FEATURE_NO_BLEND
#define ULIS_FEATURE_BLEND_ENABLED
#endif // !ULIS_FEATURE_NO_BLEND

#ifndef ULIS_FEATURE_NO_CLEAR
#define ULIS_FEATURE_CLEAR_ENABLED
#endif // !ULIS_FEATURE_NO_CLEAR

#ifndef ULIS_FEATURE_NO_CONV
#define ULIS_FEATURE_CONV_ENABLED
#endif // !ULIS_FEATURE_NO_CONV

#ifndef ULIS_FEATURE_NO_COPY
#define ULIS_FEATURE_COPY_ENABLED
#endif // !ULIS_FEATURE_NO_COPY

#ifndef ULIS_FEATURE_NO_FILL
#define ULIS_FEATURE_FILL_ENABLED
#endif // !ULIS_FEATURE_NO_FILL

#ifndef ULIS_FEATURE_NO_TEXT
#define ULIS_FEATURE_TEXT_ENABLED
#endif // !ULIS_FEATURE_NO_TEXT

#ifndef ULIS_FEATURE_NO_TRANSFORM
#define ULIS_FEATURE_TRANSFORM_ENABLED
#endif // !ULIS_FEATURE_NO_TRANSFORM

/////////////////////////////////////////////////////
// Erors
//#pragma warning(disable : 4251)     // Shut down dll interface warnings.
//#pragma warning(disable : 26812)    // Shut non-class enum warnings.
#pragma warning(disable : 4010)     // Shut single-line comment contains line-continuation character
#pragma warning(disable : 4996)     // Shut CRT SECURE
/////////////////////////////////////////////////////
// Define Namespaces
#define ULIS_NAMESPACE_NAME        ULIS
//#define ULIS_SHORT_NAMESPACE_NAME  ul
#define ULIS_NAMESPACE_BEGIN       namespace ULIS_NAMESPACE_NAME {
#define ULIS_NAMESPACE_END         }
namespace ULIS_NAMESPACE_NAME {}
//namespace ULIS_SHORT_NAMESPACE_NAME = ULIS_NAMESPACE_NAME;

/////////////////////////////////////////////////////
// Crash Behaviours
#define ULIS_CRASH *((volatile int*)0) = 0

/////////////////////////////////////////////////////
// Assert Behaviours

#if defined( ULIS_DEBUG ) || defined( ULIS_RELWITHDEBINFO ) || defined( ULIS_FORCE_ASSERT )
    #define ULIS_ASSERT( cond, log )  if( !( cond ) ) { std::cout << __FILE__ << " " << __FUNCTION__ << " " << __LINE__ << " " << "Assertion failed: " << log << std::endl; ULIS_CRASH; }
    #define ULIS_ASSERT_ENABLED
#else
    #define ULIS_ASSERT( cond, log )
#endif

/////////////////////////////////////////////////////
// SIMD PATCH FOR GNU < 9
// CHECK: this conflicts with xcode sometimes identifying itself as GNUC < 9 but defining the appropriate simd instructions.
#ifdef ULIS_COMPILED_WITH_SIMD_SUPPORT
#if defined( ULIS_GCC ) && ( defined( ULIS_WIN ) || defined( ULIS_LINUX ) )
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
#endif

/////////////////////////////////////////////////////
// Conditional compile time detection macro in order to decide if we should include SIMD versions in the various dispatch
#ifdef ULIS_COMPILED_WITH_SIMD_SUPPORT
    #ifdef __AVX2__
        #define ULIS_COMPILETIME_AVX_SUPPORT
    #endif
    #ifdef __SSE4_2__
        #define ULIS_COMPILETIME_SSE_SUPPORT
    #endif
#endif

/////////////////////////////////////////////////////
// Conditional Debug Statistics
#if defined( ULIS_DEBUG ) || defined( ULIS_RELWITHDEBINFO ) || defined( ULIS_FORCE_STATISTICS )
    #define ULIS_STATISTICS_ENABLED
#endif

/////////////////////////////////////////////////////
// Disable REGISTER Keyword for LCMS2
#define CMS_NO_REGISTER_KEYWORD

