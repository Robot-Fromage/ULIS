// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerFolder.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerFolder class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/LayerFolder.h"

ULIS_NAMESPACE_BEGIN

FLayerFolder::~FLayerFolder()
{
}

FLayerFolder::FLayerFolder( const FString& iName )
    : ILayer( Layer_Folder, iName )
    , mLayers()
{
}

TArray< ILayer* >&
FLayerFolder::Layers()
{
    return  mLayers;
}

const TArray< ILayer* >&
FLayerFolder::Layers() const
{
    return  mLayers;
}

void
FLayerFolder::AddLayer( ILayer* iLayer, int iIndex = -1 )
{
    mLayers.Insert( iIndex, iLayer );
}

void
FLayerFolder::RemoveLayer( int iIndex )
{
    mLayers.Erase( iIndex, 1 );
}

ULIS_NAMESPACE_END

