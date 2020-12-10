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
    BuildJob_Scanlines(
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

    void
    BuildJob_Chunks(
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
};

/////////////////////////////////////////////////////
// Macro Helpers for Redundant Compositing Operations
// macros for ASSIGN and small helper functions, possibly inline.
#define SampleSubpixelAlpha( _DST )                                                                                     \
    if( fmt.HEA ) {                                                                                                     \
        m11 = ( notLastCol && notLastLine )                     ? TYPE2FLOAT( src,                  fmt.AID ) : 0.f;    \
        m10 = ( notLastCol && ( notFirstLine || hasTopData ) )  ? TYPE2FLOAT( src - cargs->src_bps, fmt.AID ) : 0.f;    \
    } else {                                                                                                            \
        m11 = ( notLastCol && notLastLine )     ? 1.f : 0.f;                                                            \
        m10 = ( notLastCol && notFirstLine )    ? 1.f : 0.f;                                                            \
    }                                                                                                                   \
    vv1     = m10 * cargs->subpixelComponent.y + m11 * cargs->buspixelComponent.y;                                      \
    _DST    = vv0 * cargs->subpixelComponent.x + vv1 * cargs->buspixelComponent.x;

#define SampleSubpixelChannel( _DST, _CHAN )                                                                                                \
    s11 = ( notLastCol  && notLastLine )                                    ?   TYPE2FLOAT( src,                            _CHAN ) : 0.f;  \
    s01 = ( notLastLine && ( x > 0 || hasLeftData ) )                       ?   TYPE2FLOAT( src - fmt.BPP,                  _CHAN ) : 0.f;  \
    s10 = ( notLastCol && ( notFirstLine || hasTopData ) )                  ?   TYPE2FLOAT( src - cargs->src_bps,           _CHAN ) : 0.f;  \
    s00 = ( ( x > 0 || hasLeftData ) && ( notFirstLine || hasTopData ) )    ?   TYPE2FLOAT( src - cargs->src_bps - fmt.BPP, _CHAN ) : 0.f;  \
    v1 = ( s00 * m00 ) * cargs->subpixelComponent.y + ( s01 * m01 ) * cargs->buspixelComponent.y;                                           \
    v2 = ( s10 * m10 ) * cargs->subpixelComponent.y + ( s11 * m11 ) * cargs->buspixelComponent.y;                                           \
    _DST = res == 0.f ? 0.f : ( ( v1 ) * cargs->subpixelComponent.x + ( v2 ) * cargs->buspixelComponent.x ) / res;

#define ULIS_ACTION_ASSIGN_ALPHAF( _AM, iTarget, iSrc, iBdp )      iTarget = AlphaF< _AM >( iSrc, iBdp );
#define ULIS_ACTION_ASSIGN_ALPHASSEF( _AM, iTarget, iSrc, iBdp )   iTarget = AlphaSSEF< _AM >( iSrc, iBdp );
#define ULIS_ACTION_ASSIGN_ALPHAAVXF( _AM, iTarget, iSrc, iBdp )   iTarget = AlphaAVXF< _AM >( iSrc, iBdp );
#define ULIS_ASSIGN_ALPHAF( iAlphaMode, iTarget, iSrc, iBdp )      ULIS_SWITCH_FOR_ALL_DO( iAlphaMode, ULIS_FOR_ALL_AM_DO, ULIS_ACTION_ASSIGN_ALPHAF, iTarget, iSrc, iBdp )
#define ULIS_ASSIGN_ALPHASSEF( iAlphaMode, iTarget, iSrc, iBdp )   ULIS_SWITCH_FOR_ALL_DO( iAlphaMode, ULIS_FOR_ALL_AM_DO, ULIS_ACTION_ASSIGN_ALPHASSEF, iTarget, iSrc, iBdp )
#define ULIS_ASSIGN_ALPHAAVXF( iAlphaMode, iTarget, iSrc, iBdp )   ULIS_SWITCH_FOR_ALL_DO( iAlphaMode, ULIS_FOR_ALL_AM_DO, ULIS_ACTION_ASSIGN_ALPHAAVXF, iTarget, iSrc, iBdp )

// Include MEM Generic Implementation
#include "Blend/Generic/AlphaBlendMT_MEM_Generic.h"
#include "Blend/Generic/BlendMT_Separable_MEM_Generic.h"
#include "Blend/Generic/BlendMT_NonSeparable_MEM_Generic.h"
#include "Blend/Generic/BlendMT_Misc_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_Separable_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_NonSeparable_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_Misc_MEM_Generic.h"

/////////////////////////////////////////////////////
// Schedulers
#define ULIS_DEFINE_BLEND_COMMAND_GENERIC( iName )          \
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(  \
    Schedule ## iName                                       \
    , FBlendJobArgs                                         \
    , FBlendCommandArgs                                     \
    , &Invoke ## iName ## < T >                             \
    , &FBlendJobArgs::BuildJob_Scanlines                    \
    , &FBlendJobArgs::BuildJob_Chunks                       \
)
#define ULIS_DEFINE_BLEND_COMMAND_SPECIALIZATION( iName )   \
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(          \
    Schedule ## iName                                       \
    , FBlendJobArgs                                         \
    , FBlendCommandArgs                                     \
    , &Invoke ## iName                                      \
    , &FBlendJobArgs::BuildJob_Scanlines                    \
    , &FBlendJobArgs::BuildJob_Chunks                       \
)

ULIS_DEFINE_BLEND_COMMAND_GENERIC( AlphaBlendMT_Separable_MEM_Generic_Subpixel  )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( AlphaBlendMT_Separable_MEM_Generic           )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( BlendMT_NonSeparable_MEM_Generic_Subpixel    )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( BlendMT_NonSeparable_MEM_Generic             )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( BlendMT_Separable_MEM_Generic_Subpixel       )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( BlendMT_Separable_MEM_Generic                )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( BlendMT_Misc_MEM_Generic_Subpixel            )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( BlendMT_Misc_MEM_Generic                     )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( TiledBlendMT_NonSeparable_MEM_Generic        )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( TiledBlendMT_Separable_MEM_Generic           )
ULIS_DEFINE_BLEND_COMMAND_GENERIC( TiledBlendMT_Misc_MEM_Generic                )

/////////////////////////////////////////////////////
// Dispatchers
ULIS_DECLARE_DISPATCHER( FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector  )
ULIS_DECLARE_DISPATCHER( FDispatchedAlphaBlendSeparableInvocationSchedulerSelector          )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector    )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendNonSeparableInvocationSchedulerSelector            )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector       )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendSeparableInvocationSchedulerSelector               )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector            )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendMiscInvocationSchedulerSelector                    )
ULIS_DECLARE_DISPATCHER( FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector       )
ULIS_DECLARE_DISPATCHER( FDispatchedTiledBlendSeparableInvocationSchedulerSelector          )
ULIS_DECLARE_DISPATCHER( FDispatchedTiledBlendMiscInvocationSchedulerSelector               )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector,   &ScheduleAlphaBlendMT_Separable_MEM_Generic_Subpixel< T >   )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedAlphaBlendSeparableInvocationSchedulerSelector,           &ScheduleAlphaBlendMT_Separable_MEM_Generic< T >            )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector,     &ScheduleBlendMT_NonSeparable_MEM_Generic_Subpixel< T >     )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendNonSeparableInvocationSchedulerSelector,             &ScheduleBlendMT_NonSeparable_MEM_Generic< T >              )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector,        &ScheduleBlendMT_Separable_MEM_Generic_Subpixel< T >        )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendSeparableInvocationSchedulerSelector,                &ScheduleBlendMT_Separable_MEM_Generic< T >                 )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector,             &ScheduleBlendMT_Misc_MEM_Generic_Subpixel< T >             )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendMiscInvocationSchedulerSelector,                     &ScheduleBlendMT_Misc_MEM_Generic< T >                      )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector,        &ScheduleTiledBlendMT_NonSeparable_MEM_Generic< T >         )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTiledBlendSeparableInvocationSchedulerSelector,           &ScheduleTiledBlendMT_Separable_MEM_Generic< T >            )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTiledBlendMiscInvocationSchedulerSelector,                &ScheduleTiledBlendMT_Misc_MEM_Generic< T >                 )

ULIS_NAMESPACE_END

