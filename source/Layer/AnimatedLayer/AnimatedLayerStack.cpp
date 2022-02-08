// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerStack.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FAnimatedLayerStack class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/AnimatedLayer/AnimatedLayerStack.h"

ULIS_NAMESPACE_BEGIN
// DTor
FAnimatedLayerStack::~FAnimatedLayerStack()
{
    ULIS_DEBUG_PRINTF( "FAnimatedLayerStack Destroyed" )
}

// CTor
FAnimatedLayerStack::FAnimatedLayerStack(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , uint16 iFps
    , int64 iBeginFrame
    , int64 iEndFrame
    , int64 iCurrentFrame
    , const FOnTimeSettingsChanged& iOnTimeSettingsChanged
    , const FOnAnimatedSelfChanged& iOnSelfChanged
    , const FOnAnimatedNodeAdded& iOnLayerAdded
    , const FOnAnimatedNodeRemoved& iOnLayerRemoved
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
)
    : TNode< IAnimatedLayer >(
          nullptr
        , FOnAnimatedParentChanged()
        , iOnSelfChanged
    )
    , IAnimatedLayer(
          "AnimatedLayerStack"
        , false
        , false
        , true
        , FColor::Transparent
        , nullptr
        , FOnNameChanged()
        , FOnBoolChanged()
        , FOnBoolChanged()
        , FOnBoolChanged()
        , FOnColorChanged()
        , iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
        , FOnAnimatedParentChanged()
        , iOnSelfChanged
    )
    , TRoot< IAnimatedLayer > (
          nullptr
        , FOnAnimatedParentChanged()
        , iOnSelfChanged
        , iOnLayerAdded
        , iOnLayerRemoved
    )
    , TSearchable< TRoot< IAnimatedLayer > >()
    , IHasSize2D( FVec2UI16( iWidth, iHeight ) )
    , IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , IHasTimeSettings(
          iFps
        , iBeginFrame
        , iEndFrame
        , iCurrentFrame
        , iOnTimeSettingsChanged
    )
{
    ULIS_DEBUG_PRINTF( "FAnimatedLayerStack Created" )
}

// FAnimatedLayerStack Interface
void
FAnimatedLayerStack::Reset(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
    , uint16 iFps
    , int64 iBeginFrame
    , int64 iEndFrame
    , int64 iCurrentFrame
    , const FOnTimeSettingsChanged& iOnTimeSettingsChanged
    , const FOnAnimatedSelfChanged& iOnSelfChanged
    , const FOnAnimatedNodeAdded& iOnLayerAdded
    , const FOnAnimatedNodeRemoved& iOnLayerRemoved
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
)
{
    ReinterpretFormat( iFormat );
    AssignColorSpace( iColorSpace );
    ReinterpretSize( FVec2UI16( iWidth, iHeight ) );

    SetFps( iFps );
    SetBeginFrame( iBeginFrame );
    SetEndFrame( iEndFrame );
    SetCurrentFrame( iCurrentFrame );

    this->TRoot< IAnimatedLayer >::Reset(
          nullptr
        , FOnAnimatedParentChanged()
        , iOnLayerAdded
        , iOnLayerRemoved
    );
    this->FOnTimeSettingsChanged::SetDelegate( iOnTimeSettingsChanged );
    this->FOnAnimatedSelfChanged::SetDelegate( iOnSelfChanged );
    this->FOnUserDataAdded::SetDelegate( iOnUserDataAdded );
    this->FOnUserDataChanged::SetDelegate( iOnUserDataChanged );
    this->FOnUserDataRemoved::SetDelegate( iOnUserDataRemoved );
}

uint64
FAnimatedLayerStack::GetNumFrames() const {
    uint64 numFrames = 0;
    const uint64 size = Children().Size();
    for( uint64 i = 0; i < size; ++i ) {
        ::ULIS::IAnimatedLayer* layer = dynamic_cast< ::ULIS::IAnimatedLayer* >( &( Children()[i]->Self() ) );
        ULIS_ASSERT( layer, "Bad cast, shouldn't be possible!" );
        numFrames = FMath::Max( numFrames, layer->GetNumFrames() );
    }
    return  numFrames;
}

ULIS_NAMESPACE_END

