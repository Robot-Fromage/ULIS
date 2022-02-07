// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerVector.cpp
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "LayerVector.h"

FLayerVector::~FLayerVector() {
    ULIS_DEBUG_PRINTF( "FLayerVector Destroyed" )
}

// CTors
FLayerVector::FLayerVector(
      const ::ULIS::FString& iName
    , bool iLocked
    , bool iVisible
    , const ::ULIS::FColor& iPrettyColor
    , ::ULIS::eBlendMode iBlendMode
    , ::ULIS::eAlphaMode iAlphaMode
    , ::ULIS::ufloat iOpacity
    , const ::ULIS::TRoot< ILayer >* iParent

    , const ::ULIS::FOnNameChanged& iOnNameChanged
    , const ::ULIS::FOnBoolChanged& iOnLockChanged
    , const ::ULIS::FOnBoolChanged& iOnVisibleChanged
    , const ::ULIS::FOnColorChanged& iOnColorChanged
    , const ::ULIS::FOnUserDataAdded& iOnUserDataAdded
    , const ::ULIS::FOnUserDataChanged& iOnUserDataChanged
    , const ::ULIS::FOnUserDataRemoved& iOnUserDataRemoved
    , const ::ULIS::FOnParentChanged& iOnParentChanged
    , const ::ULIS::FOnSelfChanged& iOnSelfChanged

    , const ::ULIS::FOnBlendInfoChanged& iOnBlendInfoChanged
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
{
    ULIS_DEBUG_PRINTF( "FLayerVector Created" )
}

FGroupVectorShape&
FLayerVector::VectorData() {
    return  mVectorData;
}

const FGroupVectorShape&
FLayerVector::VectorData() const {
    return  mVectorData;
}

