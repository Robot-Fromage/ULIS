// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformAffineMT_Bilinear_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Core/Core.h"
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
#include "Process/Transform/RGBA8/TransformAffineMT_Bilinear_SSE_RGBA8.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeTransformAffineMT_Bilinear_SSE_RGBA8(
      const FTransformJobArgs* jargs
    , const FTransformCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;
    //Vec4i _idt( fmt.IDT[0], fmt.IDT[1], fmt.IDT[2], fmt.IDT[3] );
    Vec4i _idt( 0, 1, 2, 3 );
    _idt.insert( fmt.AID, 4 );

    FVec3F point_in_dst( cargs->dstRect.x, cargs->dstRect.y + jargs->line, 1.f );
    FVec2F point_in_src( cargs->inverseMatrix * point_in_dst );
    FVec2F src_dx( cargs->inverseMatrix * FVec3F( 1.f, 0.f, 0.f ) );

    Vec4f c00, c10, c11, c01, hh0, hh1, res, alp;

    const int minx = cargs->srcRect.x;
    const int miny = cargs->srcRect.y;
    const int maxx = minx + cargs->srcRect.w;
    const int maxy = miny + cargs->srcRect.h;
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        const int   left    = static_cast< int >( floor( point_in_src.x ) );
        const int   top     = static_cast< int >( floor( point_in_src.y ) );
        const int   right   = left + 1;
        const int   bot     = top + 1;
        const Vec4f tx      = point_in_src.x - left;
        const Vec4f ux      = 1.f - tx;
        const Vec4f ty      = point_in_src.y - top;
        const Vec4f uy      = 1.f - ty;

        #define LOAD( X )   _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( X ) ) ) )
        #define TEMP( _C, _X, _Y )                                                                                                                          \
            if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) {                                                                                      \
                const uint8* pptr = cargs->src.PixelBits( _X, _Y );                                                                                         \
                Vec4f _ch = LOAD( pptr );                                                                                                                   \
                Vec4f _al = _mm_set_ps1( pptr[ fmt.AID ] );                                                                                                 \
                _C = lookup8( _idt, ( _ch * _al ) / 255.f, _al );                                                                                           \
            } else {                                                                                                                                        \
                _C = _mm_setzero_ps();                                                                                                                      \
            }

        TEMP( c00, left, top );
        TEMP( c10, right, top );
        TEMP( c11, right, bot );
        TEMP( c01, left, bot );
        #undef TEMP
        #undef LOAD
        hh0 = c00 * ux + c10 * tx;
        hh1 = c01 * ux + c11 * tx;
        res = hh0 * uy + hh1 * ty;
        alp = lookup4( fmt.AID, res );
        alp.insert( fmt.AID, 255.f );
        res = ( res * 255.f ) / alp;

        auto _pack = _mm_cvtps_epi32( res );
        _pack = _mm_packus_epi32( _pack, _pack );
        _pack = _mm_packus_epi16( _pack, _pack );
        *( uint32* )dst = static_cast< uint32 >( _mm_cvtsi128_si32( _pack ) );

        dst += fmt.BPP;
        point_in_src += src_dx;
    }
}

ULIS_DEFINE_TRANSFORM_COMMAND_SPECIALIZATION( TransformAffineMT_Bilinear_SSE_RGBA8 )

ULIS_NAMESPACE_END
#endif // ULIS_COMPILETIME_SSE_SUPPORT

