// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerStack FLayerStack.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Layer/LayerStack.h"

ULIS_NAMESPACE_BEGIN
// DTor
FLayerStack::~FLayerStack()
{
    ULIS_DEBUG_PRINTF( "FLayerStack Destroyed" )
}

// CTor
FLayerStack::FLayerStack(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , FTilePool* iTilePool
    , const FOnSelfChanged& iOnSelfChanged
    , const FOnNodeAdded& iOnLayerAdded
    , const FOnNodeRemoved& iOnLayerRemoved
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
)
    : TNode< ILayer >(
          nullptr
        , FOnParentChanged()
        , iOnSelfChanged
    )
    , ILayer(
          "LayerStack"
        , false
        , true
        , FColor::Transparent
        , nullptr
        , FOnNameChanged()
        , FOnBoolChanged()
        , FOnBoolChanged()
        , FOnColorChanged()
        , iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
        , FOnParentChanged()
        , iOnSelfChanged
    )
    , TRoot< ILayer > (
          nullptr
        , FOnParentChanged()
        , iOnSelfChanged
        , iOnLayerAdded
        , iOnLayerRemoved
    )
    , TSearchable< TRoot< ILayer > >()
    , IHasSize2D( FVec2UI16( iWidth, iHeight ) )
    , IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , mTilePool( iTilePool )
{
    ULIS_DEBUG_PRINTF( "FLayerStack Created" )
}

// FLayerStack Interface
void
FLayerStack::Reset(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , FTilePool* iTilePool
    , const FOnSelfChanged& iOnSelfChanged
    , const FOnNodeAdded& iOnLayerAdded
    , const FOnNodeRemoved& iOnLayerRemoved
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
)
{
    mTilePool = iTilePool;
    ReinterpretFormat( iFormat );
    AssignColorSpace( iColorSpace );
    ReinterpretSize( FVec2UI16( iWidth, iHeight ) );
    this->TRoot< ILayer >::Reset(
          nullptr
        , FOnParentChanged()
        , iOnLayerAdded
        , iOnLayerRemoved
    );
    this->FOnSelfChanged::SetDelegate( iOnSelfChanged );
    this->FOnUserDataAdded::SetDelegate( iOnUserDataAdded );
    this->FOnUserDataChanged::SetDelegate( iOnUserDataChanged );
    this->FOnUserDataRemoved::SetDelegate( iOnUserDataRemoved );
}

FTilePool*
FLayerStack::TilePool() const {
    return  mTilePool;
}

ULIS_NAMESPACE_END

