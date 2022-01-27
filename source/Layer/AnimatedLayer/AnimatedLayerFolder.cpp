// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerFolder.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FAnimatedLayerFolder class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/AnimatedLayer/AnimatedLayerFolder.h"

ULIS_NAMESPACE_BEGIN
FAnimatedLayerFolder::~FAnimatedLayerFolder() {
    ULIS_DEBUG_PRINTF( "FAnimatedLayerFolder Destroyed" )
}

FAnimatedLayerFolder::FAnimatedLayerFolder(
      const FString& iName
    , bool iLocked
    , bool iTimeLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iCollapsed
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
    , const FOnAnimatedNodeAdded& iOnLayerAdded
    , const FOnAnimatedNodeRemoved& iOnLayerRemoved

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
    , IHasCollapse(
          iCollapsed
        , iOnCollapseChanged
    )
{
    ULIS_DEBUG_PRINTF( "FAnimatedLayerFolder Created" )
}

// TNode< ILayer > Interface
void
FAnimatedLayerFolder::InitFromParent( const TRoot< IAnimatedLayer >* iParent ) // override
{
    // Override to avoid ambiguous with virtual inheritance.
    // This will call Init From Parent on all children based on TRoot implementation.
    TRoot< IAnimatedLayer >::InitFromParent( iParent );
}

ULIS_NAMESPACE_END

