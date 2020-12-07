// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SpecializationCondition.cpp
* @author       Clement Berthaud
* @brief        This file provides implementation for the dispatch specialization utilities.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Dispatch/SpecializationCondition.h"
#include "Image/Format.h"

ULIS_NAMESPACE_BEGIN
bool DispatchTestIsUnorderedRGBA8( eFormat iFormat ) {
    return  ( iFormat & ULIS_FORMAT_MASK_LAYOUT ) == eFormat::Format_RGBA8;
}

bool DispatchTestIsUnorderedRGBAF( eFormat iFormat ) {
    return  ( iFormat & ULIS_FORMAT_MASK_LAYOUT ) == eFormat::Format_RGBAF;
}

ULIS_NAMESPACE_END

