// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
* @file         ResizeMT_NN_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic transform entry point functions.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Maths/Geometry.h"
#include "Transform/Dispatch/TransformInfo.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN
template< typename T > void
InvokeResizeMTProcessScanline_NN_MEM_Generic( tByte* iDst, int32 iLine, std::shared_ptr< const _FResizeInfoPrivate > iInfo ) {
    const _FResizeInfoPrivate&      info    = *iInfo;
    const FFormatInfo&              fmt     = info.destination->FormatInfo();
    tByte*                          dst     = iDst;

    FVec2F point_in_dst( info.dst_roi.x, info.dst_roi.y + iLine );
    FVec2F point_in_src( info.inverseScale * ( point_in_dst - info.shift ) + FVec2F( info.src_roi.x, info.src_roi.y ) );
    FVec2F src_dx( info.inverseScale * FVec2F( 1.f, 0.f ) );

    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        int src_x = static_cast< int >( point_in_src.x );
        int src_y = static_cast< int >( point_in_src.y );
        if( src_x >= minx && src_y >= miny && src_x < maxx && src_y < maxy )
            memcpy( dst, info.source->PixelPtr( src_x, src_y ), fmt.BPP );

        dst += fmt.BPP;
        point_in_src += src_dx;
    }
}

template< typename T > void
ResizeMT_NN_MEM_Generic( std::shared_ptr< const _FResizeInfoPrivate > iInfo ) {
    const _FResizeInfoPrivate&      info        = *iInfo;
    tByte*                          dst         = info.destination->DataPtr();
    const tSize                     dst_bps     = info.destination->BytesPerScanLine();
    const tSize                     dst_decal_y = info.dst_roi.y;
    const tSize                     dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeResizeMTProcessScanline_NN_MEM_Generic< T >
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo );
}

ULIS3_NAMESPACE_END
