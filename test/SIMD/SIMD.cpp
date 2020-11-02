// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/**
*
*   ULIS
*__________________
*
* @file         Test.cpp
* @author       Clement Berthaud
* @brief        Test application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
#include <immintrin.h>
#include <vectorclass.h>
#include <stdlib.h>
#include <chrono>
#include <iostream>

#ifndef GET_CACHE_LINE_SIZE_H_INCLUDED
#define GET_CACHE_LINE_SIZE_H_INCLUDED

// Author: Nick Strupat
// Date: October 29, 2010
// Returns the cache line size (in bytes) of the processor, or 0 on failure

#include <stddef.h>
size_t cache_line_size();

#if defined(__APPLE__)

#include <sys/sysctl.h>
size_t cache_line_size() {
    size_t line_size = 0;
    size_t sizeof_line_size = sizeof(line_size);
    sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, 0, 0);
    return line_size;
}

#elif defined(_WIN32)

#include <stdlib.h>
#include <windows.h>
size_t cache_line_size() {
    size_t line_size = 0;
    DWORD buffer_size = 0;
    DWORD i = 0;
    SYSTEM_LOGICAL_PROCESSOR_INFORMATION * buffer = 0;

    GetLogicalProcessorInformation(0, &buffer_size);
    buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *)malloc(buffer_size);
    GetLogicalProcessorInformation(&buffer[0], &buffer_size);

    for (i = 0; i != buffer_size / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
        if (buffer[i].Relationship == RelationCache && buffer[i].Cache.Level == 1) {
            line_size = buffer[i].Cache.LineSize;
            break;
        }
    }

    free(buffer);
    return line_size;
}

#elif defined(linux)

#include <stdio.h>
size_t cache_line_size() {
    FILE * p = 0;
    p = fopen("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size", "r");
    unsigned int i = 0;
    if (p) {
        fscanf(p, "%d", &i);
        fclose(p);
    }
    return i;
}

#else
#error Unrecognized platform
#endif

#endif

using namespace ::ul3;

int
main() {
    size_t cache = cache_line_size();
    std::cout << cache << std::endl;


    FVec2I a( 5, 5 );

    const int repeat = 1;
    // unaligned premult: 0.0010823

    const int size = 64;
    const int area = size * size;
    const int bps  = 8;
    const int spp  = 4;
    const int bpp  = 4 * 8;
    const int bytesTotal = area * bpp;
    const int numPixelsAtATime = 4;
    const int len = area / numPixelsAtATime;
    const float opacity = 0.5f;
    const int iopacity = static_cast< int >( opacity * 255 );
    const __m128i alpha16 = _mm_set1_epi16( iopacity );
    uint8* base     = reinterpret_cast< uint8* >( _aligned_malloc( bytesTotal, 16 ) );
    uint8* over     = reinterpret_cast< uint8* >( _aligned_malloc( bytesTotal, 16 ) );
    memset( base, 0xFF, bytesTotal );
    memset( over, 0xAA, bytesTotal );

    const __m128i allo = _mm_setr_epi32( 0xff03ff03, 0xff03ff03, 0xff07ff07, 0x0ff7ff07 );
    const __m128i alhi = _mm_setr_epi32( 0xff0bff0b, 0xff0bff0b, 0xff0fff0f, 0x0fffff0f );
    const __m128i zero = _mm_setr_epi32( 0x00000000, 0x00000000, 0x00000000, 0x00000000 );
    const __m128i i255 = _mm_setr_epi32( 0xff00ff00, 0xff00ff00, 0xff00ff00, 0xff00ff00 );

    auto startTime = std::chrono::steady_clock::now();
    for( int r = 0; r < repeat; ++r ) {

        const __m128i* psrc = reinterpret_cast< const __m128i* >( over );
              __m128i* pbdp = reinterpret_cast< __m128i* >( base );

        for( uint32 i = 0; i < len; ++i ) {
            const __m128i src = _mm_load_si128( psrc );
            const __m128i bdp = _mm_load_si128( pbdp );

            __m128i under0      = _mm_cvtepu8_epi16( bdp );
            __m128i under1      = _mm_unpackhi_epi8( bdp, zero );
            __m128i over0       = _mm_cvtepu8_epi16( src );
            __m128i over1       = _mm_unpackhi_epi8( src, zero );
            __m128i alpha0      = _mm_mullo_epi16(_mm_shuffle_epi8( src, allo ), alpha16 );
            __m128i alpha1      = _mm_mullo_epi16(_mm_shuffle_epi8( src, alhi ), alpha16 );
            __m128i invAlpha0   = _mm_xor_si128( i255, alpha0 );
            __m128i invAlpha1   = _mm_xor_si128( i255, alpha1 );
            __m128i underMul0   = _mm_mulhi_epu16( under0, invAlpha0 );
            __m128i underMul1   = _mm_mulhi_epu16( under1, invAlpha1 );
            __m128i overMul0    = _mm_mulhi_epu16( over0, alpha0 );
            __m128i overMul1    = _mm_mulhi_epu16( over1, alpha1 );
            __m128i underFinal  = _mm_packus_epi16( underMul0, underMul1 );
            __m128i overFinal   = _mm_packus_epi16( overMul0, overMul1 );
            __m128i res         = _mm_adds_epu8( overFinal, underFinal );
            _mm_storeu_si128( pbdp, res );
            ++psrc;
            ++pbdp;

            /*
            const __m128i u16_0 = _mm_cvtepu8_epi16(bdp);
            const __m128i u16_1 = _mm_unpackhi_epi8(bdp, zero);
            const __m128i al8_0 = _mm_shuffle_epi8 (src,  allo);
            const __m128i al8_1 = _mm_shuffle_epi8 (src,  alhi);
            const __m128i mal_0 = _mm_xor_si128(i255, al8_0);
            const __m128i mal_1 = _mm_xor_si128(i255, al8_1);
            const __m128i mul_0 = _mm_mulhi_epu16  (u16_0, mal_0);
            const __m128i mul_1 = _mm_mulhi_epu16  (u16_1, mal_1);
            const __m128i pixel = _mm_packus_epi16 (mul_0, mul_1);
            const __m128i res   = _mm_adds_epi8(src, pixel);
            _mm_storeu_si128( pbdp, res );
            ++psrc;
            ++pbdp;
            */
        }
    }

    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = static_cast< double >( std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count() ) / static_cast< double >( repeat );
    std::cout << deltaMs << std::endl;

    _aligned_free( base );
    _aligned_free( over );
    return  0;
}

