// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         TiledBlendMT_Misc_MEM_Generic.h
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
#include "Blend/Func/AlphaFuncF.h"
#include "Blend/PRNG.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeTiledBlendMTProcessScanline_Misc_MEM_Generic( const uint8* iSrc, uint8* iBdp, int32 iLine, const FBlendCommandArgs* iArgs ) {
    const FBlendCommandArgs&   info    = *iInfo;
    const FFormat&  fmt     = info.source->FormatInfo();
    const uint8*        src     = iSrc + info.shift.x * fmt.BPP;
    uint8*              bdp     = iBdp;

    switch( info.blendingMode ) {
        case Blend_Dissolve: {
            int32 seedy = info.backdropWorkingRect.y + iLine + 1;
            uint32 localPRNGSeed = ( 8253729 % seedy ) * GetBlendPRNGSeed() + ( 2396403 % ( seedy + 64578 ) * seedy );

            for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
                const float alpha_bdp   = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const float alpha_src   = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * info.opacityValue : info.opacityValue;
                localPRNGSeed = 8253729 * localPRNGSeed + 2396403;
                float toss = ( localPRNGSeed % 65537 ) / 65537.f;
                if( toss < alpha_src ) {
                    float alpha_result;
                    ULIS_ASSIGN_ALPHAF( info.alphaMode, alpha_result, 1.f, alpha_bdp );
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
                if( ( x + info.shift.x ) % info.sourceRect.w == 0 )
                    src = iSrc;
            }
            break;
        }

        case Blend_BayerDither8x8: {
            for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
                const float alpha_bdp   = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const float alpha_src   = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * info.opacityValue : info.opacityValue;
                const float bayerEl     = gBayer8x8Matrix[ ( info.backdropWorkingRect.y + iLine ) % 8 ][ ( info.backdropWorkingRect.x + x ) % 8 ];
                if( alpha_src >= bayerEl ) {
                    float alpha_result;
                    ULIS_ASSIGN_ALPHAF( info.alphaMode, alpha_result, 1.f, alpha_bdp );
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
                if( ( x + info.shift.x ) % info.sourceRect.w == 0 )
                    src = iSrc;
            }
            break;
        }
    }
}

template< typename T >
void
ScheduleTiledBlendMT_Misc_MEM_Generic( const FBlendCommandArgs* iArgs, const FSchedulePolicy& iPolicy, FThreadPool& iPool ) {
    const FBlendCommandArgs&   info        = *iInfo;
    const uint8*        src         = info.source->Bits();
    uint8*              bdp         = info.backdrop->Bits();
    const uint32         src_bps     = info.source->BytesPerScanLine();
    const uint32         bdp_bps     = info.backdrop->BytesPerScanLine();
    const uint32         src_decal_y = info.shift.y + info.sourceRect.y;
    const uint32         src_decal_x = ( info.sourceRect.x )  * info.source->BytesPerPixel();
    const uint32         bdp_decal_x = ( info.backdropWorkingRect.x )        * info.source->BytesPerPixel();
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.backdropWorkingRect.h
                                   , InvokeTiledBlendMTProcessScanline_Misc_MEM_Generic< T >
                                   , src + ( ( info.sourceRect.y + ( ( info.shift.y + pLINE ) % info.sourceRect.h ) ) * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , iInfo );
}

ULIS_NAMESPACE_END

