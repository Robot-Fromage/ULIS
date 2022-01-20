// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerImage.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerImage FLayerImage.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Layer/Layer/LayerImage.h"
#include "Layer/Layer/LayerFolder.h"
#include "Layer/Layer/LayerStack.h"

ULIS_NAMESPACE_BEGIN

FLayerImage::~FLayerImage() {
    ULIS_DEBUG_PRINTF( "FLayerImage Destroyed" )
}


FLayerImage::FLayerImage(
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
    , bool iAlphaLocked
    , const TRoot< ILayer >* iParent

    , const FOnNameChanged& iOnNameChanged
    , const FOnBoolChanged& iOnLockChanged
    , const FOnBoolChanged& iOnVisibleChanged
    , const FOnColorChanged& iOnColorChanged
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
    , const FOnParentChanged& iOnParentChanged
    , const FOnSelfChanged& iOnSelfChanged

    , const TOnBlockChanged< FBlock >& iOnBlockChanged
    , const FOnBlendInfoChanged& iOnBlendInfoChanged
    , const FOnBoolChanged& iOnPaintLockChanged
)
    : TNode< ILayer >(
          iParent
        , iOnParentChanged
        , iOnSelfChanged
    )
    , ILayer(
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
    , THasBlock< FBlock, FRegularBlockAllocator >(
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
    , IHasPaintLock(
          iAlphaLocked
        , iOnPaintLockChanged
    )
{
    ULIS_DEBUG_PRINTF( "FLayerImage Created" )
}


FLayerImage::FLayerImage(
      FBlock* iBlock
    , const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iAlphaLocked
    , const TRoot< ILayer >* iParent

    , const FOnNameChanged& iOnNameChanged
    , const FOnBoolChanged& iOnLockChanged
    , const FOnBoolChanged& iOnVisibleChanged
    , const FOnColorChanged& iOnColorChanged
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
    , const FOnParentChanged& iOnParentChanged
    , const FOnSelfChanged& iOnSelfChanged

    , const TOnBlockChanged< FBlock >& iOnBlockChanged
    , const FOnBlendInfoChanged& iOnBlendInfoChanged
    , const FOnBoolChanged& iOnPaintLockChanged
)
    : TNode< ILayer >(
          iParent
        , iOnParentChanged
        , iOnSelfChanged
    )
    , ILayer(
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
    , THasBlock< FBlock, FRegularBlockAllocator >(
          iBlock
        , iOnBlockChanged
    )
    , IHasBlendInfo(
          iBlendMode
        , iAlphaMode
        , iOpacity
        , iOnBlendInfoChanged
    )
    , IHasPaintLock(
          iAlphaLocked
        , iOnPaintLockChanged
    )
{
    ULIS_DEBUG_PRINTF( "FLayerImage Created" )
}

/*
// TDrawable Interface
FEvent
FLayerImage::RenderImage(
      FContext& iCtx
    , FBlock& ioBlock
    , const FRectI& iRect
    , const FVec2I& iPos
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
) // override
{
    FEvent ev;
    ulError err = iCtx.Blend(
          *tSelf::Block()
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
    ULIS_ASSERT( !err, "Error during layer image blend" );
    return  ev;
}

// TRasterizable Interface
typename FLayerImage::tSelf*
FLayerImage::Rasterize( FContext& iCtx, FEvent* oEvent ) // override
{
    const FBlock* ref = tSelf::Block();
    if( !ref )
        return  nullptr;

    // Actual Deep Copy with Event.
    tSelf* rasterized = new tSelf(
          tSelf::Name()
        , tSelf::IsLocked()
        , tSelf::IsVisible()
        , tSelf::PrettyColor()
        , ref->Width()
        , ref->Height()
        , ref->Format()
        , ref->ColorSpace()
        , BlendMode()
        , AlphaMode()
        , Opacity()
        , IsPaintLocked()
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

        , TOnBlockChanged< FBlock >::GetDelegate()
        , FOnBlendInfoChanged::GetDelegate()
        , FOnBoolChanged::GetDelegate()
    );

    iCtx.Copy( *tSelf::Block(), *(rasterized->Block()), FRectI::Auto, FVec2I( 0 ), FSchedulePolicy::CacheEfficient, 0, nullptr, oEvent );
    return  rasterized;
}
*/

// TNode< ILayer > Interface

void
FLayerImage::InitFromParent( const TRoot< ILayer >* iParent ) // override
{
    ULIS_ASSERT( iParent == Parent(), "Inconsistent Parent" );
    const tParent* topLevel = iParent->TopLevelParent();
    if( !topLevel )
        return;

    if( !FLayerImage::Block() ) {
        const ILayer* layer = dynamic_cast< const ILayer* >( topLevel );
        //const ILayer* layer = (const ILayer*)( topLevel ); // Unsafe !
        ULIS_ASSERT( layer, "Parent cannot be cast to ILayer, there's something wrong with the FLayerImage hierarchy !" );
        switch( layer->TypeID() ) {
            case FLayerStack::StaticTypeID(): {
                const FLayerStack* stack = dynamic_cast< const FLayerStack* >( layer );
                ULIS_ASSERT( stack, "Parent cannot be cast to stack, this is inconsistent with the StaticTypeID !" );
                Realloc( stack->Width(), stack->Height(), stack->Format(), stack->ColorSpace() );
                break;
            }
            case FLayerFolder::StaticTypeID(): {
                const FLayerFolder* folder = dynamic_cast< const FLayerFolder* >( layer );
                ULIS_ASSERT( folder, "Parent cannot be cast to folder, this is inconsistent with the StaticTypeID !" );
                ULIS_ASSERT( false, "Cannot init from parent in orphan folder, we need height width format info !" );
                //const FBlock* ref = folder->Block();
                //if( !ref )
                //    break;
                //Realloc( ref->Width(), ref->Height(), ref->Format(), ref->ColorSpace() );
                //break;
            }
        }
    }
}

ULIS_NAMESPACE_END

