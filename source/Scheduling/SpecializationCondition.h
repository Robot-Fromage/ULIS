// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SpecializationCondition.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the dispatch specialization utilities.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
typedef bool (*fpCond)( eFormat );
bool DispatchTestIsUnorderedRGBA8( eFormat iFormat );
bool DispatchTestIsUnorderedRGBAF( eFormat iFormat );

ULIS_NAMESPACE_END

