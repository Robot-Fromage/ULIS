// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerRoot.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ILayerRoot class.
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
/// @class      ILayerRoot
/// @brief      The ILayerRoot class provides a class to store a folder of
///             layers in a layer stack for painting applications.
class ULIS_API ILayerRoot
    : public virtual ILayer
{
protected:
    virtual ~ILayerRoot() override;
    ILayerRoot( const FString& iName, ILayerRoot* iParent = nullptr );

    ILayerRoot( const ILayerRoot& ) = delete;
    ILayerRoot& operator=( const ILayerRoot& ) = delete;

public:
    virtual eLayerType Type() const override;
    TArray< ILayer* >& Layers();
    const TArray< ILayer* >& Layers() const;
    ILayerRoot& AddLayer( ILayer* iLayer, uint64 iIndex = ULIS_UINT64_MAX );
    ILayerRoot& AddLayer( ILayer& iLayer, uint64 iIndex = ULIS_UINT64_MAX );
    void DeleteLayer( int iIndex );
    void Reset();

private:
    TArray< ILayer* > mLayers;
};

ULIS_NAMESPACE_END

