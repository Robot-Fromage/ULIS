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
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
FLayerFolder::~FLayerFolder()
{
    Reset();
}

FLayerFolder::FLayerFolder( const FString& iName, uint16 iWidth, uint16 iHeight, eFormat iFormat )
    : ILayer( Layer_Folder, iName )
    , mLayers()
    , mFolderBlock( nullptr )
{
    mFolderBlock = new FBlock( iWidth, iHeight, iFormat );
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
FLayerFolder::AddLayer( ILayer* iLayer, int iIndex )
{
    mLayers.Insert( iIndex, iLayer );
}

void
FLayerFolder::RemoveLayer( int iIndex )
{
    mLayers.Erase( iIndex, 1 );
}

void
FLayerFolder::Reset()
{
    for( uint64 i = 0; i < mLayers.Size(); ++i ) {
        delete  mLayers[i];
    }
    mLayers.Clear();
}

FBlock&
FLayerFolder::FolderBlock()
{
    return  *mFolderBlock;
}

const FBlock&
FLayerFolder::FolderBlock() const
{
    return  *mFolderBlock;
}

ULIS_NAMESPACE_END

