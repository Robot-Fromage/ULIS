// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_Test_SSE.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include <ULIS_CORE>
#include <immintrin.h>
using namespace ::ULIS;

struct alignas( 16 ) FVectorSIMD128
{
    union {
        uint8   u8[16];
        uint16  u16[8];
        uint32  u32[4];
        float   f32[4];
        __m128i m128i;
        __m128  m128;
    };
};

struct alignas( 32 ) FVectorSIMD256
{
    union {
        uint8   u8[32];
        uint16  u16[16];
        uint32  u32[8];
        float   f32[8];
        __m256i m256i;
        __m256  m256;
    };
};


int main()
{
    uint8 values[] = { 0, 1, 2, 3, 4, 5, 6, 7 ,8, 9, 10, 11, 12, 13, 14, 15 };
    FVectorSIMD128 T8;
    FVectorSIMD128 T32;
    FVectorSIMD128 F32;
    T8.m128i    = _mm_loadu_si128( (const __m128i*)values );
    T32.m128i   = _mm_cvtepu8_epi32( T8.m128i );
    F32.m128    = _mm_cvtepi32_ps( T32.m128i );

    FVectorSIMD256 T25632;
    FVectorSIMD256 F25632;
    T25632.m256i    = _mm256_cvtepu8_epi32( T8.m128i );
    F25632.m256     = _mm256_cvtepi32_ps( T25632.m256i );
    //F25632.m256     = _mm256_mul_ps( F25632.m256, _mm256_set1_ps( 0.5 ) );

    for( int i = 0; i < 8; ++i )
        std::cout << (float)F25632.f32[i] << ",";



    return 0;
}

