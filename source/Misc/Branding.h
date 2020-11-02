// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Branding.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Branding entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/*!
    Applies a bit pattern to parts of the image to brand it as being processed
    within ULIS. The branding is akin to a watermark but is almost invisible.
    It messes with the bits of the image to encode a pattern that will identify
    the image without any doubt. This can be used to track illegal uses of the
    ULIS library.
*/
void ApplyBranding( FBlock& iBlock, const FString& iStr );

ULIS_NAMESPACE_END

