// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerFolder.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TLayerFolder class.
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
#include "Layer/Layer/AbstractLayerDrawable.h"

ULIS_NAMESPACE_BEGIN
// Forward-declarations
template< class BlockType, class RendererType, class SuperStackExtra > class TLayerStack;
template< class BlockType, class RasterizerType, class RendererType, class BlockAllocatorType, class LayerStackType > class TLayerImage;

/////////////////////////////////////////////////////
/// @class      TLayerFolder
/// @brief      The TLayerFolder class provides a class to store a folder of
///             layers in a layer stack for painting applications.
template<
      class BlockType
    , class RasterizerType
    , class RendererType
    , class BlockAllocatorType
    , class LayerStackType
>
class TLayerFolder
    : public TAbstractLayerDrawable< BlockType >
    , public TRoot< ILayer >
    , public TSearchable< TRoot< ILayer > >
    , public TRasterizable< TLayerImage< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType > >
    , public THasBlock< BlockType, BlockAllocatorType >
    , public IHasBlendInfo
    , public IHasCollapse
{
public:
    // ITypeIdentifiable Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE( "Folder" );
};

ULIS_NAMESPACE_END

#include "Layer/Layer/LayerFolder.tpp"

