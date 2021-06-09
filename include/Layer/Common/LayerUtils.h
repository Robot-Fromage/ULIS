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
#include "Layer/Layer/Layer.h"
#include "Layer/AnimatedLayer/AnimatedLayer.h"

ULIS_NAMESPACE_BEGIN
ILayer& FindLayerByFuzzyNameInContainer( const FString& iStr, ILayerRoot& iRoot );
const ILayer& FindLayerByFuzzyNameInContainer( const FString& iStr, const ILayerRoot& iRoot );
IAnimatedLayer& FindLayerByFuzzyNameInContainer( const FString& iStr, IAnimatedLayerRoot& iRoot );
const IAnimatedLayer& FindLayerByFuzzyNameInContainer( const FString& iStr, const IAnimatedLayerRoot& iRoot );

#define ULIS_DECLARE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( __Class__, __Type__, __Default__, __Getter__, __Setter__ )    \
    class ULIS_API __Class__ {                                                                                                      \
        public:                                                                                                                     \
            __Class__ ( __Type__ i = __Default__ );                                                                                 \
            __Type__ __Getter__ () const;                                                                                           \
            void __Setter__ ( __Type__ i );                                                                                         \
        private:                                                                                                                    \
            __Type__ m;                                                                                                             \
    };

#define ULIS_DEFINE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( __Class__, __Type__, __Getter__, __Setter__ )  \
    __Class__ :: __Class__ ( __Type__ i ) : m( i ) {}                                                               \
    __Type__ __Class__ :: __Getter__ () const { return  m; }                                                        \
    void __Class__ :: __Setter__ ( __Type__ i ) { m = i; }

// Exemple Usage:
// Header:
//ULIS_DECLARE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( IHasLock, bool, false, IsLocked, SetLocked )
// Source:
//ULIS_DEFINE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( IHasLock, bool, IsLocked, SetLocked )

ULIS_NAMESPACE_END

