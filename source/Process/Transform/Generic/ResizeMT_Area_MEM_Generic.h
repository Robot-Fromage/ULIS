// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ResizeMT_Area_MEM_Generic.h
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
InvokeResizeMT_Area_MEM_Generic( uint8* iDst, int32 iLine, std::shared_ptr< const FResizeArgs > iInfo ) {
    const FResizeArgs&  info    = *iInfo;
    const FFormatMetrics&  fmt     = info.destination->FormatMetrics();
    const FFormatMetrics&  sat_fmt = info.optionalSAT->FormatMetrics();
    uint8*              dst     = iDst;

    FVec2F point_in_dst( info.dst_roi.x, info.dst_roi.y + iLine );
    FVec2F point_in_src( info.inverseScale * ( point_in_dst - info.shift ) + FVec2F( info.src_roi.x, info.src_roi.y ) );
    FVec2F src_dx( info.inverseScale * FVec2F( 1.f, 0.f ) );
    FVec2F coverage( FVec2F( 1.f, 1.f ) * info.inverseScale );
    float  coverage_area = coverage.x * coverage.y;

    const int minx = info.src_roi.x;
    const int miny = info.src_roi.y;
    const int maxx = minx + info.src_roi.w;
    const int maxy = miny + info.src_roi.h;

    uint8* c00 = new uint8[ sat_fmt.BPP * 4 ];
    uint8* c10 = c00 + sat_fmt.BPP;
    uint8* c11 = c10 + sat_fmt.BPP;
    uint8* c01 = c11 + sat_fmt.BPP;
    uint8* hh0 = new uint8[ sat_fmt.BPP * 2 ];
    uint8* hh1 = hh0 + sat_fmt.BPP;

    uint8* m00 = new uint8[ sat_fmt.BPP * 4 ];
    uint8* m10 = m00 + sat_fmt.BPP;
    uint8* m11 = m10 + sat_fmt.BPP;
    uint8* m01 = m11 + sat_fmt.BPP;

    float fpos[4];
    int   ipos[4];
    float t[4];
    float u[4];

    for( int x = 0; x < info.dst_roi.w; ++x ) {
        // order: left top right bot
        fpos[0] = point_in_src.x - 1.f;
        fpos[1] = point_in_src.y - 1.f;
        fpos[2] = fpos[0] + coverage.x;
        fpos[3] = fpos[1]  + coverage.y;
        for( int i = 0; i < 4; ++i ) {
            ipos[i] = static_cast< int >( fpos[i] );
            t[i]    = FMath::FloatingPartOfNumber( fpos[i] );
            u[i]    = 1.f - t[i];
        }

        #define SUBSAMPLE_CORNER_IMP( _C, _X, _Y ) if( _X >= minx && _Y >= miny && _X < maxx && _Y < maxy ) { memcpy( _C, info.optionalSAT->PixelBits( _X, _Y ), sat_fmt.BPP ); } else { memset( _C, 0, sat_fmt.BPP ); }
        #define SUBSAMPLE_CORNER( _P0, _P1, _M )                                                            \
            SUBSAMPLE_CORNER_IMP( c00, ipos[ _P0 ],     ipos[ _P1 ]     );                                  \
            SUBSAMPLE_CORNER_IMP( c10, ipos[ _P0 ] + 1, ipos[ _P1 ]     );                                  \
            SUBSAMPLE_CORNER_IMP( c11, ipos[ _P0 ] + 1, ipos[ _P1 ] + 1 );                                  \
            SUBSAMPLE_CORNER_IMP( c01, ipos[ _P0 ],     ipos[ _P1 ] + 1 );                                  \
            SampleBilinearSAT< float >( (uint8*)hh0, (uint8*)c00, (uint8*)c10, sat_fmt, t[ _P0 ], u[ _P0 ] );  \
            SampleBilinearSAT< float >( (uint8*)hh1, (uint8*)c01, (uint8*)c11, sat_fmt, t[ _P0 ], u[ _P0 ] );  \
            SampleBilinearSAT< float >( (uint8*)_M, (uint8*)hh0, (uint8*)hh1, sat_fmt, t[ _P1 ], u[ _P1 ] );
        SUBSAMPLE_CORNER( 0, 1, m00 )
        SUBSAMPLE_CORNER( 2, 1, m10 )
        SUBSAMPLE_CORNER( 2, 3, m11 )
        SUBSAMPLE_CORNER( 0, 3, m01 )
        #undef SUBSAMPLE_CORNER_IMP
        #undef SUBSAMPLE_CORNER

        float alpha = FMath::Clamp( ( ((float*)m11)[sat_fmt.AID] + ((float*)m00)[sat_fmt.AID] - ((float*)m10)[sat_fmt.AID] - ((float*)m01)[sat_fmt.AID] ) / coverage_area, static_cast< float >( MinType< T >() ), static_cast< float >( MaxType< T >() ) );
        *( reinterpret_cast< T* >( dst ) + sat_fmt.AID ) = static_cast< T >( alpha );
        for( int i = 0; i < sat_fmt.NCC; ++i ) {
            uint8 r = sat_fmt.IDT[i];
            *( reinterpret_cast< T* >( dst ) + r ) = static_cast< T >( FMath::Clamp( ( ((float*)m11)[r] + ((float*)m00)[r] - ((float*)m10)[r] - ((float*)m01)[r] ) / coverage_area, static_cast< float >( MinType< T >() ), static_cast< float >( MaxType< T >() ) ) / alpha * static_cast< float >( MaxType< T >() ) );
        }

        dst += fmt.BPP;
        point_in_src += src_dx;
    }

    delete [] c00;
    delete [] hh0;
    delete [] m00;
}

template< typename T > void
ResizeMT_Area_MEM_Generic( std::shared_ptr< const FResizeArgs > iInfo ) {
    const FResizeArgs&  info        = *iInfo;
    uint8*              dst         = info.destination->Bits();
    const uint32         dst_bps     = info.destination->BytesPerScanLine();
    const uint32         dst_decal_y = info.dst_roi.y;
    const uint32         dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeResizeMT_Area_MEM_Generic< T >
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo );
}

ULIS_NAMESPACE_END

