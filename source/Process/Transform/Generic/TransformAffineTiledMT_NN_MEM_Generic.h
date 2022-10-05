// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformAffineTiledMT_NN_MEM_Generic.h
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
InvokeTransformAffineTiledMT_NN_MEM_Generic(
      const FTransformJobArgs* jargs
    , const FTransformCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;

    FVec3F point_in_dst( cargs->dstRect.x, cargs->dstRect.y + jargs->line, 1.f );
    FVec2F point_in_src( cargs->inverseMatrix * point_in_dst );
    FVec2F src_dx( cargs->inverseMatrix * FVec3F( 1.f, 0.f, 0.f ) );

    //const int minx = cargs->srcRect.x;
    //const int miny = cargs->srcRect.y;
    //const int maxx = minx + cargs->srcRect.w;
    //const int maxy = miny + cargs->srcRect.h;
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        int src_x = FMath::PyModulo( static_cast< int >( point_in_src.x ), cargs->srcRect.w );
        int src_y = FMath::PyModulo( static_cast< int >( point_in_src.y ), cargs->srcRect.h );
        memcpy( dst, cargs->src.PixelBits( src_x, src_y ), fmt.BPP );

        dst += fmt.BPP;
        point_in_src += src_dx;
    }
}

ULIS_DEFINE_TRANSFORM_COMMAND_GENERIC( TransformAffineTiledMT_NN_MEM_Generic )

ULIS_NAMESPACE_END

