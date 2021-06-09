// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Drawable.tpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasCachedImageRendering class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasCachedImageRendering.h"

ULIS_NAMESPACE_BEGIN
IHasCachedImageRendering::IHasCachedImageRendering()
    : mImageCacheValid( false )
{}

bool
IHasCachedImageRendering::IsImageCacheValid() const {
    return  mImageCacheValid;
}

FEvent
IHasCachedImageRendering::RenderCache( FContext& iCtx ) {
    if( IsImageCacheValid() )
        return  FEvent::NoOP();

    ValidateImageCache();
    return  FEvent::NoOP();
}

FEvent
IHasCachedImageRendering::RenderImage(
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
IHasCachedImageRendering::InvalidImageCache() {
    mImageCacheValid = false;
}

void
IHasCachedImageRendering::ValidateImageCache() {
    mImageCacheValid = true;
}


ULIS_NAMESPACE_END

