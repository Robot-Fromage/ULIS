// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Drawable.tpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the TDrawable class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/Drawable.h"

ULIS_NAMESPACE_BEGIN
TDrawable::TDrawable()
    : mImageCacheValid( false )
{}

bool
TDrawable::IsImageCacheValid() const {
    return  mImageCacheValid;
}

FEvent
TDrawable::RenderCache( FContext& iCtx ) {
    if( IsImageCacheValid() )
        return  FEvent::NoOP();

    ValidateImageCache();
    return  FEvent::NoOP();
}

FEvent
TDrawable::RenderImage(
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
TDrawable::InvalidImageCache() {
    mImageCacheValid = false;
}

void
TDrawable::ValidateImageCache() {
    mImageCacheValid = true;
}


ULIS_NAMESPACE_END

