// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformPerspectiveMT_Bicubic_MEM_Generic.h
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
InvokeTransformPerspectiveMT_Bicubic_MEM_Generic(
      const FTransformJobArgs* jargs
    , const FTransformCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;

    FVec2F pointInDst( static_cast< float >( cargs->dstRect.x ), static_cast< float >( cargs->dstRect.y + jargs->line ) );

    uint8* p00 = new uint8[ fmt.BPP * 4 ];      uint8* p01 = new uint8[ fmt.BPP * 4 ];
    uint8* p10 = p00 + fmt.BPP;                 uint8* p11 = p01 + fmt.BPP;
    uint8* p20 = p10 + fmt.BPP;                 uint8* p21 = p11 + fmt.BPP;
    uint8* p30 = p20 + fmt.BPP;                 uint8* p31 = p21 + fmt.BPP;
    uint8* p02 = new uint8[ fmt.BPP * 4 ];      uint8* p03 = new uint8[ fmt.BPP * 4 ];
    uint8* p12 = p02 + fmt.BPP;                 uint8* p13 = p03 + fmt.BPP;
    uint8* p22 = p12 + fmt.BPP;                 uint8* p23 = p13 + fmt.BPP;
    uint8* p32 = p22 + fmt.BPP;                 uint8* p33 = p23 + fmt.BPP;
    float* hh0 = new float[ fmt.SPP * 4 ];
    float* hh1 = new float[ fmt.SPP * 4 ];
    float* hh2 = new float[ fmt.SPP * 4 ];
    float* hh3 = new float[ fmt.SPP * 4 ];

    const int minx = cargs->srcRect.x;
    const int miny = cargs->srcRect.y;
    const int maxx = minx + cargs->srcRect.w;
    const int maxy = miny + cargs->srcRect.h;
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        FVec2F pointInSrc = cargs->inverseMatrix.ApplyHomography( pointInDst );
        const int   src_x   = static_cast< int >( floor( pointInSrc.x ) );
        const int   src_y   = static_cast< int >( floor( pointInSrc.y ) );
        const float tx      = pointInSrc.x - src_x;
        const float ty      = pointInSrc.y - src_y;

        #define GETPIXEL( _C, _X, _Y ) if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) { memcpy( _C, cargs->src.PixelBits( _X, _Y ), fmt.BPP ); } else { memset( _C, 0, fmt.BPP ); }
        GETPIXEL( p00, src_x - 1, src_y - 1 );  GETPIXEL( p01, src_x - 1, src_y + 0 );  GETPIXEL( p02, src_x - 1, src_y + 1 );  GETPIXEL( p03, src_x - 1, src_y + 2 );
        GETPIXEL( p10, src_x + 0, src_y - 1 );  GETPIXEL( p11, src_x + 0, src_y + 0 );  GETPIXEL( p12, src_x + 0, src_y + 1 );  GETPIXEL( p13, src_x + 0, src_y + 2 );
        GETPIXEL( p20, src_x + 1, src_y - 1 );  GETPIXEL( p21, src_x + 1, src_y + 0 );  GETPIXEL( p22, src_x + 1, src_y + 1 );  GETPIXEL( p23, src_x + 1, src_y + 2 );
        GETPIXEL( p30, src_x + 2, src_y - 1 );  GETPIXEL( p31, src_x + 2, src_y + 0 );  GETPIXEL( p32, src_x + 2, src_y + 1 );  GETPIXEL( p33, src_x + 2, src_y + 2 );
        #undef GETPIXEL
        SampleBicubicH< T >( hh0, p00, p10, p20, p30, fmt, tx );
        SampleBicubicH< T >( hh1, p01, p11, p21, p31, fmt, tx );
        SampleBicubicH< T >( hh2, p02, p12, p22, p32, fmt, tx );
        SampleBicubicH< T >( hh3, p03, p13, p23, p33, fmt, tx );
        SampleBicubicV< T >( dst, hh0, hh1, hh2, hh3, fmt, ty );

        dst += fmt.BPP;
        pointInDst.x += 1;
    }

    delete [] p00;
    delete [] p01;
    delete [] p02;
    delete [] p03;
    delete [] hh0;
    delete [] hh1;
    delete [] hh2;
    delete [] hh3;
}

ULIS_DEFINE_TRANSFORM_COMMAND_GENERIC( TransformPerspectiveMT_Bicubic_MEM_Generic )

ULIS_NAMESPACE_END

