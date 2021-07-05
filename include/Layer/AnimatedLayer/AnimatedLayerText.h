// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerText.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TAnimatedLayerText class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Components/HasBlendInfo.h"
#include "Layer/Components/HasBlock.h"
#include "Layer/Components/Rasterizable.h"
#include "Layer/Components/HasText.h"
#include "Layer/Components/HasTransform.h"
#include "Layer/AnimatedLayer/AbstractAnimatedLayerDrawable.h"

ULIS_NAMESPACE_BEGIN
// Forward-declarations
template< class BlockType, class RendererType, class SuperStackExtra > class TAnimatedLayerStack;
template< class BlockType, class RasterizerType, class RendererType, class BlockAllocatorType, class LayerStackType > class TAnimatedLayerFolder;
template< class BlockType, class RasterizerType, class RendererType, class BlockAllocatorType, class LayerStackType > class TAnimatedLayerImage;
/////////////////////////////////////////////////////
/// @class      TAnimatedLayerText
/// @brief      The TAnimatedLayerText class provides a class to store text in a layer
///             of a layer stack for painting applications.
template<
      class BlockType
    , class RasterizerType
    , class RendererType
    , class BlockAllocatorType
    , class LayerStackType
>
class TAnimatedLayerText final
    : public TAbstractAnimatedLayerDrawable< BlockType >
    , public TRasterizable< TAnimatedLayerImage< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > >
    , public THasBlock< BlockType, BlockAllocatorType >
    , public IHasBlendInfo
    , public IHasText
    , public IHasTransform
{
    typedef TRoot< IAnimatedLayer > tParent;
    typedef TAnimatedLayerText< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > tSelf;
    typedef TAnimatedLayerFolder< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > tSiblingFolder;
    typedef TAnimatedLayerImage< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > tSiblingImage;
    typedef TAbstractAnimatedLayerDrawable< BlockType > tAbstractLayerDrawable;
    typedef TRasterizable< tSiblingImage > tRasterizable;
    typedef THasBlock< BlockType, BlockAllocatorType > tHasBlock;

public:
    // DTor
    virtual ~TAnimatedLayerText() override;

    // CTors
    TAnimatedLayerText(
          const FString& iName = "Untitled Text"
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
        , const FWString& iText = L"Lorem ipsum dolor sit amet..."
        , const FFont& iFont = FFont::DefaultFont
        , uint32 iFontSize = 12
        , const ISample& iTextColor = FColor::Black
        , bool iAntiAliased = true
        , const FTransformInfo& iTransform = {}
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
        , const FOnTextInfoChanged& iOnTextInfoChanged = FOnTextInfoChanged()
        , const FOnTransformInfoChanged& iOnTransformInfoChanged = FOnTransformInfoChanged()
    );

    TAnimatedLayerText(
          BlockType* iBlock
        , const FString& iName = "Untitled Text"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iPrettyColor = FColor::Transparent
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , const FWString& iText = L"Lorem ipsum dolor sit amet..."
        , const FFont& iFont = FFont::DefaultFont
        , uint32 iFontSize = 12
        , const ISample& iTextColor = FColor::Black
        , bool iAntiAliased = true
        , const FTransformInfo& iTransform = {}
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
        , const FOnTextInfoChanged& iOnTextInfoChanged = FOnTextInfoChanged()
        , const FOnTransformInfoChanged& iOnTransformInfoChanged = FOnTransformInfoChanged()
    );

    // Disable copy
    TAnimatedLayerText( const TAnimatedLayerText& ) = delete;
    TAnimatedLayerText& operator=( const TAnimatedLayerText& ) = delete;

public:
    // ITypeIdentifiable Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE( "AnimatedText" );

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

private:
    // TNode< IAnimatedLayer > Interface
    void InitFromParent( const TRoot< IAnimatedLayer >* iParent ) override;
};

ULIS_NAMESPACE_END

#include "Layer/AnimatedLayer/AnimatedLayerText.tpp"

    //Next Things to do:
    // 3) AnimatedLayerImage
    // 4) Add TimeBoundaries
    // 5) Add Cels
    // 6) ....
