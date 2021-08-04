// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerImage.cpp
* @author       Clement Berthaud, Eric Scholl
* @brief        This file provides the definition for the TAnimatedLayerImage class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Layer/AnimatedLayer/AnimatedLayerImage.h"

// Template Macro Utility
#define TEMPLATE template< class BlockType, class RasterizerType, class RendererType, class BlockAllocatorType, class LayerStackType >
#define CLASS TAnimatedLayerImage< BlockType, RasterizerType, RendererType, BlockAllocatorType, LayerStackType >

ULIS_NAMESPACE_BEGIN
TEMPLATE
CLASS::~TAnimatedLayerImage() {
    ULIS_DEBUG_PRINTF( "TAnimatedLayerImage Destroyed" )
}

TEMPLATE
CLASS::TAnimatedLayerImage(
      const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iAlphaLocked
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

    , const TOnBlockChanged< BlockType >& iOnBlockChanged
    , const TOnSequenceChanged< BlockType >& iOnSequenceChanged
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
    , tHasSequence(
          nullptr
        , iOnSequenceChanged
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
    ULIS_DEBUG_PRINTF( "TAnimatedLayerImage Created" )
}

// TDrawable Interface
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
    FEvent ev;
    iCtx.Clear( *Block(), FRectI::Auto, FSchedulePolicy::CacheEfficient, 0, nullptr, &ev );
    uint32 currentFrame = mSequence->FirstFrame();

    if (iFrame < currentFrame)
        return ev;

    BlockType* ref = nullptr;

    for (int i = 0; i < mSequence->Cels().Size(); i++)
    {
        //TODO: manage Faux-fixe and pre/post behaviour
        if (currentFrame >= currentFrame + mSequence->Cels()[i].Exposure() )
        {
            currentFrame += mSequence->Cels()[i].Exposure();
            continue;
        }

        ref = mSequence->Cels()[i].Block();
        break;
    }

    if (!ref)
        return  ev;

    ulError err = iCtx.Blend(
          *ref
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
TEMPLATE
typename CLASS::tSelf*
CLASS::Rasterize( FContext& iCtx, FEvent* oEvent ) // override
{
    uint32 firstFrame = 0;
    const TArray<FCelInfo> celInfos = GetDrawableCelInfos(&firstFrame);

    tSiblingImage* rasterized = new tSiblingImage(
        Name()
        , IsLocked()
        , IsVisible() 
        , PrettyColor()
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

    rasterized->InitFromParent(Parent());

    TSequence<BlockType>* sequence = rasterized->Sequence();

    sequence->FirstFrame(firstFrame);
    uint32 currentFrame = firstFrame;
    for (int i = 0; i < celInfos.Size(); i++)
    {
        sequence->AddCel(celInfos[i]);
        //This is not optimal, as RenderImage will run through mSequence->Cels()
        //We should directly navigate mSequence here instead  
        FEvent ev = RenderImage( iCtx, *( sequence->Cels()[i]->Block() ), currentFrame, FRectI::Auto, FVec2I( 0 ), FSchedulePolicy::CacheEfficient, 1, &ev, oEvent);
        currentFrame += celInfos[i].Exposure();
    }
}

TEMPLATE
const TArray<FCelInfo>
CLASS::GetDrawableCelInfos(uint32* oFirstFrame) const
{
    *oFirstFrame = Sequence()->FirstFrame();
    TArray<FCelInfo> celInfos;
    for (int i = 0; i < Sequence()->Cels().Size(); i++)
        celInfos.PushBack(Sequence()->Cels()[i].Info());

    return celInfos;
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
        const IAnimatedLayer* layer = dynamic_cast< const IAnimatedLayer* >( topLevel );
        ULIS_ASSERT( layer, "Parent cannot be cast to IAnimatedLayer, there's something wrong with the class hierarchy !" );
        switch( layer->TypeID() ) {
            case LayerStackType::StaticTypeID(): {
                const LayerStackType* stack = dynamic_cast< const LayerStackType* >( layer );
                ULIS_ASSERT( stack, "Parent cannot be cast to stack, this is inconsistent with the StaticTypeID !" );
                Realloc( stack->Width(), stack->Height(), stack->Format(), stack->ColorSpace() );
                ReallocSequence( stack->Width(), stack->Height(), stack->Format(), stack->ColorSpace() );
                break;
            }
            case tSiblingFolder::StaticTypeID(): {
                const tSiblingFolder* folder = dynamic_cast< const tSiblingFolder* >( layer );
                ULIS_ASSERT( folder, "Parent cannot be cast to folder, this is inconsistent with the StaticTypeID !" );
                const BlockType* ref = folder->Block();
                if( !ref )
                    break;
                Realloc( ref->Width(), ref->Height(), ref->Format(), ref->ColorSpace() );
                ReallocSequence( stack->Width(), stack->Height(), stack->Format(), stack->ColorSpace() );
                break;
            }
        }
    }
}

ULIS_NAMESPACE_END

// Template Macro Utility
#undef TEMPLATE
#undef CLASS

