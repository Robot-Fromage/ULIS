// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerRoot.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FLayerRoot class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Layer.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
template class ULIS_API TArray< ILayer* >;
/////////////////////////////////////////////////////
/// @class      FLayerRoot
/// @brief      The FLayerRoot class provides a class to store a folder of
///             layers in a layer stack for painting applications.
class ULIS_API FLayerRoot
    : public virtual ILayer
{
protected:
    virtual ~FLayerRoot() override;
    FLayerRoot( const FString& iName, FLayerRoot* iParent );

    FLayerRoot( const FLayerRoot& ) = delete;
    FLayerRoot& operator=( const FLayerRoot& ) = delete;

public:
    virtual eLayerType Type() const override;
    TArray< ILayer* >& Layers();
    const TArray< ILayer* >& Layers() const;
    void AddLayer( ILayer* iLayer, uint64 iIndex );
    void RemoveLayer( int iIndex );
    void Reset();

private:
    TArray< ILayer* > mLayers;
};

ULIS_NAMESPACE_END

