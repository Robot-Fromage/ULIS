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
#include "Maths/Geometry.h"
#include "Maths/Maths.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Blend
void Blend( FThreadPool*            iThreadPool
          , bool                    iBlocking
          , uint32                  iPerfIntent
          , const FHostDeviceInfo&  iHostDeviceInfo
          , bool                    iCallCB
          , const FBlock*           iSource
          , FBlock*                 iBackdrop
          , const FRect&            iSourceRect
          , const FVec2F&           iPosition
          , bool                    iSubpixelFlag
          , eBlendingMode           iBlendingMode
          , eAlphaMode              iAlphaMode
          , float                   iOpacityValue )
{
    // Assertions
    ULIS3_ASSERT( iSource,                                  "Bad source."                                               );
    ULIS3_ASSERT( iBackdrop,                                "Bad destination."                                          );
    ULIS3_ASSERT( iSource->Format() == iBackdrop->Format(), "Formats do not match."                                     );
    ULIS3_ASSERT( iThreadPool,                              "Bad pool."                                                 );
    ULIS3_ASSERT( !iCallCB || iBlocking,                    "Callback flag is specified on non-blocking operation."     );

    // Compute coordinates of target rect in destination, with source rect dimension
    // Ensure the selected target actually fits in destination
    FRect src_roi = iSourceRect & iSource->Rect();
    int target_xmin = static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.x ) );
    int target_ymin = static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.y ) );
    int target_xmax = iSubpixelFlag ? static_cast< int >( FMaths::RoundToPositiveInfinity( iPosition.x + src_roi.w ) ) : static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.x + src_roi.w ) );
    int target_ymax = iSubpixelFlag ? static_cast< int >( FMaths::RoundToPositiveInfinity( iPosition.y + src_roi.h ) ) : static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.y + src_roi.h ) );
    FRect dst_target = FRect::FromMinMax( target_xmin, target_ymin, target_xmax, target_ymax );
    FRect dst_fit    = dst_target & iBackdrop->Rect();

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
    iBackdrop->Invalidate( dst_fit, iCallCB );
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
               , const FRect&           iSourceRect
               , const FVec2F&          iPosition
               , bool                   iSubpixelFlag
               , float                  iOpacityValue )
{
    // Assertions
    ULIS3_ASSERT( iSource,                                  "Bad source."                                               );
    ULIS3_ASSERT( iBackdrop,                                "Bad destination."                                          );
    ULIS3_ASSERT( iSource->Format() == iBackdrop->Format(), "Formats do not match."                                     );
    ULIS3_ASSERT( iThreadPool,                              "Bad pool."                                                 );
    ULIS3_ASSERT( !iCallCB || iBlocking,                    "Callback flag is specified on non-blocking operation."     );
    ULIS3_ASSERT( iSource->HasAlpha(),                      "Cannot call AlphaBlend on format without alpha."           );

    // Compute coordinates of target rect in destination, with source rect dimension
    // Ensure the selected target actually fits in destination
    FRect src_roi = iSourceRect & iSource->Rect();
    int target_xmin = static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.x ) );
    int target_ymin = static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.y ) );
    int target_xmax = iSubpixelFlag ? static_cast< int >( FMaths::RoundToPositiveInfinity( iPosition.x + src_roi.w ) ) : static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.x + src_roi.w ) );
    int target_ymax = iSubpixelFlag ? static_cast< int >( FMaths::RoundToPositiveInfinity( iPosition.y + src_roi.h ) ) : static_cast< int >( FMaths::RoundToNegativeInfinity( iPosition.y + src_roi.h ) );
    FRect dst_target = FRect::FromMinMax( target_xmin, target_ymin, target_xmax, target_ymax );
    FRect dst_fit    = dst_target & iBackdrop->Rect();

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
    iBackdrop->Invalidate( dst_fit, iCallCB );
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
               , const FRect&               iSourceRect
               , const FRect&               iDestRect
               , const FVec2I&              iShift
               , eBlendingMode              iBlendingMode
               , eAlphaMode                 iAlphaMode
               , float                      iOpacityValue )
{
    // Assertions
    ULIS3_ASSERT( iSource,                                  "Bad source."                                               );
    ULIS3_ASSERT( iBackdrop,                                "Bad destination."                                          );
    ULIS3_ASSERT( iSource->Format() == iBackdrop->Format(), "Formats do not match."                                     );
    ULIS3_ASSERT( iThreadPool,                              "Bad pool."                                                 );
    ULIS3_ASSERT( !iCallCB || iBlocking,                    "Callback flag is specified on non-blocking operation."     );

    // Compute coordinates of target rect in destination, with source rect dimension
    // Ensure the selected target actually fits in destination
    FRect src_roi = iSourceRect & iSource->Rect();
    FRect dst_roi = iDestRect   & iBackdrop->Rect();

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
    commandArgsRef.subpixelFlag         = ULIS3_NOAA;
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
    iBackdrop->Invalidate( dst_roi, iCallCB );
}

/////////////////////////////////////////////////////
// BlendColor
void BlendColor( FThreadPool*           iThreadPool
               , uint32                 iPerfIntent
               , const FHostDeviceInfo& iHostDeviceInfo
               , bool                   iCallCB
               , const FPixelValue&     iColor
               , FBlock*                iBackdrop
               , const FRect&           iDestRect
               , eBlendingMode          iBlendingMode
               , eAlphaMode             iAlphaMode
               , float                  iOpacityValue )
{
    // Assertions
    ULIS3_ASSERT( iBackdrop,                                "Bad destination."                                          );
    ULIS3_ASSERT( iThreadPool,                              "Bad pool."                                                 );

    FPixelValue color( iBackdrop->Format() );
    Conv( iColor, color );
    FBlock block( color.Ptr(), 1, 1, iBackdrop->Format() );
    BlendTiled( iThreadPool, ULIS3_BLOCKING, iPerfIntent, iHostDeviceInfo, iCallCB, &block, iBackdrop, FRect( 0, 0, 1, 1 ), iDestRect, FVec2I( 0 ), iBlendingMode, iAlphaMode, iOpacityValue );
}

ULIS3_NAMESPACE_END

