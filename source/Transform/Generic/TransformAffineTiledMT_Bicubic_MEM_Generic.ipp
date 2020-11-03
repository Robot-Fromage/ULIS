// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         TransformAffineTiledMT_Bicubic_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Transform/TransformArgs.h"
#include "Transform/TransformHelpers.h"
#include "Thread/ThreadPool.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

ULIS3_NAMESPACE_BEGIN
template< typename T > void
InvokeTransformAffineTiledMTProcessScanline_Bicubic_MEM_Generic( tByte* iDst, int32 iLine, std::shared_ptr< const FTransformArgs > iInfo ) {
    const FTransformArgs&   info    = *iInfo;
    const FFormatInfo&      fmt     = info.destination->FormatInfo();
    tByte*                  dst     = iDst;

    glm::vec3 point_in_dst( info.dst_roi.x, info.dst_roi.y + iLine, 1.f );
    glm::vec2 point_in_src( info.inverseTransform * point_in_dst );
    glm::vec2 src_dx( info.inverseTransform * glm::vec3( 1.f, 0.f, 0.f ) );

    tByte* p00 = new tByte[ fmt.BPP * 4 ];      tByte* p01 = new tByte[ fmt.BPP * 4 ];
    tByte* p10 = p00 + fmt.BPP;                 tByte* p11 = p01 + fmt.BPP;
    tByte* p20 = p10 + fmt.BPP;                 tByte* p21 = p11 + fmt.BPP;
    tByte* p30 = p20 + fmt.BPP;                 tByte* p31 = p21 + fmt.BPP;
    tByte* p02 = new tByte[ fmt.BPP * 4 ];      tByte* p03 = new tByte[ fmt.BPP * 4 ];
    tByte* p12 = p02 + fmt.BPP;                 tByte* p13 = p03 + fmt.BPP;
    tByte* p22 = p12 + fmt.BPP;                 tByte* p23 = p13 + fmt.BPP;
    tByte* p32 = p22 + fmt.BPP;                 tByte* p33 = p23 + fmt.BPP;
    float* hh0 = new float[ fmt.SPP * 4 ];
    float* hh1 = new float[ fmt.SPP * 4 ];
    float* hh2 = new float[ fmt.SPP * 4 ];
    float* hh3 = new float[ fmt.SPP * 4 ];

    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        const int   src_x   = static_cast< int >( floor( point_in_src.x ) );
        const int   src_y   = static_cast< int >( floor( point_in_src.y ) );
        const float tx      = point_in_src.x - src_x;
        const float ty      = point_in_src.y - src_y;

        const int xm1 = FMaths::PyModulo( src_x - 1, info.src_roi.w );
        const int xp0 = FMaths::PyModulo( src_x    , info.src_roi.w );
        const int xp1 = FMaths::PyModulo( src_x + 1, info.src_roi.w );
        const int xp2 = FMaths::PyModulo( src_x + 2, info.src_roi.w );
        const int ym1 = FMaths::PyModulo( src_y - 1, info.src_roi.h );
        const int yp0 = FMaths::PyModulo( src_y    , info.src_roi.h );
        const int yp1 = FMaths::PyModulo( src_y + 1, info.src_roi.h );
        const int yp2 = FMaths::PyModulo( src_y + 2, info.src_roi.h );
        #define GETPIXEL( _C, _X, _Y ) { memcpy( _C, info.source->PixelPtr( _X, _Y ), fmt.BPP ); }
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

template< typename T > void
TransformAffineTiledMT_Bicubic_MEM_Generic( std::shared_ptr< const FTransformArgs > iInfo ) {
    const FTransformArgs&   info        = *iInfo;
    tByte*                  dst         = info.destination->DataPtr();
    const tSize             dst_bps     = info.destination->BytesPerScanLine();
    const tSize             dst_decal_y = info.dst_roi.y;
    const tSize             dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeTransformAffineTiledMTProcessScanline_Bicubic_MEM_Generic< T >
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo );
}

ULIS3_NAMESPACE_END

