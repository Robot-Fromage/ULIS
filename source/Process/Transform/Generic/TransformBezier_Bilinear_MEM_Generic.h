// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformBezier_Bilinear_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Transform/TransformArgs.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
template< typename T > void
InvokeTransformBezierMT_Bilinear_MEM_Generic(
      const FTransformJobArgs* jargs
    , const FTransformCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;
    const float* field = reinterpret_cast< const float* >( iField->ScanlineBits( jargs->line ) );
    const uint8* mask  = reinterpret_cast< const uint8* >( iMask->ScanlineBits( jargs->line ) );
    const int rangex = cargs->srcRect.w - 1;
    const int rangey = cargs->srcRect.h - 1;

    uint8* c00 = new uint8[ fmt.BPP * 4 ];
    uint8* c10 = c00 + fmt.BPP;
    uint8* c11 = c10 + fmt.BPP;
    uint8* c01 = c11 + fmt.BPP;
    uint8* hh0 = new uint8[ fmt.BPP * 2 ];
    uint8* hh1 = hh0 + fmt.BPP;
    const int minx = cargs->srcRect.x;
    const int miny = cargs->srcRect.y;
    const int maxx = minx + cargs->srcRect.w;
    const int maxy = miny + cargs->srcRect.h;
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        if( *mask & 0xFF ) {
            float srcxf = field[0] * rangex;
            float srcyf = field[1] * rangey;
            const int   left    = static_cast< int >( floor( srcxf ) );
            const int   top     = static_cast< int >( floor( srcyf ) );
            const int   right   = left + 1;
            const int   bot     = top + 1;
            const float tx      = srcxf - left;
            const float ux      = 1.f - tx;
            const float ty      = srcyf - top;
            const float uy      = 1.f - ty;

            #define TEMP( _C, _X, _Y ) if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) { memcpy( _C, cargs->src.PixelBits( _X, _Y ), fmt.BPP ); } else { memset( _C, 255, fmt.BPP ); }
            TEMP( c00, left, top );
            TEMP( c10, right, top );
            TEMP( c11, right, bot );
            TEMP( c01, left, bot );
            #undef TEMP
            SampleBilinear< T >( hh0, c00, c10, fmt, tx, ux );
            SampleBilinear< T >( hh1, c01, c11, fmt, tx, ux );
            SampleBilinear< T >( dst, hh0, hh1, fmt, ty, uy );

        }
        dst += fmt.BPP;
        field += 2;
        ++mask;
    }

    delete [] c00;
    delete [] hh0;
}

ULIS_DEFINE_TRANSFORM_COMMAND_GENERIC( TransformBezierMT_Bilinear_MEM_Generic )

ULIS_NAMESPACE_END

