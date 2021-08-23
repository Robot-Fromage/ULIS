// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         TransformAffineTiledMT_Bilinear_MEM_Generic.ipp
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
InvokeTransformAffineTiledMTProcessScanline_Bilinear_MEM_Generic( tByte* iDst, int32 iLine, std::shared_ptr< const FTransformArgs > iInfo ) {
    const FTransformArgs&   info    = *iInfo;
    const FFormatInfo&      fmt     = info.destination->FormatInfo();
    tByte*                  dst     = iDst;

    glm::vec3 point_in_dst( info.dst_roi.x, info.dst_roi.y + iLine, 1.f );
    glm::vec2 point_in_src( info.inverseTransform * point_in_dst );
    glm::vec2 src_dx( info.inverseTransform * glm::vec3( 1.f, 0.f, 0.f ) );
    tByte* c00 = new tByte[ fmt.BPP * 4 ];
    tByte* c10 = c00 + fmt.BPP;
    tByte* c11 = c10 + fmt.BPP;
    tByte* c01 = c11 + fmt.BPP;
    tByte* hh0 = new tByte[ fmt.BPP * 2 ];
    tByte* hh1 = hh0 + fmt.BPP;

    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        const float modx = FMaths::PyFModulo( point_in_src.x, static_cast< float >( info.src_roi.w ) );
        const float mody = FMaths::PyFModulo( point_in_src.y, static_cast< float >( info.src_roi.h ) );
        const int   left    = static_cast< int >( modx );
        const int   top     = static_cast< int >( mody );
        const int   right   = ( left + 1 ) % info.src_roi.w;
        const int   bot     = ( top  + 1 ) % info.src_roi.h;
        const float tx      = modx - left;
        const float ux      = 1.f - tx;
        const float ty      = mody - top;
        const float uy      = 1.f - ty;

        #define TEMP( _C, _X, _Y ) { memcpy( _C, info.source->PixelPtr( _X, _Y ), fmt.BPP ); }
        TEMP( c00, left, top );
        TEMP( c10, right, top );
        TEMP( c11, right, bot );
        TEMP( c01, left, bot );
        #undef TEMP
        SampleBilinear< T >( hh0, c00, c10, fmt, tx, ux );
        SampleBilinear< T >( hh1, c01, c11, fmt, tx, ux );
        SampleBilinear< T >( dst, hh0, hh1, fmt, ty, uy );

        dst += fmt.BPP;
        point_in_src += src_dx;
    }

    delete [] c00;
    delete [] hh0;
}

template< typename T > void
TransformAffineTiledMT_Bilinear_MEM_Generic( std::shared_ptr< const FTransformArgs > iInfo ) {
    const FTransformArgs&   info        = *iInfo;
    tByte*                  dst         = info.destination->DataPtr();
    const tSize             dst_bps     = info.destination->BytesPerScanLine();
    const tSize             dst_decal_y = info.dst_roi.y;
    const tSize             dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeTransformAffineTiledMTProcessScanline_Bilinear_MEM_Generic< T >
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo );
}

ULIS3_NAMESPACE_END

