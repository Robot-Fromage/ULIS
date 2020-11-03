// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         TransformPerspectiveMT_Bilinear_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic transform entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Maths/Geometry.h"
#include "Transform/Dispatch/TransformInfo.h"
#include "Transform/Dispatch/Samplers.ipp"

ULIS2_NAMESPACE_BEGIN
template< typename T > void
InvokeTransformPerspectiveMTProcessScanline_Bilinear_MEM_Generic( tByte* iDst, int32 iLine, std::shared_ptr< const _FTransformInfoPrivate > iInfo ) {
    const _FTransformInfoPrivate&   info    = *iInfo;
    const FFormatInfo&              fmt     = info.destination->FormatInfo();
    tByte*                          dst     = iDst;

    FVec2F pointInDst( static_cast< float >( info.dst_roi.x ), static_cast< float >( info.dst_roi.y + iLine ) );
    tByte* c00 = new tByte[ fmt.SPP * 4 ];
    tByte* c10 = c00 + fmt.SPP;
    tByte* c11 = c10 + fmt.SPP;
    tByte* c01 = c11 + fmt.SPP;
    tByte* hh0 = new tByte[ fmt.SPP * 2 ];
    tByte* hh1 = hh0 + fmt.SPP;

    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        FVec2F pointInSrc = HomographyTransform( pointInDst, info.inverseTransform );
        const int   left    = static_cast< int >( floor( pointInSrc.x ) );
        const int   top     = static_cast< int >( floor( pointInSrc.y ) );
        const int   right   = left + 1;
        const int   bot     = top + 1;
        const float tx      = pointInSrc.x - left;
        const float ux      = 1.f - tx;
        const float ty      = pointInSrc.y - top;
        const float uy      = 1.f - ty;

        #define TEMP( _C, _X, _Y ) if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) { memcpy( _C, info.source->PixelPtr( _X, _Y ), fmt.BPP ); } else { memset( _C, 0, fmt.BPP ); }
        TEMP( c00, left, top );
        TEMP( c10, right, top );
        TEMP( c11, right, bot );
        TEMP( c01, left, bot );
        #undef TEMP
        SampleBilinear< T >( hh0, c00, c10, fmt, tx, ux );
        SampleBilinear< T >( hh1, c01, c11, fmt, tx, ux );
        SampleBilinear< T >( dst, hh0, hh1, fmt, ty, uy );

        dst += fmt.BPP;
        pointInDst.x += 1;
    }

    delete [] c00;
    delete [] hh0;
}

template< typename T > void
TransformPerspectiveMT_Bilinear_MEM_Generic( std::shared_ptr< const _FTransformInfoPrivate > iInfo ) {
    const _FTransformInfoPrivate&   info        = *iInfo;
    tByte*                          dst         = info.destination->DataPtr();
    const tSize                     dst_bps     = info.destination->BytesPerScanLine();
    const tSize                     dst_decal_y = info.dst_roi.y;
    const tSize                     dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    ULIS2_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeTransformPerspectiveMTProcessScanline_Bilinear_MEM_Generic< T >
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo );
}


ULIS2_NAMESPACE_END

