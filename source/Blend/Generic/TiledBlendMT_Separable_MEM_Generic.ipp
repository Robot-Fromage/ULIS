// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         TiledBlendMT_Separable_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Blend specialization as described in the title.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/BlendArgs.h"
#include "Blend/BlendHelpers.h"
#include "Blend/Modes.h"
#include "Blend/Func/AlphaFuncF.ipp"
#include "Blend/Func/SeparableBlendFuncF.ipp"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN
template< typename T >
void
InvokeTiledBlendMTProcessScanline_Separable_MEM_Generic( const tByte* iSrc, tByte* iBdp, int32 iLine, std::shared_ptr< const FBlendArgs > iInfo ) {
    const FBlendArgs&   info    = *iInfo;
    const FFormatInfo&  fmt     = info.source->FormatInfo();
    const tByte*        src     = iSrc + info.shift.x * fmt.BPP;
    tByte*              bdp     = iBdp;

    for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
        const float alpha_src  = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * info.opacityValue : info.opacityValue;
        const float alpha_bdp  = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
        const float alpha_comp = AlphaNormalF( alpha_src, alpha_bdp );
        const float var        = alpha_comp == 0.f ? 0.f : alpha_src / alpha_comp;
        float alpha_result;
        ULIS3_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );
        for( uint8 j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            float srcvf = TYPE2FLOAT( src, r );
            float bdpvf = TYPE2FLOAT( bdp, r );
            #define TMP_ASSIGN( _BM, _E1, _E2, _E3 ) FLOAT2TYPE( bdp, r, SeparableCompOpF< _BM >( srcvf, bdpvf, alpha_bdp, var ) );
            ULIS3_SWITCH_FOR_ALL_DO( info.blendingMode, ULIS3_FOR_ALL_SEPARABLE_BM_DO, TMP_ASSIGN, 0, 0, 0 )
            #undef TMP_ASSIGN
        }
        if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
        src += fmt.BPP;
        bdp += fmt.BPP;

        if( ( x + info.shift.x ) % info.sourceRect.w == 0 )
            src = iSrc;
    }
}

template< typename T >
void
TiledBlendMT_Separable_MEM_Generic( std::shared_ptr< const FBlendArgs > iInfo ) {
    const FBlendArgs&   info        = *iInfo;
    const tByte*        src         = info.source->DataPtr();
    tByte*              bdp         = info.backdrop->DataPtr();
    const tSize         src_bps     = info.source->BytesPerScanLine();
    const tSize         bdp_bps     = info.backdrop->BytesPerScanLine();
    const tSize         src_decal_y = info.shift.y + info.sourceRect.y;
    const tSize         src_decal_x = ( info.sourceRect.x )  * info.source->BytesPerPixel();
    const tSize         bdp_decal_x = ( info.backdropWorkingRect.x )        * info.source->BytesPerPixel();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.backdropWorkingRect.h
                                   , InvokeTiledBlendMTProcessScanline_Separable_MEM_Generic< T >
                                   , src + ( ( info.sourceRect.y + ( ( info.shift.y + pLINE ) % info.sourceRect.h ) ) * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , iInfo );
}

ULIS3_NAMESPACE_END

