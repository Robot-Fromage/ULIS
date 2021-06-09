// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerImage.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FAnimatedLayerImage class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/AnimatedLayer/AnimatedLayerImage.h"
#include "Layer/AnimatedLayer/AnimatedLayerFolder.h"
#include "Layer/AnimatedLayer/AnimatedLayerStack.h"
#include "Image/Block.h"
#include "Context/Context.h"

ULIS_NAMESPACE_BEGIN
FAnimatedLayerImage::~FAnimatedLayerImage() {
    if( mBlock )
        delete  mBlock;
}

FAnimatedLayerImage::FAnimatedLayerImage(
      const FString& iName
    , bool iPaintLocked
    , bool iTimeLocked
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
    : tSuperClass( iName, iPaintLocked, iTimeLocked, iVisible, iColor, iParent )
    , mBlock( nullptr )
    , mBlendMode( iBlendMode )
    , mAlphaMode( iAlphaMode )
    , mOpacity( iOpacity )
    , mAlphaLock( iAlphaLocked )
{
    if( iWidth && iHeight )
        mBlock = new FBlock( iWidth, iHeight, iFormat );
}

FAnimatedLayerImage::FAnimatedLayerImage(
      FBlock* iBlock
    , const FString& iName
    , bool iPaintLocked
    , bool iTimeLocked
    , bool iVisible
    , const FColor& iColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iAlphaLocked
    , tParent* iParent
)
    : tSuperClass( iName, iPaintLocked, iTimeLocked, iVisible, iColor, iParent )
    , mBlock( iBlock )
    , mBlendMode( iBlendMode )
    , mAlphaMode( iAlphaMode )
    , mOpacity( iOpacity )
    , mAlphaLock( iAlphaLocked )
{
}

FBlock&
FAnimatedLayerImage::Block() {
    return  *mBlock;
}

const FBlock&
FAnimatedLayerImage::Block() const {
    return  *mBlock;
}

bool
FAnimatedLayerImage::IsAlphaLocked() const {
    return  mAlphaLock;
}

eBlendMode
FAnimatedLayerImage::BlendMode() const {
    return  mBlendMode;
}

eAlphaMode
FAnimatedLayerImage::AlphaMode() const {
    return  mAlphaMode;
}

ufloat
FAnimatedLayerImage::Opacity() const {
    return  mOpacity;
}

void
FAnimatedLayerImage::SetBlendMode( eBlendMode iValue )
{
    mBlendMode = iValue;
    InvalidImageCache();
}

void
FAnimatedLayerImage::SetAlphaMode( eAlphaMode iValue ) {
    mAlphaMode = iValue;
    InvalidImageCache();
}

void
FAnimatedLayerImage::SetOpacity( ufloat iValue ) {
    mOpacity = iValue;
    InvalidImageCache();
}

void
FAnimatedLayerImage::SetAlphaLocked( bool iValue ) {
    mAlphaLock = iValue;
}

FEvent
FAnimatedLayerImage::RenderImage(
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
    FEvent ev;
    ulError err = iCtx.Blend(
          *mBlock
        , ioBlock
        , iRect
        , iPos
        , mBlendMode
        , mAlphaMode
        , mOpacity
        , iPolicy
        , iNumWait
        , iWaitList
        , &ev
    );
    ULIS_ASSERT( !err, "Error during layer image blend" );
    return  ev;
}

void
FAnimatedLayerImage::InitFromParent( const tParent* iParent ) {
    const tParent* topLevel = iParent->TopLevelParent();
    if( !topLevel )
        return;

    if( !mBlock ) {
        const IAnimatedLayer* layer = dynamic_cast< const IAnimatedLayer* >( topLevel );
        ULIS_ASSERT( layer, "Parent cannot be cast to IAnimatedLayer !" );
        uint32 typeID = layer->TypeID();
        switch( typeID ) {
            case FAnimatedLayerStack::StaticTypeID(): {
                const FAnimatedLayerStack* stack = dynamic_cast< const FAnimatedLayerStack* >( layer );
                ULIS_ASSERT( stack, "Parent cannot be cast to stack !" );
                mBlock = new FBlock( stack->Width(), stack->Height(), stack->Format() );
                break;
            }
            case FAnimatedLayerFolder::StaticTypeID(): {
                const FAnimatedLayerFolder* folder = dynamic_cast< const FAnimatedLayerFolder* >( layer );
                ULIS_ASSERT( folder, "Parent cannot be cast to folder !" );
                const FBlock& ref = folder->Block();
                mBlock = new FBlock( ref.Width(), ref.Height(), ref.Format() );
                break;
            }
        }
    }
}

ULIS_NAMESPACE_END

