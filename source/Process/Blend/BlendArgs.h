// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BlendArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for BlendArgs.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Conv/Conv.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FBlendCommandArgs
class FBlendCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FBlendCommandArgs() override
    {
        if( color )
            delete color;
    };

    FBlendCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , const FVec2F& iSubpixelComponent
        , const FVec2F& iBuspixelComponent
        , const eBlendMode iBlendingMode
        , const eAlphaMode iAlphaMode
        , const ufloat iOpacity
        , const FVec2I& iShift
        , const FVec2I& iBackdropCoverage
        , const fpConvertFormat iFwd
        , const fpConvertFormat iBkd
        , const Vec4i iIDT
        , const uint32 iSrcBps
        , bool iTiled = false
        , const FBlock* iColor = nullptr
    )
        : FDualBufferCommandArgs(
              iSrc
            , iDst
            , iSrcRect
            , iDstRect
            )
        , subpixelComponent( iSubpixelComponent )
        , buspixelComponent( iBuspixelComponent )
        , blendingMode( iBlendingMode )
        , alphaMode( iAlphaMode )
        , opacity( iOpacity )
        , shift( iShift )
        , backdropCoverage( iBackdropCoverage )
        , fwd( iFwd )
        , bkd( iBkd )
        , idt( iIDT )
        , src_bps( iSrcBps )
        , tiled( iTiled )
        , color( iColor )
        {}

    const FVec2F subpixelComponent;
    const FVec2F buspixelComponent;
    const eBlendMode blendingMode;
    const eAlphaMode alphaMode;
    const ufloat opacity;
    const FVec2I shift;
    const FVec2I backdropCoverage;
    const fpConvertFormat fwd;
    const fpConvertFormat bkd;
    const Vec4i idt;
    const uint32 src_bps;
    bool tiled;
    const FBlock* const color;
};


/////////////////////////////////////////////////////
// FBlendJobArgs
class FBlendJobArgs final
    : public IJobArgs
{
public:

    ~FBlendJobArgs() override {};

    const uint8* ULIS_RESTRICT src;
    uint8* ULIS_RESTRICT bdp;
    uint32 line;
};

/////////////////////////////////////////////////////
// Builders
static
const uint8*
ComputeBufferPosition(
      const uint8* iBase
    , const int iSourceRectY
    , const int iShiftY
    , const int iSourceRectH
    , const uint32 iSrcBps
    , const uint32 iSrcDecalX
    , const uint32 iSrcDecalY
    , const int iLine
    , const bool iTiled
)
{
    if( iTiled ) {
        return  iBase + ( ( iSourceRectY + ( ( iShiftY + iLine ) % iSourceRectH ) ) * iSrcBps ) + iSrcDecalX;
    } else {
        return  iBase + ( ( iSrcDecalY + iLine ) * iSrcBps ) + iSrcDecalX;
    }
}


static
void
BuildBlendJob_Scanlines(
      const FBlendCommandArgs* iCargs
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
    , const int64 iIndex
    , FBlendJobArgs& oJargs
)
{
    const FFormatMetrics& fmt               = iCargs->src.FormatMetrics();
    const uint8* const ULIS_RESTRICT src    = iCargs->src.Bits();
    uint8* const ULIS_RESTRICT dst          = iCargs->dst.Bits();
    const uint32 src_bps                     = static_cast< uint32 >( iCargs->src.BytesPerScanLine() );
    const uint32 dst_bps                     = static_cast< uint32 >( iCargs->dst.BytesPerScanLine() );
    const int64 size                        = iCargs->dstRect.w * fmt.BPP;
    const uint32 src_decal_y                = iCargs->shift.y + iCargs->srcRect.y;
    const uint32 src_decal_x                = ( iCargs->shift.x + iCargs->srcRect.x ) * iCargs->src.BytesPerPixel();
    const uint32 bdp_decal_x                = iCargs->dstRect.x * fmt.BPP;
    oJargs.src                              = ComputeBufferPosition( src, iCargs->srcRect.y, iCargs->shift.y, iCargs->srcRect.h, src_bps, src_decal_x, src_decal_y, int(iIndex), iCargs->tiled );
    oJargs.bdp                              = dst + ( ( iCargs->dstRect.y + iIndex ) * dst_bps ) + bdp_decal_x;
    oJargs.line                             = static_cast< uint32 >( iIndex );
}

static
void
BuildBlendJob_Chunks(
      const FBlendCommandArgs* iCargs
    , const int64 iSize
    , const int64 iCount
    , const int64 iOffset
    , const int64 iIndex
    , FBlendJobArgs& oJargs
)
{
    ULIS_ASSERT( false, "Chunk Scheduling not available for Blend" );
    /*
    const uint8* const ULIS_RESTRICT src    = iCargs->src.Bits();
    uint8* const ULIS_RESTRICT dst          = iCargs->dst.Bits();
    const int64 btt                         = static_cast< int64 >( iCargs->src.BytesTotal() );
    oJargs.src                              = src + iIndex;
    oJargs.bdp                              = dst + iIndex;
    oJargs.line                             = FMath::Min( iOffset + iSize, btt ) - iOffset;
    */
}

/////////////////////////////////////////////////////
// Macro Helpers for Redundant Compositing Operations
#define SampleSubpixelChannel( _DST, _CHAN )                                                                                                \
    s11 = ( notLastCol  && notLastLine )                                    ?   TYPE2FLOAT( src,                            _CHAN ) : 0.f;  \
    s01 = ( notLastLine && ( x > 0 || hasLeftData ) )                       ?   TYPE2FLOAT( src - fmt.BPP,                  _CHAN ) : 0.f;  \
    s10 = ( notLastCol && ( notFirstLine || hasTopData ) )                  ?   TYPE2FLOAT( src - cargs->src_bps,           _CHAN ) : 0.f;  \
    s00 = ( ( x > 0 || hasLeftData ) && ( notFirstLine || hasTopData ) )    ?   TYPE2FLOAT( src - cargs->src_bps - fmt.BPP, _CHAN ) : 0.f;  \
    v1 = ( s00 * m00 ) * cargs->subpixelComponent.y + ( s01 * m01 ) * cargs->buspixelComponent.y;                                           \
    v2 = ( s10 * m10 ) * cargs->subpixelComponent.y + ( s11 * m11 ) * cargs->buspixelComponent.y;                                           \
    _DST = res == 0.f ? 0.f : ( ( v1 ) * cargs->subpixelComponent.x + ( v2 ) * cargs->buspixelComponent.x ) / res;

/////////////////////////////////////////////////////
// Schedulers
#define ULIS_DEFINE_BLEND_COMMAND_GENERIC( iName )          \
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(  \
    Schedule ## iName                                       \
    , FBlendJobArgs                                         \
    , FBlendCommandArgs                                     \
    , &Invoke ## iName ## < T >                             \
    , &BuildBlendJob_Scanlines                              \
    , &BuildBlendJob_Chunks                                 \
)
#define ULIS_DEFINE_BLEND_COMMAND_SPECIALIZATION( iName )   \
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(          \
    Schedule ## iName                                       \
    , FBlendJobArgs                                         \
    , FBlendCommandArgs                                     \
    , &Invoke ## iName                                      \
    , &BuildBlendJob_Scanlines                              \
    , &BuildBlendJob_Chunks                                 \
)

ULIS_NAMESPACE_END

