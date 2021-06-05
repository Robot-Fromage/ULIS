// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayer.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IAnimatedLayer class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/AnimatedLayer.h"
#include "Layer/AnimatedLayerStack.h"
#include "Layer/AnimatedLayerFolder.h"

ULIS_NAMESPACE_BEGIN
IAnimatedLayer::~IAnimatedLayer()
{
}

IAnimatedLayer::IAnimatedLayer(
      const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iColor
    , tParent* iParent
)
    : tSuperClass( iParent )
    , mName( iName )
    , mLocked( iLocked )
    , mVisible( iVisible )
    , mPrettyColor( iColor.ToFormat( Format_RGBA8 ) )
{}

const FString&
IAnimatedLayer::Name() const
{
    return  mName;
}

bool
IAnimatedLayer::Locked() const
{
    return  mLocked;
}

bool
IAnimatedLayer::Visible() const
{
    return  mVisible;
}

const FColor&
IAnimatedLayer::PrettyColor() const
{
    // supposed to be always RGBA8
    return  mPrettyColor;
}

void
IAnimatedLayer::SetName( const FString& iName )
{
    mName = iName;
}

void
IAnimatedLayer::SetLocked( bool iValue )
{
    mLocked = iValue;
}

void
IAnimatedLayer::SetVisible( bool iValue )
{
    mVisible = iValue;
    InvalidImageCache();
}

void
IAnimatedLayer::SetPrettyColor( const FColor& iColor )
{
    // Ensure conversion to local mPrettyColor format,
    // supposed to be always RGBA8
    FColor::ConvertFormat( iColor, mPrettyColor );
}

void
IAnimatedLayer::InvalidImageCache() {
    IAnimatedCachedImageRendering::InvalidImageCache();
    tParent* root = Parent();
    IAnimatedLayer* layer = dynamic_cast< IAnimatedLayer* >( root );
    if( !layer )
        return;

    uint32 typeID = layer->TypeID();
    switch( typeID ) {
        case FAnimatedLayerStack::StaticTypeID(): {
            FAnimatedLayerStack* stack = dynamic_cast< FAnimatedLayerStack* >( layer );
            ULIS_ASSERT( stack, "Parent cannot be cast to stack !" );
            stack->InvalidImageCache();
            break;
        }
        case FAnimatedLayerFolder::StaticTypeID(): {
            FAnimatedLayerFolder* folder = dynamic_cast< FAnimatedLayerFolder* >( layer );
            ULIS_ASSERT( folder, "Parent cannot be cast to folder !" );
            folder->InvalidImageCache();
            break;
        }
    }
}
ULIS_NAMESPACE_END

