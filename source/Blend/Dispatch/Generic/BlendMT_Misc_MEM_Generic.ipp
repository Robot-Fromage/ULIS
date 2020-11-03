// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         BlendMT_Misc_MEM_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic Blend entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Base/PRNG.h"
#include "Blend/Dispatch/BlendInfo.h"
#include "Blend/Func/AlphaFuncF.ipp"
#include "Blend/Modes.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"

float gBayer8x8Matrix[8][8] = {
    { 0.015625f, 0.765625f, 0.203125f, 0.953125f, 0.0625f, 0.8125f, 0.25f, 1.0f          },
    { 0.515625f, 0.265625f, 0.703125f, 0.453125f, 0.5625f, 0.3125f, 0.75f, 0.5f          },
    { 0.140625f, 0.890625f, 0.078125f, 0.828125f, 0.1875f, 0.9375f, 0.125f, 0.875f       },
    { 0.640625f, 0.390625f, 0.578125f, 0.328125f, 0.6875f, 0.4375f, 0.625f, 0.375f       },
    { 0.046875f, 0.796875f, 0.234375f, 0.984375f, 0.03125f, 0.78125f, 0.21875f, 0.96875f },
    { 0.546875f, 0.296875f, 0.734375f, 0.484375f, 0.53125f, 0.28125f, 0.71875f, 0.46875f },
    { 0.171875f, 0.921875f, 0.109375f, 0.859375f, 0.15625f, 0.90625f, 0.09375f, 0.84375f },
    { 0.671875f, 0.421875f, 0.609375f, 0.359375f, 0.65625f, 0.40625f, 0.59375f, 0.34375f } };

ULIS2_NAMESPACE_BEGIN
template< typename T >
void
InvokeBlendMTProcessScanline_Misc_MEM_Generic_Subpixel( const tByte* iSrc, tByte* iBdp, int32 iLine, const tSize iSrcBps, std::shared_ptr< const _FBlendInfoPrivate > iInfo ) {
    const _FBlendInfoPrivate&   info    = *iInfo;
    const FFormatInfo&          fmt     = info.source->FormatInfo();
    const tByte*                src     = iSrc;
    tByte*                      bdp     = iBdp;
    const bool notLastLine  = iLine < info.backdropCoverage.y;
    const bool notFirstLine = iLine > 0;
    const bool onLeftBorder = info.backdropWorkingRect.x == 0;
    const bool hasLeftData  = info.sourceRect.x + info.shift.x > 0;
    const bool hasTopData   = info.sourceRect.y + info.shift.y > 0;

    switch( info.blendingMode ) {
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case BM_DISSOLVE: {
            int32 seedy = info.backdropWorkingRect.y + iLine + 1;
            uint32 localPRNGSeed = ( 8253729 % seedy ) * GetWeakPRNGSeed() + ( 2396403 % ( seedy + 64578 ) * seedy );
            float m11, m01, m10, m00, vv0, vv1, res;
            m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - fmt.BPP,              fmt.AID ) : 0.f;
            m10 = ( hasLeftData && ( notFirstLine || hasTopData ) ) ? TYPE2FLOAT( src - iSrcBps - fmt.BPP,    fmt.AID ) : 0.f;
            vv1 = m10 * info.subpixelComponent.y + m11 * info.buspixelComponent.y;

            for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
                const bool notLastCol = x < info.backdropCoverage.x;
                m00 = m10;
                m01 = m11;
                vv0 = vv1;
                SampleSubpixelAlpha( res );
                const float alpha_bdp   = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const float alpha_src   = res * info.opacityValue;
                localPRNGSeed = 8253729 * localPRNGSeed + 2396403;
                float toss = ( localPRNGSeed % 65537 ) / 65537.f;
                if( toss < alpha_src ) {
                    float alpha_result;
                    ULIS2_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
            }
            break;
        } // BM_DISSOLVE
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case BM_BAYERDITHER8x8: {
            float m11, m01, m10, m00, vv0, vv1, res;
            m11 = ( notLastLine && onLeftBorder && hasLeftData )    ? TYPE2FLOAT( src - fmt.BPP,              fmt.AID ) : 0.f;
            m10 = ( hasLeftData && ( notFirstLine || hasTopData ) ) ? TYPE2FLOAT( src - iSrcBps - fmt.BPP,    fmt.AID ) : 0.f;
            vv1 = m10 * info.subpixelComponent.y + m11 * info.buspixelComponent.y;

            for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
                const bool notLastCol = x < info.backdropCoverage.x;
                m00 = m10;
                m01 = m11;
                vv0 = vv1;
                SampleSubpixelAlpha( res );
                const float alpha_bdp   = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const float alpha_src   = res * info.opacityValue;
                const tSize bayerX          = ( info.backdropWorkingRect.x + x )     % 8;
                const tSize bayerY          = ( info.backdropWorkingRect.y + iLine ) % 8;
                const float bayerEl = gBayer8x8Matrix[ bayerY ][ bayerX ];
                if( alpha_src >= bayerEl ) {
                    float alpha_result;
                    ULIS2_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
            }
            break;
        } // BM_DISSOLVE
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
}

template< typename T >
void
BlendMT_Misc_MEM_Generic_Subpixel( std::shared_ptr< const _FBlendInfoPrivate > iInfo ) {
    const _FBlendInfoPrivate&   info        = *iInfo;
    const tByte*                src         = info.source->DataPtr();
    tByte*                      bdp         = info.backdrop->DataPtr();
    const tSize                 src_bps     = info.source->BytesPerScanLine();
    const tSize                 bdp_bps     = info.backdrop->BytesPerScanLine();
    const tSize                 src_decal_y = info.shift.y + info.sourceRect.y;
    const tSize                 src_decal_x = ( info.shift.x + info.sourceRect.x )  * info.source->BytesPerPixel();
    const tSize                 bdp_decal_x = ( info.backdropWorkingRect.x )        * info.source->BytesPerPixel();
    ULIS2_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.backdropWorkingRect.h
                                   , InvokeBlendMTProcessScanline_Misc_MEM_Generic_Subpixel< T >
                                   , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , src_bps, iInfo );
}

template< typename T >
void
InvokeBlendMTProcessScanline_Misc_MEM_Generic( const tByte* iSrc, tByte* iBdp, int32 iLine, std::shared_ptr< const _FBlendInfoPrivate > iInfo ) {
    const _FBlendInfoPrivate&   info    = *iInfo;
    const FFormatInfo&          fmt     = info.source->FormatInfo();
    const tByte*                src     = iSrc;
    tByte*                      bdp     = iBdp;

    switch( info.blendingMode ) {
        case BM_DISSOLVE: {
            int32 seedy = info.backdropWorkingRect.y + iLine + 1;
            uint32 localPRNGSeed = ( 8253729 % seedy ) * GetWeakPRNGSeed() + ( 2396403 % ( seedy + 64578 ) * seedy );

            for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
                const float alpha_bdp   = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const float alpha_src   = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * info.opacityValue : info.opacityValue;
                localPRNGSeed = 8253729 * localPRNGSeed + 2396403;
                float toss = ( localPRNGSeed % 65537 ) / 65537.f;
                if( toss < alpha_src ) {
                    float alpha_result;
                    ULIS2_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
            }
            break;
        }

        case BM_BAYERDITHER8x8: {
            for( int x = 0; x < info.backdropWorkingRect.w; ++x ) {
                const float alpha_bdp   = fmt.HEA ? TYPE2FLOAT( bdp, fmt.AID ) : 1.f;
                const float alpha_src   = fmt.HEA ? TYPE2FLOAT( src, fmt.AID ) * info.opacityValue : info.opacityValue;
                const float bayerEl     = gBayer8x8Matrix[ ( info.backdropWorkingRect.y + iLine ) % 8 ][ ( info.backdropWorkingRect.x + x ) % 8 ];
                if( alpha_src >= bayerEl ) {
                    float alpha_result;
                    ULIS2_ASSIGN_ALPHAF( info.alphaMode, alpha_result, alpha_src, alpha_bdp );
                    memcpy( bdp, src, fmt.BPP );
                    if( fmt.HEA ) FLOAT2TYPE( bdp, fmt.AID, alpha_result );
                }
                src += fmt.BPP;
                bdp += fmt.BPP;
            }
            break;
        }
    }
}

template< typename T >
void
BlendMT_Misc_MEM_Generic( std::shared_ptr< const _FBlendInfoPrivate > iInfo ) {
    const _FBlendInfoPrivate&   info        = *iInfo;
    const tByte*                src         = info.source->DataPtr();
    tByte*                      bdp         = info.backdrop->DataPtr();
    const tSize                 src_bps     = info.source->BytesPerScanLine();
    const tSize                 bdp_bps     = info.backdrop->BytesPerScanLine();
    const tSize                 src_decal_y = info.shift.y + info.sourceRect.y;
    const tSize                 src_decal_x = ( info.shift.x + info.sourceRect.x )  * info.source->BytesPerPixel();
    const tSize                 bdp_decal_x = ( info.backdropWorkingRect.x )        * info.source->BytesPerPixel();
    ULIS2_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.backdropWorkingRect.h
                                   , InvokeBlendMTProcessScanline_Misc_MEM_Generic< T >
                                   , src + ( ( src_decal_y + pLINE )                * src_bps ) + src_decal_x
                                   , bdp + ( ( info.backdropWorkingRect.y + pLINE ) * bdp_bps ) + bdp_decal_x
                                   , pLINE , iInfo );
}

ULIS2_NAMESPACE_END

