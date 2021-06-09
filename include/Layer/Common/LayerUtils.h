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
ULIS_NAMESPACE_END

