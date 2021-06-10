// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerUtils.h
* @author       Clement Berthaud
* @brief        This file provides utilities for layers.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Common/LayerUtils.h"
#include "Layer/Layer/LayerFolder.h"
#include "Layer/Layer/LayerStack.h"
#include "String/Utils.h"
#include <algorithm>
#include <tuple>
#include <vector>

ULIS_NAMESPACE_BEGIN
ILayer&
FindLayerByFuzzyNameInContainer( const FString& iStr, TRoot< ILayer >& iRoot ) {
    const uint64 size = iRoot.Children().Size();
    std::vector< std::tuple< int, ILayer* > > matches;
    matches.reserve( size );
    for( uint64 i = 0; i < size; ++i ) {
        ILayer* layer = &iRoot[i];
        const FString& name = layer->Name();
        uint32 typeID = iRoot[i].TypeID();
        int dist = static_cast< int >( LevenshteinDistance( iStr.Data(), iStr.Size(), name.Data(), name.Size() ) );
        matches.push_back( std::tuple< int, ILayer* >( dist, layer ) );
    }
    std::sort( matches.begin(), matches.end() );
    return  matches.size() ? *std::get< 1 >( matches[0] ) : iRoot.Self();
}

const ILayer&
FindLayerByFuzzyNameInContainer( const FString& iStr, const TRoot< ILayer >& iRoot ) {
    const uint64 size = iRoot.Children().Size();
    std::vector< std::tuple< int, const ILayer* > > matches;
    matches.reserve( size );
    for( uint64 i = 0; i < size; ++i ) {
        const ILayer* layer = &iRoot[i];
        const FString& name = layer->Name();
        uint32 typeID = iRoot[i].TypeID();
        int dist = static_cast< int >( LevenshteinDistance( iStr.Data(), iStr.Size(), name.Data(), name.Size() ) );
        matches.push_back( std::tuple< int, const ILayer* >( dist, layer ) );
    }
    std::sort( matches.begin(), matches.end() );
    return  matches.size() ? *std::get< 1 >( matches[0] ) : iRoot.Self();
}

IAnimatedLayer&
FindLayerByFuzzyNameInContainer( const FString& iStr, IAnimatedLayerRoot& iRoot ) {
    const uint64 size = iRoot.Children().Size();
    std::vector< std::tuple< int, IAnimatedLayer* > > matches;
    matches.reserve( size );
    for( uint64 i = 0; i < size; ++i ) {
        IAnimatedLayer* layer = &iRoot[i];
        const FString& name = layer->Name();
        uint32 typeID = iRoot[i].TypeID();
        int dist = static_cast< int >( LevenshteinDistance( iStr.Data(), iStr.Size(), name.Data(), name.Size() ) );
        matches.push_back( std::tuple< int, IAnimatedLayer* >( dist, layer ) );
    }
    std::sort( matches.begin(), matches.end() );
    return  matches.size() ? *std::get< 1 >( matches[0] ) : iRoot.Self();
}

const IAnimatedLayer&
FindLayerByFuzzyNameInContainer( const FString& iStr, const IAnimatedLayerRoot& iRoot ) {
    const uint64 size = iRoot.Children().Size();
    std::vector< std::tuple< int, const IAnimatedLayer* > > matches;
    matches.reserve( size );
    for( uint64 i = 0; i < size; ++i ) {
        const IAnimatedLayer* layer = &iRoot[i];
        const FString& name = layer->Name();
        uint32 typeID = iRoot[i].TypeID();
        int dist = static_cast< int >( LevenshteinDistance( iStr.Data(), iStr.Size(), name.Data(), name.Size() ) );
        matches.push_back( std::tuple< int, const IAnimatedLayer* >( dist, layer ) );
    }
    std::sort( matches.begin(), matches.end() );
    return  matches.size() ? *std::get< 1 >( matches[0] ) : iRoot.Self();
}

ULIS_NAMESPACE_END

