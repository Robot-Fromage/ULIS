// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformAffineTiledMT_Bilinear_MEM_Generic.h
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
InvokeTransformAffineTiledMT_Bilinear_MEM_Generic(
      const FTransformJobArgs* jargs
    , const FTransformCommandArgs* cargs
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    uint8* ULIS_RESTRICT dst = jargs->dst;

    FVec3F point_in_dst( cargs->dstRect.x, cargs->dstRect.y + jargs->line, 1.f );
    FVec2F point_in_src( cargs->inverseMatrix * point_in_dst );
    FVec2F src_dx( cargs->inverseMatrix * FVec3F( 1.f, 0.f, 0.f ) );
    uint8* c00 = new uint8[ fmt.BPP * 4 ];
    uint8* c10 = c00 + fmt.BPP;
    uint8* c11 = c10 + fmt.BPP;
    uint8* c01 = c11 + fmt.BPP;
    uint8* hh0 = new uint8[ fmt.BPP * 2 ];
    uint8* hh1 = hh0 + fmt.BPP;

    //const int minx = cargs->srcRect.x;
    //const int miny = cargs->srcRect.y;
    //const int maxx = minx + cargs->srcRect.w;
    //const int maxy = miny + cargs->srcRect.h;
    for( int x = 0; x < cargs->dstRect.w; ++x ) {
        const float modx = FMath::PyModulo( point_in_src.x, static_cast< float >( cargs->srcRect.w ) );
        const float mody = FMath::PyModulo( point_in_src.y, static_cast< float >( cargs->srcRect.h ) );
        const int   left    = static_cast< int >( modx );
        const int   top     = static_cast< int >( mody );
        const int   right   = ( left + 1 ) % cargs->srcRect.w;
        const int   bot     = ( top  + 1 ) % cargs->srcRect.h;
        const float tx      = modx - left;
        const float ux      = 1.f - tx;
        const float ty      = mody - top;
        const float uy      = 1.f - ty;

        #define TEMP( _C, _X, _Y ) { memcpy( _C, cargs->src.PixelBits( _X, _Y ), fmt.BPP ); }
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

ULIS_DEFINE_TRANSFORM_COMMAND_GENERIC( TransformAffineTiledMT_Bilinear_MEM_Generic )

ULIS_NAMESPACE_END

