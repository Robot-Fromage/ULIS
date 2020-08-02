// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Blend entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Blend/Blend.h"
#include "Blend/BlendArgs.h"
#include "Blend/BlendDispatch.ipp"
#include "Conv/Conv.h"
#include "Data/Block.h"
#include "Maths/Geometry/Rectangle.h"
#include "Maths/Geometry/Vector.h"
#include "Maths/Maths.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Blend
void Blend( FThreadPool*            iThreadPool
          , bool                    iBlocking
          , uint32                  iPerfIntent
          , const FHostDeviceInfo&  iHostDeviceInfo
          , bool                    iCallCB
          , const FBlock*           iSource
          , FBlock*                 iBackdrop
          , const FRectI&            iSourceRect
          , const FVec2F&           iPosition
          , bool                    iSubpixelFlag
          , eBlendingMode           iBlendingMode
          , eAlphaMode              iAlphaMode
          , float                   iOpacityValue )
{
    // Assertions
    ULIS_ASSERT( iSource,                                  "Bad source."                                               );
    ULIS_ASSERT( iBackdrop,                                "Bad destination."                                          );
    ULIS_ASSERT( iSource->Format() == iBackdrop->Format(), "Formats do not match."                                     );
    ULIS_ASSERT( iThreadPool,                              "Bad pool."                                                 );
    ULIS_ASSERT( !iCallCB || iBlocking,                    "Callback flag is specified on non-blocking operation."     );

    // Compute coordinates of target rect in destination, with source rect dimension
    // Ensure the selected target actually fits in destination
    FRectI src_roi = iSourceRect & iSource->Rect();
    int target_xmin = static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.x ) );
    int target_ymin = static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.y ) );
    int target_xmax = iSubpixelFlag ? static_cast< int >( FMaths::RoundToPositiveInfinity( iPosition.x + src_roi.w ) ) : static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.x + src_roi.w ) );
    int target_ymax = iSubpixelFlag ? static_cast< int >( FMaths::RoundToPositiveInfinity( iPosition.y + src_roi.h ) ) : static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.y + src_roi.h ) );
    FRectI dst_target = FRectI::FromMinMax( target_xmin, target_ymin, target_xmax, target_ymax );
    FRectI dst_fit    = dst_target & iBackdrop->Rect();

    // Check no-op
    if( dst_fit.Area() <= 0 )
        return;

    // Forward arguments baking
    FVec2F      subpixelComponent   = FMaths::FloatingPart( iPosition );
    const int   translationX        = dst_fit.x - dst_target.x;
    const int   translationY        = dst_fit.y - dst_target.y;
    const int   coverageX           = src_roi.w - ( src_roi.x + translationX ) >= dst_fit.w ? dst_fit.w : static_cast< int >( dst_fit.w - ceil( subpixelComponent.x ) );
    const int   coverageY           = src_roi.h - ( src_roi.y + translationY ) >= dst_fit.h ? dst_fit.h : static_cast< int >( dst_fit.h - ceil( subpixelComponent.y ) );

    // Bake forward params, shared Ptr for thread safety and scope life time extension in non blocking multithreaded processing
    auto commandArgs = std::make_shared< FBlendArgs >();
    FBlendArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool                 = iThreadPool;
    commandArgsRef.blocking             = iBlocking;
    commandArgsRef.hostDeviceInfo       = &iHostDeviceInfo;
    commandArgsRef.perfIntent           = iPerfIntent;
    commandArgsRef.source               = iSource;
    commandArgsRef.backdrop             = iBackdrop;
    commandArgsRef.sourceRect           = src_roi;
    commandArgsRef.subpixelComponent    = subpixelComponent;
    commandArgsRef.buspixelComponent    = FVec2F( 1.f - subpixelComponent.x, 1.f - subpixelComponent.y );
    commandArgsRef.subpixelFlag         = iSubpixelFlag;
    commandArgsRef.blendingMode         = iBlendingMode;
    commandArgsRef.alphaMode            = iAlphaMode;
    commandArgsRef.opacityValue         = FMaths::Clamp( iOpacityValue, 0.f, 1.f );
    commandArgsRef.shift                = FVec2I( translationX, translationY );
    commandArgsRef.backdropCoverage     = FVec2I( coverageX, coverageY );
    commandArgsRef.backdropWorkingRect  = dst_fit;

    // Query dispatched method
    fpBlendInvocation dispatchedInvocation = TDispatcher< FBlendInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo(), commandArgsRef );
    dispatchedInvocation( commandArgs );

    // Invalid
    iBackdrop->Dirty( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// AlphaBlend
void AlphaBlend( FThreadPool*           iThreadPool
               , bool                   iBlocking
               , uint32                 iPerfIntent
               , const FHostDeviceInfo& iHostDeviceInfo
               , bool                   iCallCB
               , const FBlock*          iSource
               , FBlock*                iBackdrop
               , const FRectI&           iSourceRect
               , const FVec2F&          iPosition
               , bool                   iSubpixelFlag
               , float                  iOpacityValue )
{
    // Assertions
    ULIS_ASSERT( iSource,                                  "Bad source."                                               );
    ULIS_ASSERT( iBackdrop,                                "Bad destination."                                          );
    ULIS_ASSERT( iSource->Format() == iBackdrop->Format(), "Formats do not match."                                     );
    ULIS_ASSERT( iThreadPool,                              "Bad pool."                                                 );
    ULIS_ASSERT( !iCallCB || iBlocking,                    "Callback flag is specified on non-blocking operation."     );
    ULIS_ASSERT( iSource->HasAlpha(),                      "Cannot call AlphaBlend on format without alpha."           );

    // Compute coordinates of target rect in destination, with source rect dimension
    // Ensure the selected target actually fits in destination
    FRectI src_roi = iSourceRect & iSource->Rect();
    int target_xmin = static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.x ) );
    int target_ymin = static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.y ) );
    int target_xmax = iSubpixelFlag ? static_cast< int >( FMaths::RoundToPositiveInfinity( iPosition.x + src_roi.w ) ) : static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.x + src_roi.w ) );
    int target_ymax = iSubpixelFlag ? static_cast< int >( FMaths::RoundToPositiveInfinity( iPosition.y + src_roi.h ) ) : static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.y + src_roi.h ) );
    FRectI dst_target = FRectI::FromMinMax( target_xmin, target_ymin, target_xmax, target_ymax );
    FRectI dst_fit    = dst_target & iBackdrop->Rect();

    // Check no-op
    if( dst_fit.Area() <= 0 )
        return;

    // Forward arguments baking
    FVec2F      subpixelComponent   = FMaths::FloatingPart( iPosition );
    const int   translationX        = dst_fit.x - dst_target.x;
    const int   translationY        = dst_fit.y - dst_target.y;
    const int   coverageX           = src_roi.w - ( src_roi.x + translationX ) >= dst_fit.w ? dst_fit.w : static_cast< int >( dst_fit.w - ceil( subpixelComponent.x ) );
    const int   coverageY           = src_roi.h - ( src_roi.y + translationY ) >= dst_fit.h ? dst_fit.h : static_cast< int >( dst_fit.h - ceil( subpixelComponent.y ) );

    // Bake forward params, shared Ptr for thread safety and scope life time extension in non blocking multithreaded processing
    auto commandArgs = std::make_shared< FBlendArgs >();
    FBlendArgs& commandArgsRef  = *commandArgs;
    commandArgsRef.pool                 = iThreadPool;
    commandArgsRef.blocking             = iBlocking;
    commandArgsRef.hostDeviceInfo       = &iHostDeviceInfo;
    commandArgsRef.perfIntent           = iPerfIntent;
    commandArgsRef.source               = iSource;
    commandArgsRef.backdrop             = iBackdrop;
    commandArgsRef.sourceRect           = src_roi;
    commandArgsRef.subpixelComponent    = subpixelComponent;
    commandArgsRef.buspixelComponent    = FVec2F( 1.f - subpixelComponent.x, 1.f - subpixelComponent.y );
    commandArgsRef.subpixelFlag         = iSubpixelFlag;
    commandArgsRef.blendingMode         = BM_NORMAL;
    commandArgsRef.alphaMode            = AM_NORMAL;
    commandArgsRef.opacityValue         = FMaths::Clamp( iOpacityValue, 0.f, 1.f );
    commandArgsRef.shift                = FVec2I( translationX, translationY );
    commandArgsRef.backdropCoverage     = FVec2I( coverageX, coverageY );
    commandArgsRef.backdropWorkingRect  = dst_fit;

    // Query dispatched method
    fpBlendInvocation dispatchedInvocation = TDispatcher< FAlphaBlendInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo(), commandArgsRef );
    dispatchedInvocation( commandArgs );

    // Invalid
    iBackdrop->Dirty( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// BlendTiled
void BlendTiled( FThreadPool*               iThreadPool
               , bool                       iBlocking
               , uint32                     iPerfIntent
               , const FHostDeviceInfo&     iHostDeviceInfo
               , bool                       iCallCB
               , const FBlock*              iSource
               , FBlock*                    iBackdrop
               , const FRectI&               iSourceRect
               , const FRectI&               iDestRect
               , const FVec2I&              iShift
               , eBlendingMode              iBlendingMode
               , eAlphaMode                 iAlphaMode
               , float                      iOpacityValue )
{
    // Assertions
    ULIS_ASSERT( iSource,                                  "Bad source."                                               );
    ULIS_ASSERT( iBackdrop,                                "Bad destination."                                          );
    ULIS_ASSERT( iSource->Format() == iBackdrop->Format(), "Formats do not match."                                     );
    ULIS_ASSERT( iThreadPool,                              "Bad pool."                                                 );
    ULIS_ASSERT( !iCallCB || iBlocking,                    "Callback flag is specified on non-blocking operation."     );

    // Compute coordinates of target rect in destination, with source rect dimension
    // Ensure the selected target actually fits in destination
    FRectI src_roi = iSourceRect & iSource->Rect();
    FRectI dst_roi = iDestRect   & iBackdrop->Rect();

    // Check no-op
    if( dst_roi.Area() <= 0 || src_roi.Area() <= 0 )
        return;

    // Forward arguments baking
    const int   coverageX = dst_roi.w;
    const int   coverageY = dst_roi.h;
    FVec2I src_size( src_roi.w, src_roi.h );

    const int   translationX = dst_roi.x - iDestRect.x;
    const int   translationY = dst_roi.y - iDestRect.y;
    FVec2I translation( translationX, translationY );
    FVec2I mod_shift = FMaths::PyModulo( - FMaths::PyModulo( iShift - translation, src_size ), src_size );

    // Bake forward params, shared Ptr for thread safety and scope life time extension in non blocking multithreaded processing
    auto commandArgs = std::make_shared< FBlendArgs >();
    FBlendArgs& commandArgsRef   = *commandArgs;
    commandArgsRef.pool                 = iThreadPool;
    commandArgsRef.blocking             = iBlocking;
    commandArgsRef.hostDeviceInfo       = &iHostDeviceInfo;
    commandArgsRef.perfIntent           = iPerfIntent;
    commandArgsRef.source               = iSource;
    commandArgsRef.backdrop             = iBackdrop;
    commandArgsRef.sourceRect           = src_roi;
    commandArgsRef.subpixelComponent    = FVec2F();
    commandArgsRef.buspixelComponent    = FVec2F( 1.f );
    commandArgsRef.subpixelFlag         = ULIS_NOAA;
    commandArgsRef.blendingMode         = iBlendingMode;
    commandArgsRef.alphaMode            = iAlphaMode;
    commandArgsRef.opacityValue         = FMaths::Clamp( iOpacityValue, 0.f, 1.f );
    commandArgsRef.shift                = mod_shift;
    commandArgsRef.backdropCoverage     = FVec2I( coverageX, coverageY );
    commandArgsRef.backdropWorkingRect  = dst_roi;

    // Query dispatched method
    fpBlendInvocation dispatchedInvocation = TDispatcher< FTiledBlendInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo(), commandArgsRef );
    dispatchedInvocation( commandArgs );

    // Invalid
    iBackdrop->Dirty( dst_roi, iCallCB );
}

/////////////////////////////////////////////////////
// BlendColor
void BlendColor( FThreadPool*           iThreadPool
               , uint32                 iPerfIntent
               , const FHostDeviceInfo& iHostDeviceInfo
               , bool                   iCallCB
               , const FColor&     iColor
               , FBlock*                iBackdrop
               , const FRectI&           iDestRect
               , eBlendingMode          iBlendingMode
               , eAlphaMode             iAlphaMode
               , float                  iOpacityValue )
{
    // Assertions
    ULIS_ASSERT( iBackdrop,                                "Bad destination."                                          );
    ULIS_ASSERT( iThreadPool,                              "Bad pool."                                                 );

    FColor color( iBackdrop->Format() );
    Conv( iColor, color );
    FBlock block( color.Bits(), 1, 1, iBackdrop->Format() );
    BlendTiled( iThreadPool, ULIS_BLOCKING, iPerfIntent, iHostDeviceInfo, iCallCB, &block, iBackdrop, FRectI( 0, 0, 1, 1 ), iDestRect, FVec2I( 0 ), iBlendingMode, iAlphaMode, iOpacityValue );
}

/////////////////////////////////////////////////////
// Blend PRNG for pseudo random modes like Dissolve
static uint32 sBlendPRNGSeed = 5323; // Arbitrary

void ResetBlendPRNGSeed() {
    sBlendPRNGSeed = 5323;
}

void SetBlendPRNGSeed( uint32 iVal ) {
    sBlendPRNGSeed = iVal;
}

uint32 GetBlendPRNGSeed() {
    return  sBlendPRNGSeed;
}

uint32 GenerateBlendPRNG() {
    sBlendPRNGSeed = 8253729 * sBlendPRNGSeed + 2396403;
    return sBlendPRNGSeed % 65537;
}

ULIS_NAMESPACE_END

