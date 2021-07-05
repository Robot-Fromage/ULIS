// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AbstractAnimatedLayerDrawable.h
* @author       Clement Berthaud, Eric Scholl
* @brief        This file provides the declaration for the TAbstractAnimatedLayerDrawable class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Components/AnimatedDrawable.h"
#include "Layer/AnimatedLayer/AnimatedLayer.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TAbstractAnimatedLayerDrawable
/// @brief      Simple TAbstractAnimatedLayerDrawable class.
template< class BlockType >
class TAbstractAnimatedLayerDrawable
    : public virtual IAnimatedLayer
    , public TAnimatedDrawable< BlockType >
{
public:
    using IAnimatedLayer::IAnimatedLayer;
    virtual ~TAbstractAnimatedLayerDrawable() override = 0 {
        ULIS_DEBUG_PRINTF( "TAbstractAnimatedLayerDrawable Destroyed" )
    }

    void OnChangeNotifiedInternal() override {
        ULIS_DEBUG_PRINTF( "Invalidation" )
        InvalidImageCache();
    }
};

ULIS_NAMESPACE_END

#include "Layer/AnimatedLayer/AbstractAnimatedLayerDrawable.tpp"

