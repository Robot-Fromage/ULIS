// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerFolder.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerFolder FLayerFolder.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Layer/LayerFolder.h"
#include "Layer/Layer/LayerStack.h"

ULIS_NAMESPACE_BEGIN

FLayerFolder::~FLayerFolder() {
    ULIS_DEBUG_PRINTF( "FLayerFolder Destroyed" )
}


FLayerFolder::FLayerFolder(
      const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iCollapsed
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
    , const FOnNodeAdded& iOnLayerAdded
    , const FOnNodeRemoved& iOnLayerRemoved

    , const FOnBlendInfoChanged& iOnBlendInfoChanged
    , const FOnBoolChanged& iOnCollapseChanged
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
    ULIS_DEBUG_PRINTF( "FLayerFolder Created" )
}

/*
// TDrawable Interface
FEvent
FLayerFolder::RenderImageCache( FContext& iCtx ) // override
{
    if( tSelf::IsImageCacheValid() )
        return  FEvent::NoOP();

    FEvent ev;
    iCtx.Clear( *tSelf::Block(), FRectI::Auto, FSchedulePolicy::CacheEfficient, 0, nullptr, &ev );
    const int max = static_cast< int >( Children().Size() ) - 1;
    for( int i = max; i >= 0; --i ) {
        tAbstractLayerDrawable* drawable = dynamic_cast< tAbstractLayerDrawable* >( &( Children()[i]->Self() ) );
        if( !drawable )
            continue;

        ev = drawable->RenderImage(
              iCtx
            , *tSelf::Block()
            , FRectI::Auto
            , FVec2F( 0 )
            , FSchedulePolicy::MultiScanlines
            , 1
            , &ev
        );
    }

    tSelf::ValidateImageCache();
    return  ev;
}


FEvent
FLayerFolder::RenderImage(
      FContext& iCtx
    , BlockType& ioBlock
    , const FRectI& iRect
    , const FVec2I& iPos
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
) // override
{
    FEvent ev;
    TArray< FEvent > events( iNumWait + 1 );
    for( uint32 i = 0; i < iNumWait; ++i )
        events[i] = iWaitList[i];
    events[ iNumWait ] = RenderImageCache( iCtx );

    ulError err = iCtx.Blend(
          *tHasBlock::Block()
        , ioBlock
        , iRect
        , iPos
        , BlendMode()
        , AlphaMode()
        , Opacity()
        , iPolicy
        , iNumWait + 1
        , &events[0]
        , &ev
    );
    ULIS_ASSERT( !err, "Error during layer folder blend" );
    return  ev;
}

// TRasterizable Interface

typename FLayerFolder::tSiblingImage*
FLayerFolder::Rasterize( FContext& iCtx, FEvent* oEvent ) // override
{
    FEvent ev = RenderImageCache( iCtx );
    BlockType* ref = tHasBlock::Block();
    if( !ref )
        return  nullptr;

    // Actual Deep Copy with Event.
    tSiblingImage* rasterized = new tSiblingImage(
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

    iCtx.Copy( *tSelf::Block(), *( rasterized->Block() ), FRectI::Auto, FVec2I( 0 ), FSchedulePolicy::CacheEfficient, 1, &ev, oEvent );
    return  rasterized;
}
*/

// TNode< ILayer > Interface
void
FLayerFolder::InitFromParent( const TRoot< ILayer >* iParent ) // override
{
    ULIS_ASSERT( iParent == Parent(), "Inconsistent Parent" );
    const tParent* topLevel = iParent->TopLevelParent();
    if( !topLevel )
        return;

    /*
    if( !tSelf::Block() ) {
        const ILayer* layer = dynamic_cast< const ILayer* >( topLevel );
        //const ILayer* layer = (const ILayer*)( topLevel ); // Unsafe !
        ULIS_ASSERT( layer, "Parent cannot be cast to ILayer, there's something wrong with the FLayerFolder hierarchy !" );
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
    */

    // This will call Init From Parent on all children based on TRoot implementation
    TRoot< ILayer >::InitFromParent( iParent );
}

ULIS_NAMESPACE_END

