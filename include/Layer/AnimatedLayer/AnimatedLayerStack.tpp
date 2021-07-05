// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerStack.cpp
* @author       Clement Berthaud, Eric Scholl
* @brief        This file provides the definition for the TAnimatedLayerStack class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Layer/AnimatedLayer/AnimatedLayerStack.h"

// Template Macro Utility
#define TEMPLATE template< class BlockType, class RendererType, class SuperStackExtra >
#define CLASS TAnimatedLayerStack< BlockType, RendererType, SuperStackExtra >

ULIS_NAMESPACE_BEGIN
// DTor
TEMPLATE
CLASS::~TAnimatedLayerStack()
{
    ULIS_DEBUG_PRINTF( "TAnimatedLayerStack Destroyed" )
}

// CTor
TEMPLATE
template< typename ... Args >
CLASS::TAnimatedLayerStack(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnAnimatedLayerSelfChanged& iOnSelfChanged
    , const FOnAnimatedLayerNodeAdded& iOnLayerAdded
    , const FOnAnimatedLayerNodeRemoved& iOnLayerRemoved
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
    , Args ... args
)
    : TNode< IAnimatedLayer >(
          nullptr
        , FOnAnimatedLayerParentChanged()
        , iOnSelfChanged
    )
    , IAnimatedLayer(
          "LayerStack"
        , false
        , false
        , FColor::Transparent
        , nullptr
        , FOnNameChanged()
        , FOnBoolChanged()
        , FOnBoolChanged()
        , FOnColorChanged()
        , iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
        , FOnAnimatedLayerParentChanged()
        , iOnSelfChanged
    )
    , TAbstractAnimatedLayerDrawable< BlockType >(
          "LayerStack"
        , false
        , false
        , FColor::Transparent
        , nullptr
        , FOnNameChanged()
        , FOnBoolChanged()
        , FOnBoolChanged()
        , FOnColorChanged()
        , iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
        , FOnAnimatedLayerParentChanged()
    )
    , TRoot< IAnimatedLayer > (
          nullptr
        , FOnAnimatedLayerParentChanged()
        , iOnSelfChanged
        , iOnLayerAdded
        , iOnLayerRemoved
    )
    , TSearchable< TRoot< IAnimatedLayer > >()
    , IHasSize2D( FVec2UI16( iWidth, iHeight ) )
    , IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , SuperStackExtra( args ... )
{
    ULIS_DEBUG_PRINTF( "TAnimatedLayerStack Created" )
}

// TLayerStack Interface
TEMPLATE
template< typename ... Args >
void
CLASS::Reset(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , const FOnAnimatedLayerSelfChanged& iOnSelfChanged
    , const FOnAnimatedLayerNodeAdded& iOnLayerAdded
    , const FOnAnimatedLayerNodeRemoved& iOnLayerRemoved
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
    , Args ... args
)
{
    ReinterpretFormat( iFormat );
    AssignColorSpace( iColorSpace );
    ReinterpretSize( FVec2UI16( iWidth, iHeight ) );
    this->TRoot< IAnimatedLayer >::Reset(
          nullptr
        , FOnAnimatedLayerParentChanged()
        , iOnLayerAdded
        , iOnLayerRemoved
    );
    this->FOnAnimatedLayerSelfChanged::SetDelegate( iOnSelfChanged );
    this->FOnUserDataAdded::SetDelegate( iOnUserDataAdded );
    this->FOnUserDataChanged::SetDelegate( iOnUserDataChanged );
    this->FOnUserDataRemoved::SetDelegate( iOnUserDataRemoved );
}

// TDrawable Interface
TEMPLATE
FEvent
CLASS::RenderImage(
      FContext& iCtx
    , BlockType& ioBlock
    , uint32 iFrame
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
        typedef TAbstractAnimatedLayerDrawable< BlockType > tDrawable;
        tDrawable* drawable = dynamic_cast< tDrawable* >( &( Children()[i]->Self() ) );
        if( !drawable )
            continue;

        ev = drawable->RenderImage(
              iCtx
            , ioBlock
            , iFrame
            , iRect
            , iPos
            , iPolicy
            , 1
            , &ev
        );
    }
    return  ev;
}

ULIS_NAMESPACE_END

// Template Macro Utility
#undef TEMPLATE
#undef CLASS

