// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerRoot.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the ILayerRoot class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/LayerRoot.h"
#include "Image/Block.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
ILayerRoot::~ILayerRoot()
{
    Reset();
}

ILayerRoot::ILayerRoot( const FString& iName, ILayerRoot* iParent )
    : ILayer(
          iName
        , iParent
    )
    , mLayers()
{
}

eLayerType
ILayerRoot::Type() const
{
    return  Layer_Root;
}

TArray< ILayer* >&
ILayerRoot::Layers()
{
    return  mLayers;
}

const TArray< ILayer* >&
ILayerRoot::Layers() const
{
    return  mLayers;
}

ILayerRoot&
ILayerRoot::AddLayer( ILayer* iLayer, uint64 iIndex )
{
    iLayer->SetParent( this );

    if( iIndex >= mLayers.Size() )
        mLayers.PushBack( iLayer );
    else
        mLayers.Insert( iIndex, iLayer );

    return *this;
}

ILayerRoot&
ILayerRoot::AddLayer( ILayer& iLayer, uint64 iIndex )
{
    return  AddLayer( &iLayer, iIndex );
}

void
ILayerRoot::DeleteLayer( int iIndex )
{
    delete mLayers[ iIndex ];
    mLayers.Erase( iIndex, 1 );
}

void
ILayerRoot::Reset()
{
    for( uint64 i = 0; i < mLayers.Size(); ++i ) {
        delete  mLayers[i];
    }
    mLayers.Clear();
}

ULIS_NAMESPACE_END

