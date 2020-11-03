// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Blend entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Blend/Blend.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Blend/Dispatch/BlendInfo.h"
#include "Blend/Dispatch/Dispatch.ipp"

ULIS2_NAMESPACE_BEGIN

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
    ULIS2_ASSERT( iSource,                                  "Bad source."                                               );
    ULIS2_ASSERT( iBackdrop,                                "Bad destination."                                          );
    ULIS2_ASSERT( iSource->Format() == iBackdrop->Format(), "Formats do not match."                                     );
    ULIS2_ASSERT( iThreadPool,                              "Bad pool."                                                 );
    ULIS2_ASSERT( !iCallCB || iBlocking,                    "Callback flag is specified on non-blocking operation."     );

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
    auto forwardBlendInfo = std::make_shared< _FBlendInfoPrivate >();
    _FBlendInfoPrivate& forwardBlendInfoAlias   = *forwardBlendInfo;
    forwardBlendInfoAlias.pool                  = iThreadPool;
    forwardBlendInfoAlias.blocking              = iBlocking;
    forwardBlendInfoAlias.hostDeviceInfo        = &iHostDeviceInfo;
    forwardBlendInfoAlias.perfIntent            = iPerfIntent;
    forwardBlendInfoAlias.source                = iSource;
    forwardBlendInfoAlias.backdrop              = iBackdrop;
    forwardBlendInfoAlias.sourceRect            = src_roi;
    forwardBlendInfoAlias.subpixelComponent     = subpixelComponent;
    forwardBlendInfoAlias.buspixelComponent     = FVec2F( 1.f - subpixelComponent.x, 1.f - subpixelComponent.y );
    forwardBlendInfoAlias.subpixelFlag          = iSubpixelFlag;
    forwardBlendInfoAlias.blendingMode          = iBlendingMode;
    forwardBlendInfoAlias.alphaMode             = iAlphaMode;
    forwardBlendInfoAlias.opacityValue          = FMaths::Clamp( iOpacityValue, 0.f, 1.f );
    forwardBlendInfoAlias.shift                 = FVec2I( translationX, translationY );
    forwardBlendInfoAlias.backdropCoverage      = FVec2I( coverageX, coverageY );
    forwardBlendInfoAlias.backdropWorkingRect   = dst_fit;

    // Query dispatched method
    fpDispatchedBlendFunc fptr = QueryDispatchedBlendFunctionForParameters( forwardBlendInfoAlias );
    ULIS2_ASSERT( fptr, "No dispatch function found." );
    fptr( forwardBlendInfo );

    // Invalid
    forwardBlendInfo->backdrop->Invalidate( dst_fit, iCallCB );
}

ULIS2_NAMESPACE_END

