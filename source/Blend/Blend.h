// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Blend.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Blend API.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/Conv.h"
#include "Dispatch/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"
#include <vectorclass.h>

// Include MEM Generic Implementation
#include "Blend/Generic/AlphaBlendMT_MEM_Generic.h"
#include "Blend/Generic/BlendMT_Separable_MEM_Generic.h"
#include "Blend/Generic/BlendMT_NonSeparable_MEM_Generic.h"
#include "Blend/Generic/BlendMT_Misc_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_Separable_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_NonSeparable_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_Misc_MEM_Generic.h"

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
        , color( iColor )
        , src_bps( iSrcBps )
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
    const FBlock* const color;
};


/////////////////////////////////////////////////////
// FBlendJobArgs
class FBlendJobArgs final
    : public IJobArgs
{
public:

    ~FBlendJobArgs() override {};
    FBlendJobArgs(
          const uint8* iSrc
        , uint8* const iBdp
        , const uint32 iLine
        
    )
        : IJobArgs()
        , src( iSrc )
        , bdp( iBdp )
        , line( iLine )
    {}

    const uint8* ULIS_RESTRICT src;
    uint8* ULIS_RESTRICT bdp;
    uint32 line;

    static
    void
    BuildJob_Scanlines(
          const FBlendCommandArgs* iCargs
        , const int64 iNumJobs
        , const int64 iNumTasksPerJob
        , const int64 iIndex
        , FBlendJobArgs& oJargs
    )
    {
        const FFormatMetrics& fmt               = iCargs->src.FormatMetrics();
        const uint8* const ULIS_RESTRICT src    = iCargs->src.Bits() + iCargs->srcRect.x * fmt.BPP;
        uint8* const ULIS_RESTRICT dst          = iCargs->dst.Bits() + iCargs->dstRect.x * fmt.BPP;
        const int64 src_bps                     = static_cast< int64 >( iCargs->src.BytesPerScanLine() );
        const int64 dst_bps                     = static_cast< int64 >( iCargs->dst.BytesPerScanLine() );
        const int64 size                        = iCargs->dstRect.w * fmt.BPP;
        oJargs.src                              = src + ( iCargs->srcRect.y + iIndex ) * src_bps;
        oJargs.dst                              = dst + ( iCargs->dstRect.y + iIndex ) * dst_bps;
        oJargs.size                             = size;
    }

    void
    Build_Chunks(
          const FBlendCommandArgs* iCargs
        , const int64 iSize
        , const int64 iCount
        , const int64 iOffset
        , const int64 iIndex
        , FBlendJobArgs& oJargs
    )
    {
        const uint8* const ULIS_RESTRICT src    = iCargs->src.Bits();
        uint8* const ULIS_RESTRICT dst          = iCargs->dst.Bits();
        const int64 btt                         = static_cast< int64 >( iCargs->src.BytesTotal() );
        oJargs.src                              = src + iIndex;
        oJargs.dst                              = dst + iIndex;
        oJargs.size                             = FMath::Min( iOffset + iSize, btt ) - iOffset;
    }

};

/////////////////////////////////////////////////////
// Schedulers
#define ULIS_DEFINE_BLEND_COMMAND_GENERIC( iName )          \
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(  \
    Schedule ## iName                                       \
    , FBlendJobArgs                                         \
    , FBlendCommandArgs                                     \
    , &Invoke ## iName ## < T >                             \
    , FBlendJobArgs::BuildJob_Scanlines                     \
    , FBlendJobArgs::Build_Chunks                           \
)
ULIS_DEFINE_BLEND_COMMAND_GENERIC( AlphaBlendMT_Separable_MEM_Generic_Subpixel )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( AlphaBlendMT_Separable_MEM_Generic )

/////////////////////////////////////////////////////
// Dispatchers
ULIS_DECLARE_DISPATCHER( FDispatchedBlendSeparableInvocationSchedulerSelector               )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendNonSeparableInvocationSchedulerSelector            )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendMiscInvocationSchedulerSelector                    )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector       )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector    )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector            )
ULIS_DECLARE_DISPATCHER( FDispatchedAlphaBlendSeparableInvocationSchedulerSelector          )
ULIS_DECLARE_DISPATCHER( FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector  )
ULIS_DECLARE_DISPATCHER( FDispatchedTiledBlendSeparableInvocationSchedulerSelector          )
ULIS_DECLARE_DISPATCHER( FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector       )
ULIS_DECLARE_DISPATCHER( FDispatchedTiledBlendMiscInvocationSchedulerSelector               )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendSeparableInvocationSchedulerSelector,                &ScheduleBlendMT_Separable_MEM_Generic< T >                 )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendNonSeparableInvocationSchedulerSelector,             &ScheduleBlendMT_NonSeparable_MEM_Generic< T >              )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendMiscInvocationSchedulerSelector,                     &ScheduleBlendMT_Misc_MEM_Generic< T >                      )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector,        &ScheduleBlendMT_Separable_MEM_Generic_Subpixel< T >        )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector,     &ScheduleBlendMT_NonSeparable_MEM_Generic_Subpixel< T >     )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector,             &ScheduleBlendMT_Misc_MEM_Generic_Subpixel< T >             )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedAlphaBlendSeparableInvocationSchedulerSelector,           &ScheduleAlphaBlendMT_Separable_MEM_Generic< T >            )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector,   &ScheduleAlphaBlendMT_Separable_MEM_Generic_Subpixel< T >   )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTiledBlendSeparableInvocationSchedulerSelector,           &ScheduleTiledBlendMT_Separable_MEM_Generic< T >            )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector,        &ScheduleTiledBlendMT_NonSeparable_MEM_Generic< T >         )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTiledBlendMiscInvocationSchedulerSelector,                &ScheduleTiledBlendMT_Misc_MEM_Generic< T >                 )

ULIS_NAMESPACE_END

