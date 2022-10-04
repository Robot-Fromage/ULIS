// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformBezier_NN_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @license      Please refer to LICENSE.md
*/
#include "Core/Core.h"
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
#include "Process/Transform/RGBA8/TransformBezier_NN_SSE_RGBA8.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeTransformBezierMT_NN_SSE_RGBA8(
      const FTransformJobArgs* jargs
    , const FTransformBezierCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;
    const float*            field   = reinterpret_cast< const float* >( cargs->field.ScanlineBits( jargs->line ) );
    const uint8*            mask    = reinterpret_cast< const uint8* >( cargs->mask.ScanlineBits( jargs->line ) );
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

ULIS_DEFINE_BEZIER_COMMAND_SPECIALIZATION( TransformBezierMT_NN_SSE_RGBA8 )

ULIS_NAMESPACE_END
#endif // ULIS_COMPILETIME_SSE_SUPPORT

