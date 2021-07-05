// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerTypedefs.h
* @author       Clement Berthaud, Eric Scholl
* @brief        This file provides the declaration of specific Animated Layer Types
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/AnimatedLayer/AnimatedLayer.h"
#include "Layer/AnimatedLayer/AnimatedLayerFolder.h"
#include "Layer/AnimatedLayer/AnimatedLayerImage.h"
#include "Layer/AnimatedLayer/AnimatedLayerStack.h"
#include "Layer/AnimatedLayer/AnimatedLayerText.h"
#include "Layer/Common/RegularBlockAllocator.h"
#include "Layer/Common/RegularBlockRenderer.h"

ULIS_NAMESPACE_BEGIN
// Typedefs
typedef TAbstractAnimatedLayerDrawable< FBlock > IAnimatedLayerDrawableBlock;
typedef TAnimatedLayerStack< FBlock, void, FDummyAnimatedSuperStack > FAnimatedLayerStack;
typedef TAnimatedLayerFolder< FBlock, void, FRegularBlockRenderer, FRegularBlockAllocator, FAnimatedLayerStack > FAnimatedLayerFolder;
typedef TAnimatedLayerImage< FBlock, void, FRegularBlockRenderer, FRegularBlockAllocator, FAnimatedLayerStack > FAnimatedLayerImage;
typedef TAnimatedLayerText< FBlock, void, FRegularBlockRenderer, FRegularBlockAllocator, FAnimatedLayerStack > FAnimatedLayerText;

// Exports
//template class ULIS_API TLambdaCallback< void, const FBlock* >;
//template class ULIS_API TCallbackCapable< TLambdaCallback< void, const FBlock* > >;
//template class ULIS_API TAnimatedLayerFolder< FBlock, void, void, void >;
//template class ULIS_API TAnimatedLayerImage< FBlock, void, void, void, FAnimatedLayerStack >;
//template class ULIS_API TAnimatedLayerStack< FBlock, void, FDummySuperStack >;
//template class ULIS_API TAnimatedLayerText< FBlock, void, void, void >;

ULIS_NAMESPACE_END

