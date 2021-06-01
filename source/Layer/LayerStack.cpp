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
#include "Layer/LayerStack.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include <functional>

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

ULIS_NAMESPACE_END

