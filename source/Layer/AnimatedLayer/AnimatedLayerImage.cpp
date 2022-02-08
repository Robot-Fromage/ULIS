// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerImage.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FAnimatedLayerImage class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/AnimatedLayer/AnimatedLayerImage.h"
#include "Layer/AnimatedLayer/AnimatedLayerFolder.h"
#include "Layer/AnimatedLayer/AnimatedLayerStack.h"

ULIS_NAMESPACE_BEGIN
FAnimatedLayerImage::~FAnimatedLayerImage() {
    ULIS_DEBUG_PRINTF( "FAnimatedLayerImage Destroyed" )
}

FAnimatedLayerImage::FAnimatedLayerImage(
      const FString& iName
    , bool iLocked
    , bool iTimeLocked
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
    , const TRoot< IAnimatedLayer >* iParent

    , const FOnNameChanged& iOnNameChanged
    , const FOnBoolChanged& iOnLockChanged
    , const FOnBoolChanged& iOnTimeLockChanged
    , const FOnBoolChanged& iOnVisibleChanged
    , const FOnColorChanged& iOnColorChanged
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
    , const FOnAnimatedParentChanged& iOnParentChanged
    , const FOnAnimatedSelfChanged& iOnSelfChanged

    , const FOnBlendInfoChanged& iOnBlendInfoChanged
    , const FOnBoolChanged& iOnPaintLockChanged
)
    : TNode< IAnimatedLayer >(
          iParent
        , iOnParentChanged
        , iOnSelfChanged
    )
    , IAnimatedLayer(
          iName
        , iLocked
        , iTimeLocked
        , iVisible
        , iPrettyColor
        , iParent

        , iOnNameChanged
        , iOnLockChanged
        , iOnTimeLockChanged
        , iOnVisibleChanged
        , iOnColorChanged
        , iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
        , iOnParentChanged
        , iOnSelfChanged
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
    , TSequence< FBlock, FCelBlockFactory >(
          iWidth
        , iHeight
        , iFormat
        , iColorSpace
    )
{
    ULIS_DEBUG_PRINTF( "FAnimatedLayerImage Created" )
}

// TNode< IAnimatedLayer > Interface
void
FAnimatedLayerImage::InitFromParent( const TRoot< IAnimatedLayer >* iParent ) // override
{
    ULIS_ASSERT( iParent == Parent(), "Inconsistent Parent" );
    const tParent* topLevel = iParent->TopLevelParent();
    if( !topLevel )
        return;

    const IAnimatedLayer* layer = dynamic_cast< const IAnimatedLayer* >( topLevel );
    ULIS_ASSERT( layer, "Parent cannot be cast to IAnimatedLayer, there's something wrong with the FLayerImage hierarchy !" );
    switch( layer->TypeID() ) {
        case FAnimatedLayerStack::StaticTypeID(): {
            const FAnimatedLayerStack* stack = dynamic_cast< const FAnimatedLayerStack* >( layer );
            ULIS_ASSERT( stack, "Parent cannot be cast to stack, this is inconsistent with the StaticTypeID !" );
            TSequence< FBlock, FCelBlockFactory >::ReinterpretFormat( stack->Format() );
            TSequence< FBlock, FCelBlockFactory >::AssignColorSpace( stack->ColorSpace() );
            TSequence< FBlock, FCelBlockFactory >::ReinterpretSize( FVec2UI16( stack->Width(), stack->Height() ) );
            TSequence< FBlock, FCelBlockFactory >::Reset();
            break;
        }
        case FAnimatedLayerFolder::StaticTypeID(): {
            const FAnimatedLayerFolder* folder = dynamic_cast< const FAnimatedLayerFolder* >( layer );
            ULIS_ASSERT( folder, "Parent cannot be cast to folder, this is inconsistent with the StaticTypeID !" );
            ULIS_WARNING( false, "Init from orphan folder can be dangerous but is okay if it's only temporary during build." );
        }
    }
}

uint64
FAnimatedLayerImage::GetNumFrames() const {
    return  TotalFrames();
}

ULIS_NAMESPACE_END

