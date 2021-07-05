// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerFolder.cpp
* @author       Clement Berthaud, Eric Scholl
* @brief        This file provides the definition for the TAnimatedLayerFolder class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Layer/AnimatedLayer/AnimatedLayerFolder.h"

// Template Macro Utility
#define TEMPLATE template< class BlockType, class RasterizerType, class RendererType, class BlockAllocatorType, class LayerStackType >
#define CLASS TAnimatedLayerFolder< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType >

ULIS_NAMESPACE_BEGIN
TEMPLATE
CLASS::~TAnimatedLayerFolder() {
    ULIS_DEBUG_PRINTF( "TAnimatedLayerFolder Destroyed" )
}

TEMPLATE
CLASS::TAnimatedLayerFolder(
      const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iCollapsed
    , const TRoot< IAnimatedLayer >* iParent

    , const FOnNameChanged& iOnNameChanged
    , const FOnBoolChanged& iOnLockChanged
    , const FOnBoolChanged& iOnVisibleChanged
    , const FOnColorChanged& iOnColorChanged
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
    , const FOnAnimatedLayerParentChanged& iOnParentChanged
    , const FOnAnimatedLayerSelfChanged& iOnSelfChanged
    , const FOnAnimatedLayerNodeAdded& iOnLayerAdded
    , const FOnAnimatedLayerNodeRemoved& iOnLayerRemoved

    , const TOnBlockChanged< BlockType >& iOnBlockChanged
    , const FOnBlendInfoChanged& iOnBlendInfoChanged
    , const FOnBoolChanged& iOnCollapseChanged
)
    : TNode< IAnimatedLayer >(
          iParent
        , iOnParentChanged
        , iOnSelfChanged
    )
    , IAnimatedLayer(
          iName
        , iLocked
        , iVisible
        , iPrettyColor
        , iParent

        , iOnNameChanged
        , iOnLockChanged
        , iOnVisibleChanged
        , iOnColorChanged
        , iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
        , iOnParentChanged
        , iOnSelfChanged
    )
    , tAbstractAnimatedLayerDrawable(
          iName
        , iLocked
        , iVisible
        , iPrettyColor
        , iParent

        , iOnNameChanged
        , iOnLockChanged
        , iOnVisibleChanged
        , iOnColorChanged
        , iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
        , iOnParentChanged
        , iOnSelfChanged
    )
    , tRasterizable()
    , tHasBlock(
          iWidth
        , iHeight
        , iFormat
        , iColorSpace
        , iOnBlockChanged
    )
    , IHasBlendInfo(
          iBlendMode
        , iAlphaMode
        , iOpacity
        , iOnBlendInfoChanged
    )
    , IHasCollapse(
          iCollapsed
        , iOnCollapseChanged
    )
{
    ULIS_DEBUG_PRINTF( "TAnimatedLayerFolder Created" )
}

TEMPLATE
CLASS::TAnimatedLayerFolder(
      BlockType* iBlock
    , const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iCollapsed
    , const TRoot< IAnimatedLayer >* iParent

    , const FOnNameChanged& iOnNameChanged
    , const FOnBoolChanged& iOnLockChanged
    , const FOnBoolChanged& iOnVisibleChanged
    , const FOnColorChanged& iOnColorChanged
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
    , const FOnAnimatedLayerParentChanged& iOnParentChanged
    , const FOnAnimatedLayerSelfChanged& iOnSelfChanged
    , const FOnAnimatedLayerNodeAdded& iOnLayerAdded
    , const FOnAnimatedLayerNodeRemoved& iOnLayerRemoved

    , const TOnBlockChanged< BlockType >& iOnBlockChanged
    , const FOnBlendInfoChanged& iOnBlendInfoChanged
    , const FOnBoolChanged& iOnCollapseChanged
)
    : TNode< IAnimatedLayer >(
          iParent
        , iOnParentChanged
        , iOnSelfChanged
    )
    , IAnimatedLayer(
          iName
        , iLocked
        , iVisible
        , iPrettyColor
        , iParent

        , iOnNameChanged
        , iOnLockChanged
        , iOnVisibleChanged
        , iOnColorChanged
        , iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
        , iOnParentChanged
        , iOnSelfChanged
    )
    , tAbstractAnimatedLayerDrawable(
          iName
        , iLocked
        , iVisible
        , iPrettyColor
        , iParent

        , iOnNameChanged
        , iOnLockChanged
        , iOnVisibleChanged
        , iOnColorChanged
        , iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
        , iOnParentChanged
        , iOnSelfChanged
    )
    , tRasterizable()
    , tHasBlock(
          iBlock
        , iOnBlockChanged
    )
    , IHasBlendInfo(
          iBlendMode
        , iAlphaMode
        , iOpacity
        , iOnBlendInfoChanged
    )
    , IHasCollapse(
          iCollapsed
        , iOnCollapseChanged
    )
{
    ULIS_DEBUG_PRINTF( "TLayerFolder Created" )
}

// TDrawable Interface
TEMPLATE
FEvent
CLASS::RenderImageCache( FContext& iCtx, uint32 iFrame ) // override
{
    if( IsImageCacheValid(iFrame) )
        return  FEvent::NoOP();

    FEvent ev;
    iCtx.Clear( *Block(), FRectI::Auto, FSchedulePolicy::CacheEfficient, 0, nullptr, &ev );
    const int max = static_cast< int >( Children().Size() ) - 1;
    for( int i = max; i >= 0; --i ) {
        tAbstractAnimatedLayerDrawable* drawable = dynamic_cast< tAbstractAnimatedLayerDrawable* >( &( Children()[i]->Self() ) );
        if( !drawable )
            continue;

        ev = drawable->RenderImage(
              iCtx
            , *Block()
            , iFrame
            , FRectI::Auto
            , FVec2F( 0 )
            , FSchedulePolicy::MultiScanlines
            , 1
            , &ev
        );
    }

    ValidateImageCache(iFrame);
    return  ev;
}

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
    FEvent ev = RenderImageCache( iCtx, iFrame );
    ulError err = iCtx.Blend(
          *Block()
        , ioBlock
        , iRect
        , iPos
        , BlendMode()
        , AlphaMode()
        , Opacity()
        , iPolicy
        , iNumWait
        , iWaitList
        , &ev
    );
    ULIS_ASSERT( !err, "Error during layer folder blend" );
    return  ev;
}

// TRasterizable Interface
TEMPLATE
typename CLASS::tSiblingImage*
CLASS::Rasterize( FContext& iCtx, FEvent* oEvent ) // override
{

    // Actual Deep Copy with Event.
    /* tSiblingImage* rasterized = new tSiblingImage(
        Name()
        , IsLocked()
        , IsVisible() 
        , PrettyColor()
        , ref->Width()
        , ref->Height()
        , ref->Format()
        , ref->ColorSpace()
        , BlendMode()
        , AlphaMode()
        , Opacity()
        , false
        , nullptr

        , FOnNameChanged::GetDelegate()
        , FOnBoolChanged::GetDelegate()
        , FOnBoolChanged::GetDelegate()
        , FOnColorChanged::GetDelegate()
        , FOnUserDataAdded::GetDelegate()
        , FOnUserDataChanged::GetDelegate()
        , FOnUserDataRemoved::GetDelegate()
        , FOnParentChanged::GetDelegate()
        , FOnSelfChanged::GetDelegate()

        , TOnBlockChanged< BlockType >::GetDelegate()
        , FOnBlendInfoChanged::GetDelegate()
        , FOnBoolChanged::GetDelegate()
    );

    uint64 startBoundary = StartBoundary();
    uint64 endBoundary = EndBoundary();
    for (uint64 i = startBoundary; i <= endBoundary; i++)
    {
        FEvent ev = RenderImageCache( iCtx, i );
        BlockType* ref = Block();
        if( !ref )
            return  nullptr;

        iCtx.Copy( *Block(), *( rasterized->Block() ), FRectI::Auto, FVec2I( 0 ), FSchedulePolicy::CacheEfficient, 1, &ev, oEvent );
    }
    return  rasterized; */

    return nullptr; //TODO:
}

// TNode< IAnimatedLayer > Interface
TEMPLATE
void
CLASS::InitFromParent( const TRoot< IAnimatedLayer >* iParent ) // override
{
    ULIS_ASSERT( iParent == Parent(), "Inconsistent Parent" );
    const tParent* topLevel = iParent->TopLevelParent();
    if( !topLevel )
        return;

    if( !Block() ) {
        const ILayer* layer = dynamic_cast< const IAnimatedLayer* >( topLevel );
        ULIS_ASSERT( layer, "Parent cannot be cast to IAnimatedLayer, there's something wrong with the class hierarchy !" );
        switch( layer->TypeID() ) {
            case LayerStackType::StaticTypeID(): {
                const LayerStackType* stack = dynamic_cast< const LayerStackType* >( layer );
                ULIS_ASSERT( stack, "Parent cannot be cast to stack, this is inconsistent with the StaticTypeID !" );
                Realloc( stack->Width(), stack->Height(), stack->Format(), stack->ColorSpace() );
                break;
            }
            case tSelf::StaticTypeID(): {
                const tSelf* folder = dynamic_cast< const tSelf* >( layer );
                ULIS_ASSERT( folder, "Parent cannot be cast to folder, this is inconsistent with the StaticTypeID !" );
                const BlockType* ref = folder->Block();
                if( !ref )
                    break;
                Realloc( ref->Width(), ref->Height(), ref->Format(), ref->ColorSpace() );
                break;
            }
        }
    }

    TRoot< IAnimatedLayer >::InitFromParent( iParent );
}

ULIS_NAMESPACE_END

// Template Macro Utility
#undef TEMPLATE
#undef CLASS

