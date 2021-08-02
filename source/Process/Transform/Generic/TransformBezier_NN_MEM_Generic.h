// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformBezier_NN_MEM_Generic.h
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
InvokeTransformBezierMT_NN_MEM_Generic(
      const FTransformJobArgs* jargs
    , const FTransformBezierCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;
    const float* field = reinterpret_cast< const float* >( cargs->field.ScanlineBits( jargs->line ) );
    const uint8* mask  = reinterpret_cast< const uint8* >( cargs->mask.ScanlineBits( jargs->line ) );
    const int rangex = cargs->srcRect.w - 1;
    const int rangey = cargs->srcRect.h - 1;
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        if( *mask & 0xFF ) {
            int src_x = static_cast< int >( field[0] * rangex );
            int src_y = static_cast< int >( field[1] * rangey );
            memcpy( dst, cargs->src.PixelBits( src_x, src_y ), fmt.BPP );
        }

        dst += fmt.BPP;
        field += 2;
        ++mask;
    }
}

ULIS_DEFINE_BEZIER_COMMAND_GENERIC( TransformBezierMT_NN_MEM_Generic )

ULIS_NAMESPACE_END

