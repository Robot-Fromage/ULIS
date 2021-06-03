// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Layer.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the ILayer class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Layer.h"
#include "Layer/LayerStack.h"
#include "Layer/LayerFolder.h"

ULIS_NAMESPACE_BEGIN
ILayer::~ILayer()
{
}

ILayer::ILayer(
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
ILayer::Name() const
{
    return  mName;
}

bool
ILayer::Locked() const
{
    return  mLocked;
}

bool
ILayer::Visible() const
{
    return  mVisible;
}

const FColor&
ILayer::PrettyColor() const
{
    // supposed to be always RGBA8
    return  mPrettyColor;
}

void
ILayer::SetName( const FString& iName )
{
    mName = iName;
}

void
ILayer::SetLocked( bool iValue )
{
    mLocked = iValue;
}

void
ILayer::SetVisible( bool iValue )
{
    mVisible = iValue;
    InvalidImageCache();
}

void
ILayer::SetPrettyColor( const FColor& iColor )
{
    // Ensure conversion to local mPrettyColor format,
    // supposed to be always RGBA8
    FColor::ConvertFormat( iColor, mPrettyColor );
}

void
ILayer::InvalidImageCache() {
    ICachedImageRendering::InvalidImageCache();
    tParent* root = Parent();
    ILayer* layer = dynamic_cast< ILayer* >( root );
    if( !layer )
        return;

    uint32 typeID = layer->TypeID();
    switch( typeID ) {
        case FLayerStack::StaticTypeID(): {
            FLayerStack* stack = dynamic_cast< FLayerStack* >( layer );
            ULIS_ASSERT( stack, "Parent cannot be cast to stack !" );
            stack->InvalidImageCache();
            break;
        }
        case FLayerFolder::StaticTypeID(): {
            FLayerFolder* folder = dynamic_cast< FLayerFolder* >( layer );
            ULIS_ASSERT( folder, "Parent cannot be cast to folder !" );
            folder->InvalidImageCache();
            break;
        }
    }
}
ULIS_NAMESPACE_END

