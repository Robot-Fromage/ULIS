// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TLayerStack class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Size2D.h"
#include "Layer/Components/Searchable.h"
#include "Layer/Layer/AbstractLayerDrawable.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TLayerStack
/// @brief      The TLayerStack class provides a class to store a layer stack
///             for painting applications.
template<
      class BlockType
    , class RendererType
    , class SuperStackExtra
>
class TLayerStack final
    : public TAbstractLayerDrawable< BlockType >
    , public TRoot< ILayer >
    , public ISearchable
    , public IHasSize2D
    , public IHasFormat
    , public IHasColorSpace
    , public SuperStackExtra
{
public:
    // DTor
    ~TLayerStack() override;

    // CTor
    template< typename ... Args >
    TLayerStack(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
        , Args ... args
    );

    // Disable copy
    TLayerStack( const TLayerStack& ) = delete;
    TLayerStack& operator=( const TLayerStack& ) = delete;

public:
    // TLayerStack Interface
    void Reset(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
    );

    // ITypeIdentifiable Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE( "Stack" );

    // TDrawable Interface
    FEvent RenderImage(
          FContext& iCtx
        , BlockType& ioBlock
        , const FRectI& iRect = FRectI::Auto
        , const FVec2I& iPos = FVec2I( 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
    ) override;

    // TRoot Overload Shadow Interface
    using TRoot< ILayer >::operator[];
};

ULIS_NAMESPACE_END

#include "Layer/Layer/LayerStack.tpp"

