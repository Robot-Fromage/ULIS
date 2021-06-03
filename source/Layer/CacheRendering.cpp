// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CacheRendering.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the ICachedImageRendering class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Layer/CacheRendering.h"

ULIS_NAMESPACE_BEGIN
ICachedImageRendering::ICachedImageRendering()
    : mImageCacheValid( false )
{}

bool
ICachedImageRendering::IsImageCacheValid() const {
    return  mImageCacheValid;
}

void
ICachedImageRendering::RenderImage( FBlock& ioBlock, const FRectI& iRect, const FVec2I& iPos ) {
    if( IsImageCacheValid() )
        return;

    ValidateImageCache();
}

void
ICachedImageRendering::InvalidImageCache() {
    mImageCacheValid = false;
}

void
ICachedImageRendering::ValidateImageCache() {
    mImageCacheValid = true;
}


ULIS_NAMESPACE_END

