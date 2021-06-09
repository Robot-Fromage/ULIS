// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedDrawable.tpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the TAnimatedDrawable class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/AnimatedDrawable.h"

ULIS_NAMESPACE_BEGIN
TAnimatedDrawable::TAnimatedDrawable()
    : mImageCacheValid( false )
{}

bool
TAnimatedDrawable::IsImageCacheValid() const {
    return  mImageCacheValid;
}

FEvent
TAnimatedDrawable::RenderCache( FContext& iCtx ) {
    if( IsImageCacheValid() )
        return  FEvent::NoOP();

    ValidateImageCache();
    return  FEvent::NoOP();
}

FEvent
TAnimatedDrawable::RenderImage(
      FContext& iCtx
    , FBlock& ioBlock
    , uint32 iFrame
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
TAnimatedDrawable::InvalidImageCache() {
    mImageCacheValid = false;
}

void
TAnimatedDrawable::ValidateImageCache() {
    mImageCacheValid = true;
}


ULIS_NAMESPACE_END

