// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ResizeMT_NN_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Transform/RGBA8/ResizeMT_NN_SSE_RGBA8.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeResizeMT_NN_SSE_RGBA8(
      const FTransformJobArgs* jargs
    , const FResizeCommandArgs* cargs
)
{
    const FResizeCommandArgs&  info    = *iInfo;
    const FFormatMetrics&  fmt     = info.destination->FormatMetrics();
    uint8*              dst     = iDst;

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
            memcpy( dst, info.source->PixelBits( src_x, src_y ), fmt.BPP );

        dst += fmt.BPP;
        point_in_src += src_dx;
    }
}

ULIS_DEFINE_RESIZE_COMMAND_SPECIALIZATION( ResizeMT_NN_SSE_RGBA8 )

ULIS_NAMESPACE_END

