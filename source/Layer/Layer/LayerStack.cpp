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
#pragma once
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
    , const FOnSelfChanged& iOnSelfChanged
    , const FOnNodeAdded& iOnLayerAdded
    , const FOnNodeRemoved& iOnLayerRemoved
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
)
{
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

/*
// TDrawable Interface
FEvent
FLayerStack::RenderImage(
      FContext& iCtx
    , BlockType& ioBlock
    , const FRectI& iRect
    , const FVec2I& iPos
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
) // override
{
    FEvent ev;
    iCtx.Clear( ioBlock, FRectI::Auto, FSchedulePolicy::CacheEfficient, iNumWait, iWaitList, &ev );
    const int max = static_cast< int >( Children().Size() ) - 1;
    bool bFirst = true;
    for( int i = max; i >= 0; --i ) {
        typedef TAbstractLayerDrawable< BlockType > tDrawable;
        tDrawable* drawable = dynamic_cast< tDrawable* >( &( Children()[i]->Self() ) );
        if( !drawable )
            continue;

        ev = drawable->RenderImage(
              iCtx
            , ioBlock
            , iRect
            , iPos
            , iPolicy
            , 1
            , &ev
        );
    }
    return  ev;
}
*/

ULIS_NAMESPACE_END

