// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Layer.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ILayer class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Callback.h"
#include "Layer/Components/HasLock.h"
#include "Layer/Components/HasName.h"
#include "Layer/Components/HasPrettyColor.h"
#include "Layer/Components/HasUserData.h"
#include "Layer/Components/HasVisibility.h"
#include "Layer/Components/TypeIdentifiable.h"
#include "Memory/Tree.h"

ULIS_NAMESPACE_BEGIN
// Forward Declarations
class ILayer;

// Exports
template class ULIS_API TNode< ILayer >;
template class ULIS_API TArray< TNode< ILayer >* >;
template class ULIS_API TRoot< ILayer >;

ULIS_DECLARE_SIMPLE_DELEGATE( FOnLayerCreated, void, ILayer* )
ULIS_DECLARE_SIMPLE_DELEGATE( FOnLayerDeleted, void, ILayer* )
ULIS_DECLARE_SIMPLE_DELEGATE( FOnLayerParentChanged, void, ILayer*, const TRoot< ILayer >* )
ULIS_DECLARE_SIMPLE_DELEGATE( FOnChildAdded, void, TRoot< ILayer >*, ILayer* )
ULIS_DECLARE_SIMPLE_DELEGATE( FOnLayerNameChanged, void, ILayer*, const FString& )
ULIS_DECLARE_SIMPLE_DELEGATE( FOnLayerPrettyColorChanged, void, ILayer*, const FColor& )
ULIS_DECLARE_SIMPLE_DELEGATE( FOnLayerVisibilityChanged, void, ILayer*, bool )
ULIS_DECLARE_SIMPLE_DELEGATE( FOnLayerLockChanged, void, ILayer*, bool )
ULIS_DECLARE_SIMPLE_DELEGATE( FOnLayerUserDataChanged, void, ILayer*, const FColor& )

/////////////////////////////////////////////////////
/// @class      ILayer
/// @brief      The ILayer class provides a base abstract class to store a layer
///             in a layer stack for painting applications.
class ULIS_API ILayer
    : public IHasLock
    , public IHasName
    , public IHasPrettyColor
    , public IHasVisibility
    , public IHasUserData
    , public ITypeIdentifiable
    , public virtual TNode< ILayer >
{
public:
    // DTor
    virtual ~ILayer() override = 0;

    // CTor
    ILayer(
          const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iPrettyColor = FColor::Transparent
        , const TRoot< ILayer >* iParent = nullptr
    );
};

ULIS_NAMESPACE_END

