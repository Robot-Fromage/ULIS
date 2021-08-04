// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerFolder.h
* @author       Clement Berthaud, Eric Scholl
* @brief        This file provides the declaration for the TAnimatedLayerFolder class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Components/HasBlendInfo.h"
#include "Layer/Components/HasBlock.h"
#include "Layer/Components/HasCollapse.h"
#include "Layer/Components/Rasterizable.h"
#include "Layer/Components/Searchable.h"
#include "Layer/AnimatedLayer/AbstractAnimatedLayerDrawable.h"

ULIS_NAMESPACE_BEGIN
// Forward-declarations
template< class BlockType, class RendererType, class SuperStackExtra > class TAnimatedLayerStack;
template< class BlockType, class RasterizerType, class RendererType, class BlockAllocatorType, class LayerStackType > class TAnimatedLayerImage;

/////////////////////////////////////////////////////
/// @class      TAnimatedLayerFolder
/// @brief      The TAnimatedLayerFolder class provides a class to store a folder of
///             layers in a layer stack for painting applications.
template<
      class BlockType
    , class RasterizerType
    , class RendererType
    , class BlockAllocatorType
    , class LayerStackType
>
class TAnimatedLayerFolder
    : public TAbstractAnimatedLayerDrawable< BlockType >
    , public TRoot< IAnimatedLayer >
    , public TSearchable< TRoot< IAnimatedLayer > >
    , public TRasterizable< TAnimatedLayerImage< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > >
    , public THasBlock< BlockType, BlockAllocatorType >
    , public IHasBlendInfo
    , public IHasCollapse
{
    typedef TRoot< IAnimatedLayer > tParent;
    typedef TAnimatedLayerImage< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > tSiblingImage;
    typedef TAnimatedLayerFolder< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > tSelf;
    typedef TAbstractAnimatedLayerDrawable< BlockType > tAbstractAnimatedLayerDrawable;
    typedef TRasterizable< tSiblingImage > tRasterizable;
    typedef THasBlock< BlockType, BlockAllocatorType > tHasBlock;

public:
    // DTor
    virtual ~TAnimatedLayerFolder() override;

    // CTors
    TAnimatedLayerFolder(
          const FString& iName = "Untitled Folder"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iPrettyColor = FColor::Transparent
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iCollapsed = false
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
        , const FOnAnimatedLayerNodeAdded& iOnLayerAdded = FOnAnimatedLayerNodeAdded()
        , const FOnAnimatedLayerNodeRemoved& iOnLayerRemoved = FOnAnimatedLayerNodeRemoved()

        , const TOnBlockChanged< BlockType >& iOnBlockChanged = TOnBlockChanged< BlockType >()
        , const FOnBlendInfoChanged& iOnBlendInfoChanged = FOnBlendInfoChanged()
        , const FOnBoolChanged& iOnCollapseChanged = FOnBoolChanged()
    );

    // Disable copy
    TAnimatedLayerFolder( const TAnimatedLayerFolder& ) = delete;
    TAnimatedLayerFolder& operator=( const TAnimatedLayerFolder& ) = delete;

public:
    // ITypeIdentifiable Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE( "AnimatedFolder" );

    // TDrawable Interface
    FEvent RenderImageCache( FContext& iCtx, uint32 iFrame ) override;
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
    tSiblingImage* Rasterize( FContext& iCtx, FEvent* oEvent = nullptr ) override;

    // TRoot Overload Shadow Interface
    // TSearchable Overload Shadow Interface
    using TRoot< IAnimatedLayer >::operator[];
    using TSearchable< TRoot< IAnimatedLayer > >::operator[];

    const TArray<FCelInfo> GetDrawableCelInfos(uint32* oFirstFrame) const override;

protected:
    using THasBlock::Block;

private:
    // TNode< IAnimatedLayer > Interface
    void InitFromParent( const TRoot< IAnimatedLayer >* iParent ) override;
};

ULIS_NAMESPACE_END

#include "Layer/AnimatedLayer/AnimatedLayerFolder.tpp"

