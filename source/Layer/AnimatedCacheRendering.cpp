// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedCacheRendering.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IAnimatedCachedImageRendering class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Layer/AnimatedCacheRendering.h"

ULIS_NAMESPACE_BEGIN
IAnimatedCachedImageRendering::IAnimatedCachedImageRendering()
    : mImageCacheValid( false )
{}

bool
IAnimatedCachedImageRendering::IsImageCacheValid() const {
    return  mImageCacheValid;
}

FEvent
IAnimatedCachedImageRendering::RenderCache( FContext& iCtx ) {
    if( IsImageCacheValid() )
        return  FEvent::NoOP();

    ValidateImageCache();
    return  FEvent::NoOP();
}

FEvent
IAnimatedCachedImageRendering::RenderImage(
      FContext& iCtx
    , FBlock& ioBlock
    , const FRectI& iRect
    , const FVec2I& iPos
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
)
{
    RenderCache( iCtx );
    return  FEvent::NoOP();
}

void
IAnimatedCachedImageRendering::InvalidImageCache() {
    mImageCacheValid = false;
}

void
IAnimatedCachedImageRendering::ValidateImageCache() {
    mImageCacheValid = true;
}


ULIS_NAMESPACE_END

