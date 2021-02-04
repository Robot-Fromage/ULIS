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
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;

    FVec2F point_in_dst( cargs->dstRect.x, cargs->dstRect.y + jargs->line );
    FVec2F point_in_src( cargs->inverseScale * ( point_in_dst - cargs->shift ) + FVec2F( cargs->srcRect.x, cargs->srcRect.y ) );
    FVec2F src_dx( cargs->inverseScale * FVec2F( 1.f, 0.f ) );

    const int minx = cargs->srcRect.x;
    const int miny = cargs->srcRect.y;
    const int maxx = minx + cargs->srcRect.w;
    const int maxy = miny + cargs->srcRect.h;
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        int src_x = static_cast< int >( point_in_src.x );
        int src_y = static_cast< int >( point_in_src.y );
        if( src_x >= minx && src_y >= miny && src_x < maxx && src_y < maxy )
            memcpy( dst, cargs->src.PixelBits( src_x, src_y ), fmt.BPP );

        dst += fmt.BPP;
        point_in_src += src_dx;
    }
}

ULIS_DEFINE_RESIZE_COMMAND_SPECIALIZATION( ResizeMT_NN_SSE_RGBA8 )

ULIS_NAMESPACE_END

