// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerImage.tpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the TLayerImage class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Layer/LayerImage.h"

// Template Macro Utility
#define TEMPLATE template< class BlockType, class RasterizerType, class RendererType, class BlockAllocatorType >
#define CLASS TLayerImage< BlockType, RasterizerType, RendererType, BlockAllocatorType >

ULIS_NAMESPACE_BEGIN
TEMPLATE
CLASS::~TLayerImage() {
}

TEMPLATE
CLASS::TLayerImage(
      const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iAlphaLocked
    , TRoot< ILayer >* iParent
)
    : tAbstractLayerDrawable(
          iName
        , iLocked
        , iVisible
        , iPrettyColor
        , iParent
    )
    , tRasterizable()
    , tHasBlock(
        ( iWidth && iHeight ) ? BlockAllocatorType::New( iWidth, iHeight, iFormat ) : nullptr )
    , IHasBlendInfo(
          iBlendMode
        , iAlphaMode
        , iOpacity
    )
    , IHasPaintLock( iAlphaLocked )
{
}

TEMPLATE
CLASS::TLayerImage(
      BlockType* iBlock
    , const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iAlphaLocked
    , TRoot< ILayer >* iParent
)
    : tAbstractLayerDrawable(
          iName
        , iLocked
        , iVisible
        , iPrettyColor
        , iParent
    )
    , tRasterizable()
    , tHasBlock( iBlock )
    , IHasBlendInfo(
          iBlendMode
        , iAlphaMode
        , iOpacity
    )
    , IHasPaintLock( iAlphaLocked )
{
}

// TDrawable Interface
TEMPLATE
FEvent
CLASS::RenderImage(
      FContext& iCtx
    , BlockType& ioBlock
    , const FRectI& iRect = FRectI::Auto
    , const FVec2I& iPos = FVec2I( 0 )
    , const FSchedulePolicy& iPolicy = FSchedulePolicy()
    , uint32 iNumWait = 0
    , const FEvent* iWaitList = nullptr
) // override
{
    RenderCache( iCtx );
    FEvent ev;
    ulError err = iCtx.Blend(
          *mBlock
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

// TNode< ILayer > Interface
TEMPLATE
void
CLASS::InitFromParent( const TRoot< ILayer >* iParent ) {
    const tParent* topLevel = iParent->TopLevelParent();
    if( !topLevel )
        return;

    if( !Block() ) {
        const ILayer* layer = dynamic_cast< const ILayer* >( topLevel );
        ULIS_ASSERT( layer, "Parent cannot be cast to ILayer, there's something wrong with the class hierarchy !" );
        uint32 typeID = layer->TypeID();
        switch( typeID ) {
            case tSiblingStack::StaticTypeID(): {
                const tSiblingStack* stack = dynamic_cast< const tSiblingStack* >( layer );
                ULIS_ASSERT( stack, "Parent cannot be cast to stack, this is inconsistent with the StaticTypeID !" );
                mBlock = BlockAllocatorType::New( stack->Width(), stack->Height(), stack->Format() );
                break;
            }
            case tSiblingFolder::StaticTypeID(): {
                const tSiblingFolder* folder = dynamic_cast< const tSiblingFolder* >( layer );
                ULIS_ASSERT( folder, "Parent cannot be cast to folder, this is inconsistent with the StaticTypeID !" );
                const BlockType& ref = folder->Block();
                mBlock = BlockAllocatorType::New( ref.Width(), ref.Height(), ref.Format() );
                break;
            }
        }
    }
}

ULIS_NAMESPACE_END

