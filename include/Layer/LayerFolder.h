// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerFolder.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FLayerFolder class.
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
/// @class      FLayerFolder
/// @brief      The FLayerFolder class provides a class to store a folder of
///             layers in a layer stack for painting applications.
class ULIS_API FLayerFolder
    : public ILayer
{
public:
    ~FLayerFolder() override;
    FLayerFolder( const FString& iName );

    FLayerFolder( const FLayerFolder& ) = delete;
    FLayerFolder& operator=( const FLayerFolder& ) = delete;

public:
    TArray< ILayer* >& Layers();
    const TArray< ILayer* >& Layers() const;
    void AddLayer( ILayer* iLayer, int iIndex = -1 );
    void RemoveLayer( int iIndex );
    void Reset();

private:
    TArray< ILayer* > mLayers;
};

ULIS_NAMESPACE_END

