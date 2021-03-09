// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerImage.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerImage class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/LayerImage.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
FLayerImage::~FLayerImage()
{
    delete  mBlock;
}

FLayerImage::FLayerImage( const FString& iName, uint16 iWidth, uint16 iHeight, eFormat iFormat )
    : ILayer( eLayerType::Layer_Image, iName )
    , mAlphaLock( false )
    , mBlock( nullptr )
{
    mBlock = new FBlock( iWidth, iHeight, iFormat );
}

FBlock&
FLayerImage::Block()
{
    return  *mBlock;
}

const FBlock&
FLayerImage::Block() const
{
    return  *mBlock;
}

bool
FLayerImage::IsAlphaLocked() const
{
    return  mAlphaLock;
}

void
FLayerImage::SetAlphaLocked( bool iValue )
{
    mAlphaLock = iValue;
}

ULIS_NAMESPACE_END

