// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerText.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerText FLayerText.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Layer/LayerText.h"

ULIS_NAMESPACE_BEGIN

FLayerText::~FLayerText() {
    ULIS_DEBUG_PRINTF( "FLayerText Destroyed" )
}


FLayerText::FLayerText(
      const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , const FWString& iText
    , const FFont& iFont
    , uint32 iFontSize
    , const ISample& iTextColor
    , bool iAntiAliased
    , const FTransformInfo& iTransform
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

    //, const TOnBlockChanged< BlockType >& iOnBlockChanged
    , const FOnBlendInfoChanged& iOnBlendInfoChanged
    , const FOnTextInfoChanged& iOnTextInfoChanged
    , const FOnTransformInfoChanged& iOnTransformInfoChanged
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
    , IHasText(
          iText
        , iFont
        , iFontSize
        , iTextColor
        , iAntiAliased
        , iOnTextInfoChanged
    )
    , IHasTransform(
          FVec2F( 0 )
        , FVec2F( 0 )
        , 0
        , FVec2F( 1 )
        , iOnTransformInfoChanged
    )
{
    ULIS_DEBUG_PRINTF( "FLayerText Created" )
}


/*
// TDrawable Interface
FEvent
FLayerText::RenderImageCache( FContext& iCtx ) // override
{
    if( tSelf::IsImageCacheValid() )
        return  FEvent::NoOP();

    FEvent eventClear;
    iCtx.Clear( *tSelf::Block(), FRectI::Auto, FSchedulePolicy::CacheEfficient, 0, nullptr, &eventClear );
    FEvent ev;
    if( IsAntiAliased() ) {
        iCtx.RasterTextAA( *tSelf::Block(), Text(), Font(), FontSize(), Matrix(), TextColor(), FSchedulePolicy::MonoChunk, 1, &eventClear, &ev );
    } else {
        iCtx.RasterText( *tSelf::Block(), Text(), Font(), FontSize(), Matrix(), TextColor(), FSchedulePolicy::MonoChunk, 1, &eventClear, &ev );
    }

    tSelf::ValidateImageCache();
    return  ev;
}


FEvent
FLayerText::RenderImage(
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
    ULIS_ASSERT( !err, "Error during layer text blend" );
    return  ev;
}

// TRasterizable Interface

typename FLayerText::tSiblingImage*
FLayerText::Rasterize( FContext& iCtx, FEvent* oEvent ) // override
{
    const BlockType* ref = tSelf::Block();
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

    iCtx.Copy( *tSelf::Block(), *(rasterized->Block()), FRectI::Auto, FVec2I( 0 ), FSchedulePolicy::CacheEfficient, 0, nullptr, oEvent );
    return  rasterized;
}
*/

// TNode< ILayer > Interface
void
FLayerText::InitFromParent( const TRoot< ILayer >* iParent ) // override
{
    ULIS_ASSERT( iParent == Parent(), "Inconsistent Parent" );
    const tParent* topLevel = iParent->TopLevelParent();
    if( !topLevel )
        return;

    /*
    if( !tSelf::Block() ) {
        const ILayer* layer = dynamic_cast< const ILayer* >( topLevel );
        //const ILayer* layer = (const ILayer*)( topLevel ); // Unsafe !
        ULIS_ASSERT( layer, "Parent cannot be cast to ILayer, there's something wrong with the FLayerText hierarchy !" );
        switch( layer->TypeID() ) {
            case LayerStackType::StaticTypeID(): {
                const LayerStackType* stack = dynamic_cast< const LayerStackType* >( layer );
                ULIS_ASSERT( stack, "Parent cannot be cast to stack, this is inconsistent with the StaticTypeID !" );
                Realloc( stack->Width(), stack->Height(), stack->Format(), stack->ColorSpace() );
                break;
            }
            case tSiblingFolder::StaticTypeID(): {
                const tSiblingFolder* folder = dynamic_cast< const tSiblingFolder* >( layer );
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
}

ULIS_NAMESPACE_END

