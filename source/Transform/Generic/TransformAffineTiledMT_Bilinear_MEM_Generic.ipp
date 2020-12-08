// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformAffineTiledMT_Bilinear_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Transform/TransformArgs.h"
#include "Transform/TransformHelpers.h"
#include "Thread/ThreadPool.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

ULIS_NAMESPACE_BEGIN
template< typename T > void
InvokeTransformAffineTiledMT_Bilinear_MEM_Generic( uint8* iDst, int32 iLine, std::shared_ptr< const FTransformArgs > iInfo ) {
    const FTransformArgs&   info    = *iInfo;
    const FFormatMetrics&      fmt     = info.destination->FormatMetrics();
    uint8*                  dst     = iDst;

    FVec3F point_in_dst( info.dst_roi.x, info.dst_roi.y + iLine, 1.f );
    FVec2F point_in_src( info.inverseTransform * point_in_dst );
    FVec2F src_dx( info.inverseTransform * FVec3F( 1.f, 0.f, 0.f ) );
    uint8* c00 = new uint8[ fmt.BPP * 4 ];
    uint8* c10 = c00 + fmt.BPP;
    uint8* c11 = c10 + fmt.BPP;
    uint8* c01 = c11 + fmt.BPP;
    uint8* hh0 = new uint8[ fmt.BPP * 2 ];
    uint8* hh1 = hh0 + fmt.BPP;

    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        const float modx = FMath::PyModulo( point_in_src.x, static_cast< float >( info.src_roi.w ) );
        const float mody = FMath::PyModulo( point_in_src.y, static_cast< float >( info.src_roi.h ) );
        const int   left    = static_cast< int >( modx );
        const int   top     = static_cast< int >( mody );
        const int   right   = ( left + 1 ) % info.src_roi.w;
        const int   bot     = ( top  + 1 ) % info.src_roi.h;
        const float tx      = modx - left;
        const float ux      = 1.f - tx;
        const float ty      = mody - top;
        const float uy      = 1.f - ty;

        #define TEMP( _C, _X, _Y ) { memcpy( _C, info.source->PixelBits( _X, _Y ), fmt.BPP ); }
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
    uint8*                  dst         = info.destination->Bits();
    const uint32             dst_bps     = info.destination->BytesPerScanLine();
    const uint32             dst_decal_y = info.dst_roi.y;
    const uint32             dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeTransformAffineTiledMT_Bilinear_MEM_Generic< T >
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo );
}

ULIS_NAMESPACE_END

