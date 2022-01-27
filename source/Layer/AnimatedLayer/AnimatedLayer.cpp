// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayer.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IAnimatedLayer class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/AnimatedLayer/AnimatedLayer.h"

ULIS_NAMESPACE_BEGIN
IAnimatedLayer::~IAnimatedLayer()
{
    ULIS_DEBUG_PRINTF( "IAnimatedLayer Destroyed" )
}

IAnimatedLayer::IAnimatedLayer(
      const FString& iName
    , bool iLocked
    , bool iTimeLocked
    , bool iVisible
    , const FColor& iPrettyColor
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
)
    : TNode< IAnimatedLayer >(
          iParent
        , iOnParentChanged
        , iOnSelfChanged
    )
    , ITypeIdentifiable()
    , IHasUserData(
          iOnUserDataAdded
        , iOnUserDataChanged
        , iOnUserDataRemoved
    )
    , IHasName(
          iName
        , iOnNameChanged
    )
    , IHasLock(
          iLocked
        , iOnLockChanged
    )
    , IHasVisibility(
          iVisible
        , iOnVisibleChanged
    )
    , IHasPrettyColor(
          iPrettyColor
        , iOnColorChanged
    )
{
    ULIS_DEBUG_PRINTF( "IAnimatedLayer Created" )
}

ULIS_NAMESPACE_END

