// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerStack class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Layer/LayerStack.h"
#include "Layer/Common/LayerUtils.h"

ULIS_NAMESPACE_BEGIN
FLayerStack::~FLayerStack()
{
}

FLayerStack::FLayerStack(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
)
    : ILayer( "Stack", false, true, FColor::Transparent, nullptr )
    , TRoot< ILayer >()
    , IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , IHasSize2D( FVec2UI16( iWidth, iHeight ) )
{}

void
FLayerStack::Reset(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
)
{
    ReinterpretFormat( iFormat );
    AssignColorSpace( iColorSpace );
    ReinterpretSize( FVec2UI16( iWidth, iHeight ) );
    this->TRoot< ILayer >::Reset();
}

ILayer&
FLayerStack::operator[]( const FString& iName ) {
    return  FindLayerByFuzzyNameInContainer( iName, *this );
}

const ILayer&
FLayerStack::operator[]( const FString& iName ) const {
    return  FindLayerByFuzzyNameInContainer( iName, *this );
}

FEvent
FLayerStack::RenderImage(
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
    FEvent ev;
    for( int i = static_cast< int >( Children().Size() ) - 1; i >= 0; --i )
        ev = Children()[i]->Self().RenderImage( iCtx, ioBlock, iRect, iPos, iPolicy, iNumWait, iWaitList );
    return  ev;
}

ULIS_NAMESPACE_END

