// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerImage.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TAnimatedLayerImage class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Components/HasBlendInfo.h"
#include "Layer/Components/HasBlock.h"
#include "Layer/Components/Rasterizable.h"
#include "Layer/AnimatedLayer/AbstractAnimatedLayerDrawable.h"

ULIS_NAMESPACE_BEGIN
// Forward-declarations
template< class BlockType, class RendererType, class SuperStackExtra > class TAnimatedLayerStack;
template< class BlockType, class RasterizerType, class RendererType, class BlockAllocatorType, class LayerStackType > class TAnimatedLayerFolder;

/////////////////////////////////////////////////////
/// @class      TAnimatedLayerImage
/// @brief      The TAnimatedLayerImage class provides a class to store an image in a
///             layer stack for painting applications.
template<
      class BlockType
    , class RasterizerType
    , class RendererType
    , class BlockAllocatorType
    , class LayerStackType
>
class TAnimatedLayerImage
    : public TAbstractAnimatedLayerDrawable< BlockType >
    , public TRasterizable< TAnimatedLayerImage< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > >
    , public THasBlock< BlockType, BlockAllocatorType >
    , public IHasBlendInfo
    , public IHasPaintLock
{
    typedef TRoot< IAnimatedLayer > tParent;
    typedef TAnimatedLayerImage< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > tSelf;
    typedef TAnimatedLayerFolder< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > tSiblingFolder;
    typedef TAbstractAnimatedLayerDrawable< BlockType > tAbstractLayerDrawable;
    typedef TRasterizable< tSelf > tRasterizable;
    typedef THasBlock< BlockType, BlockAllocatorType > tHasBlock;

public:
    // DTor
    virtual ~TAnimatedLayerImage() override;

    // CTors
    TAnimatedLayerImage(
          const FString& iName = "Untitled Image"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iPrettyColor = FColor::Transparent
        , uint16 iWidth = 0
        , uint16 iHeight = 0
        , eFormat iFormat = Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iAlphaLocked = false
        , const TRoot< IAnimatedLayer >* iParent = nullptr

        , const FOnNameChanged& iOnNameChanged = FOnNameChanged()
        , const FOnBoolChanged& iOnLockChanged = FOnBoolChanged()
        , const FOnBoolChanged& iOnVisibleChanged = FOnBoolChanged()
        , const FOnColorChanged& iOnColorChanged = FOnColorChanged()
        , const FOnUserDataAdded& iOnUserDataAdded = FOnUserDataAdded()
        , const FOnUserDataChanged& iOnUserDataChanged = FOnUserDataChanged()
        , const FOnUserDataRemoved& iOnUserDataRemoved = FOnUserDataRemoved()
        , const FOnAnimatedLayerParentChanged& iOnParentChanged = FOnAnimatedLayerParentChanged()
        , const FOnAnimatedLayerSelfChanged& iOnSelfChanged = FOnAnimatedLayerSelfChanged()

        , const TOnBlockChanged< BlockType >& iOnBlockChanged = TOnBlockChanged< BlockType >()
        , const FOnBlendInfoChanged& iOnBlendInfoChanged = FOnBlendInfoChanged()
        , const FOnBoolChanged& iOnPaintLockChanged = FOnBoolChanged()
    );

    TAnimatedLayerImage(
          BlockType* iBlock
        , const FString& iName = "Untitled Image"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iPrettyColor = FColor::Transparent
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iAlphaLocked = false
        , const TRoot< IAnimatedLayer >* iParent = nullptr

        , const FOnNameChanged& iOnNameChanged = FOnNameChanged()
        , const FOnBoolChanged& iOnLockChanged = FOnBoolChanged()
        , const FOnBoolChanged& iOnVisibleChanged = FOnBoolChanged()
        , const FOnColorChanged& iOnColorChanged = FOnColorChanged()
        , const FOnUserDataAdded& iOnUserDataAdded = FOnUserDataAdded()
        , const FOnUserDataChanged& iOnUserDataChanged = FOnUserDataChanged()
        , const FOnUserDataRemoved& iOnUserDataRemoved = FOnUserDataRemoved()
        , const FOnAnimatedLayerParentChanged& iOnParentChanged = FOnAnimatedLayerParentChanged()
        , const FOnAnimatedLayerSelfChanged& iOnSelfChanged = FOnAnimatedLayerSelfChanged()

        , const TOnBlockChanged< BlockType >& iOnBlockChanged = TOnBlockChanged< BlockType >()
        , const FOnBlendInfoChanged& iOnBlendInfoChanged = FOnBlendInfoChanged()
        , const FOnBoolChanged& iOnPaintLockChanged = FOnBoolChanged()
    );

    // Disable copy
    TAnimatedLayerImage( const TAnimatedLayerImage& ) = delete;
    TAnimatedLayerImage& operator=( const TAnimatedLayerImage& ) = delete;

public:
    // ITypeIdentifiable Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE( "AnimatedImage" );

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

    // TRasterizable Interface
    tSelf* Rasterize( FContext& iCtx, FEvent* oEvent = nullptr ) override;

private:
    // TNode< IAnimatedLayer > Interface
    void InitFromParent( const TRoot< IAnimatedLayer >* iParent ) override;
};

ULIS_NAMESPACE_END

#include "Layer/AnimatedLayer/AnimatedLayerImage.tpp"

