// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformAffineMT_NN_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Transform/RGBA8/TransformAffineMT_NN_SSE_RGBA8.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeTransformAffineMT_NN_SSE_RGBA8( uint8* iDst, int32 iLine, std::shared_ptr< const FTransformCommandArgs > iInfo ) {
    const FTransformCommandArgs&   info    = *iInfo;
    const FFormatMetrics&      fmt     = info.destination->FormatMetrics();
    uint8*                  dst     = iDst;

    FVec3F point_in_dst( info.dst_roi.x, info.dst_roi.y + iLine, 1.f );
    FVec2F point_in_src( info.inverseTransform * point_in_dst );
    FVec2F src_dx( info.inverseTransform * FVec3F( 1.f, 0.f, 0.f ) );

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

ULIS_DEFINE_TRANSFORM_COMMAND_SPECIALIZATION( TransformAffineMT_NN_SSE_RGBA8 )

ULIS_NAMESPACE_END

