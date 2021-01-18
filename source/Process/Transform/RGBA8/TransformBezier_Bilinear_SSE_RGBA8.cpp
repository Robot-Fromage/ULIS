// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformBezier_Bilinear_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Transform/RGBA8/TransformBezier_Bilinear_SSE_RGBA8.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeTransformBezierMT_Bilinear_SSE_RGBA8( uint8* iDst, int32 iLine, std::shared_ptr< const FTransformCommandArgs > iInfo, std::shared_ptr< const FBlock > iField, std::shared_ptr< const FBlock > iMask, const Vec4i iIDT ) {
    const FTransformCommandArgs&   info    = *iInfo;
    const FFormatMetrics&      fmt     = info.destination->FormatMetrics();
    uint8*                  dst     = iDst;
    const float*            field   = reinterpret_cast< const float* >( iField->ScanlineBits( iLine ) );
    const uint8*            mask    = reinterpret_cast< const uint8* >( iMask->ScanlineBits( iLine ) );
    const int rangex = info.src_roi.w - 1;
    const int rangey = info.src_roi.h - 1;

    Vec4f c00, c10, c11, c01, hh0, hh1, res, alp;

    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        if( *mask & 0xFF ) {
            float srcxf = field[0] * rangex;
            float srcyf = field[1] * rangey;
            const int   left    = static_cast< int >( floor( srcxf ) );
            const int   top     = static_cast< int >( floor( srcyf ) );
            const int   right   = left + 1;
            const int   bot     = top + 1;
            const Vec4f tx      = srcxf - left;
            const Vec4f ux      = 1.f - tx;
            const Vec4f ty      = srcyf - top;
            const Vec4f uy      = 1.f - ty;

            #define LOAD( X )   _mm_cvtepi32_ps( _mm_cvtepu8_epi32( _mm_loadu_si128( reinterpret_cast< const __m128i* >( X ) ) ) )
            #define TEMP( _C, _X, _Y )                                                                                                                          \
                if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) {                                                                                      \
                    const uint8* pptr = info.source->PixelBits( _X, _Y );                                                                                        \
                    Vec4f _ch = LOAD( pptr );                                                                                                                   \
                    Vec4f _al = _mm_set_ps1( pptr[ fmt.AID ] );                                                                                                 \
                    _C = lookup8( iIDT, ( _ch * _al ) / 255.f, _al );                                                                                           \
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

        }
        dst += fmt.BPP;
        field += 2;
        ++mask;
    }
}

ULIS_DEFINE_TRANSFORM_COMMAND_SPECIALIZATION( TransformBezierMT_Bilinear_SSE_RGBA8 )

ULIS_NAMESPACE_END

