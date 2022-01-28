// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformAffineTiledMT_Bicubic_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Transform/TransformArgs.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
template< typename T > void
InvokeTransformAffineTiledMT_Bicubic_MEM_Generic(
      const FTransformJobArgs* jargs
    , const FTransformCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;

    FVec3F point_in_dst( cargs->dstRect.x, cargs->dstRect.y + jargs->line, 1.f );
    FVec2F point_in_src( cargs->inverseMatrix * point_in_dst );
    FVec2F src_dx( cargs->inverseMatrix * FVec3F( 1.f, 0.f, 0.f ) );

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

    //const int minx = cargs->srcRect.x;
    //const int miny = cargs->srcRect.y;
    //const int maxx = minx + cargs->srcRect.w;
    //const int maxy = miny + cargs->srcRect.h;
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        const int   src_x   = static_cast< int >( floor( point_in_src.x ) );
        const int   src_y   = static_cast< int >( floor( point_in_src.y ) );
        const float tx      = point_in_src.x - src_x;
        const float ty      = point_in_src.y - src_y;

        const int xm1 = FMath::PyModulo( src_x - 1, cargs->srcRect.w );
        const int xp0 = FMath::PyModulo( src_x    , cargs->srcRect.w );
        const int xp1 = FMath::PyModulo( src_x + 1, cargs->srcRect.w );
        const int xp2 = FMath::PyModulo( src_x + 2, cargs->srcRect.w );
        const int ym1 = FMath::PyModulo( src_y - 1, cargs->srcRect.h );
        const int yp0 = FMath::PyModulo( src_y    , cargs->srcRect.h );
        const int yp1 = FMath::PyModulo( src_y + 1, cargs->srcRect.h );
        const int yp2 = FMath::PyModulo( src_y + 2, cargs->srcRect.h );
        #define GETPIXEL( _C, _X, _Y ) { memcpy( _C, cargs->src.PixelBits( _X, _Y ), fmt.BPP ); }
        GETPIXEL( p00, xm1,     ym1 );  GETPIXEL( p01, xm1,     yp0 );  GETPIXEL( p02, xm1,     yp1 );  GETPIXEL( p03, xm1,     yp2 );
        GETPIXEL( p10, xp0,     ym1 );  GETPIXEL( p11, xp0,     yp0 );  GETPIXEL( p12, xp0,     yp1 );  GETPIXEL( p13, xp0,     yp2 );
        GETPIXEL( p20, xp1,     ym1 );  GETPIXEL( p21, xp1,     yp0 );  GETPIXEL( p22, xp1,     yp1 );  GETPIXEL( p23, xp1,     yp2 );
        GETPIXEL( p30, xp2,     ym1 );  GETPIXEL( p31, xp2,     yp0 );  GETPIXEL( p32, xp2,     yp1 );  GETPIXEL( p33, xp2,     yp2 );
        #undef GETPIXEL
        SampleBicubicH< T >( hh0, p00, p10, p20, p30, fmt, tx );
        SampleBicubicH< T >( hh1, p01, p11, p21, p31, fmt, tx );
        SampleBicubicH< T >( hh2, p02, p12, p22, p32, fmt, tx );
        SampleBicubicH< T >( hh3, p03, p13, p23, p33, fmt, tx );
        SampleBicubicV< T >( dst, hh0, hh1, hh2, hh3, fmt, ty );

        dst += fmt.BPP;
        point_in_src += src_dx;
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

ULIS_DEFINE_TRANSFORM_COMMAND_GENERIC( TransformAffineTiledMT_Bicubic_MEM_Generic )

ULIS_NAMESPACE_END

