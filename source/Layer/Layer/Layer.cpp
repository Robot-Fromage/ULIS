// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Layer.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the ILayer class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Layer/Layer.h"

ULIS_NAMESPACE_BEGIN
ILayer::~ILayer()
{
    ULIS_DEBUG_PRINTF( "ILayer Destroyed" )
}

ILayer::ILayer(
      const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iPrettyColor
    , const TRoot< ILayer >* iParent

    , const FOnNameChanged& iOnNameChanged
    , const FOnBoolChanged& iOnLockChanged
    , const FOnBoolChanged& iOnVisibleChanged
    , const FOnColorChanged& iOnColorChanged
    , const FOnUserDataAdded& iOnUserDataAdded
    , const FOnUserDataChanged& iOnUserDataChanged
    , const FOnUserDataRemoved& iOnUserDataRemoved
    , const FOnLayerParentChanged& iOnParentChanged
    , const FOnLayerSelfChanged& iOnSelfChanged
)
    : TNode< ILayer >(
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
    ULIS_DEBUG_PRINTF( "ILayer Created" )
}

ULIS_NAMESPACE_END

