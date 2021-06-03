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
#include "Layer/LayerFolder.h"
#include "Layer/LayerStack.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
FLayerImage::~FLayerImage() {
    if( mBlock )
        delete  mBlock;
}

FLayerImage::FLayerImage(
      const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iColor
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iAlphaLocked
    , tParent* iParent
)
    : tSuperClass( iName, iLocked, iVisible, iColor, iParent )
    , mBlock( nullptr )
    , mBlendMode( iBlendMode )
    , mAlphaMode( iAlphaMode )
    , mOpacity( iOpacity )
    , mAlphaLock( iAlphaLocked )
{
    if( iWidth && iHeight )
        mBlock = new FBlock( iWidth, iHeight, iFormat );
}

FLayerImage::FLayerImage(
      FBlock* iBlock
    , const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iAlphaLocked
    , tParent* iParent
)
    : tSuperClass( iName, iLocked, iVisible, iColor, iParent )
    , mBlock( iBlock )
    , mBlendMode( iBlendMode )
    , mAlphaMode( iAlphaMode )
    , mOpacity( iOpacity )
    , mAlphaLock( iAlphaLocked )
{
}

FBlock&
FLayerImage::Block() {
    return  *mBlock;
}

const FBlock&
FLayerImage::Block() const {
    return  *mBlock;
}

bool
FLayerImage::IsAlphaLocked() const {
    return  mAlphaLock;
}

eBlendMode
FLayerImage::BlendMode() const {
    return  mBlendMode;
}

eAlphaMode
FLayerImage::AlphaMode() const {
    return  mAlphaMode;
}

ufloat
FLayerImage::Opacity() const {
    return  mOpacity;
}

void
FLayerImage::SetBlendMode( eBlendMode iValue )
{
    mBlendMode = iValue;
    InvalidImageCache();
}

void
FLayerImage::SetAlphaMode( eAlphaMode iValue ) {
    mAlphaMode = iValue;
    InvalidImageCache();
}

void
FLayerImage::SetOpacity( ufloat iValue ) {
    mOpacity = iValue;
    InvalidImageCache();
}

void
FLayerImage::SetAlphaLocked( bool iValue ) {
    mAlphaLock = iValue;
}

FEvent
FLayerImage::RenderImage(
      FContext& iCtx
    , FBlock& ioBlock
    , const FRectI& iRect
    , const FVec2I& iPos
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    RenderCache( iCtx );
    return  FEvent::NoOP();
}

void
FLayerImage::InitFromParent( const tParent* iParent ) {
    const tParent* topLevel = iParent->TopLevelParent();
    if( !topLevel )
        return;

    if( !mBlock ) {
        const ILayer* layer = dynamic_cast< const ILayer* >( topLevel );
        ULIS_ASSERT( layer, "Parent cannot be cast to ILayer !" );
        uint32 typeID = layer->TypeID();
        switch( typeID ) {
            case FLayerStack::StaticTypeID(): {
                const FLayerStack* stack = dynamic_cast< const FLayerStack* >( layer );
                ULIS_ASSERT( stack, "Parent cannot be cast to stack !" );
                mBlock = new FBlock( stack->Width(), stack->Height(), stack->Format() );
                break;
            }
            case FLayerFolder::StaticTypeID(): {
                const FLayerFolder* folder = dynamic_cast< const FLayerFolder* >( layer );
                ULIS_ASSERT( folder, "Parent cannot be cast to folder !" );
                const FBlock& ref = folder->Block();
                mBlock = new FBlock( ref.Width(), ref.Height(), ref.Format() );
                break;
            }
        }
    }
}

ULIS_NAMESPACE_END

