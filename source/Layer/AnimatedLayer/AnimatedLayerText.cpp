// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerText.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FAnimatedLayerText class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/AnimatedLayer/AnimatedLayerText.h"

ULIS_NAMESPACE_BEGIN
FAnimatedLayerText::~FAnimatedLayerText() {
    ULIS_DEBUG_PRINTF( "FAnimatedLayerText Destroyed" )
}

FAnimatedLayerText::FAnimatedLayerText(
      const FString& iName
    , bool iLocked
    , bool iTimeLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
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
{
    ULIS_DEBUG_PRINTF( "FAnimatedLayerText Created" )
}

uint64
FAnimatedLayerText::GetNumFrames() const {
    return  TotalFrames();
}

ULIS_NAMESPACE_END

