// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerStack.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TAnimatedLayerStack class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Size2D.h"
#include "Layer/Components/Searchable.h"
#include "Layer/AnimatedLayer/AbstractAnimatedLayerDrawable.h"

ULIS_NAMESPACE_BEGIN

struct ULIS_API FDummyAnimatedSuperStack {
    ~FDummyAnimatedSuperStack() {
        ULIS_DEBUG_PRINTF( "FDummyAnimatedSuperStack Destroyed" )
    }

    FDummyAnimatedSuperStack() {
        ULIS_DEBUG_PRINTF( "FDummyAnimatedSuperStack Created" )
    }
};

/////////////////////////////////////////////////////
/// @class      TAnimatedLayerStack
/// @brief      The TAnimatedLayerStack class provides a class to store a layer stack
///             for painting applications.
template<
      class BlockType
    , class RendererType
    , class SuperStackExtra
>
class TAnimatedLayerStack final
    : public TAbstractAnimatedLayerDrawable< BlockType >
    , public TRoot< IAnimatedLayer >
    , public TSearchable< TRoot< IAnimatedLayer > >
    , public IHasSize2D
    , public IHasFormat
    , public IHasColorSpace
    , public SuperStackExtra
{
public:
    // DTor
    ~TAnimatedLayerStack() override;

    // CTor
    template< typename ... Args >
    TAnimatedLayerStack(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
        , const FOnAnimatedLayerSelfChanged& iOnSelfChanged = FOnAnimatedLayerSelfChanged()
        , const FOnAnimatedLayerNodeAdded& iOnLayerAdded = FOnAnimatedLayerNodeAdded()
        , const FOnAnimatedLayerNodeRemoved& iOnLayerRemoved = FOnAnimatedLayerNodeRemoved()
        , const FOnUserDataAdded& iOnUserDataAdded = FOnUserDataAdded()
        , const FOnUserDataChanged& iOnUserDataChanged = FOnUserDataChanged()
        , const FOnUserDataRemoved& iOnUserDataRemoved = FOnUserDataRemoved()
        , Args ... args
    );

    // Disable copy
    TAnimatedLayerStack( const TAnimatedLayerStack& ) = delete;
    TAnimatedLayerStack& operator=( const TAnimatedLayerStack& ) = delete;

public:
    // TAnimatedLayerStack Interface
    template< typename ... Args >
    void Reset(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
        , const FOnAnimatedLayerSelfChanged& iOnSelfChanged = FOnAnimatedLayerSelfChanged()
        , const FOnAnimatedLayerNodeAdded& iOnLayerAdded = FOnAnimatedLayerNodeAdded()
        , const FOnAnimatedLayerNodeRemoved& iOnLayerRemoved = FOnAnimatedLayerNodeRemoved()
        , const FOnUserDataAdded& iOnUserDataAdded = FOnUserDataAdded()
        , const FOnUserDataChanged& iOnUserDataChanged = FOnUserDataChanged()
        , const FOnUserDataRemoved& iOnUserDataRemoved = FOnUserDataRemoved()
        , Args ... args
    );

    // ITypeIdentifiable Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE( "Stack" );

    // TDrawable Interface
    FEvent RenderImage(
          FContext& iCtx
        , BlockType& ioBlock
        , uint32 iFrame
        , const FRectI& iRect = FRectI::Auto
        , const FVec2I& iPos = FVec2I( 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
    ) override;

    // TRoot Overload Shadow Interface
    // TSearchable Overload Shadow Interface
    using TRoot< ILayer >::operator[];
    using TSearchable< TRoot< ILayer > >::operator[];

private:
    // TNode< ILayer > Interface ( Hide )
    using TNode< ILayer >::SetParent;
};

ULIS_NAMESPACE_END

#include "Layer/AnimatedLayer/AnimatedLayerStack.tpp"

