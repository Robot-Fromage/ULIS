// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CPUInfo_Private.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FCPUInfo_Private tools.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/// @class      FCPUInfo_Private
/// @brief      The FCPUInfo_Private provides the internal mechanisms for
///             KCPUInfo.
/// @details    FCPUInfo_Private uses a variation of the pimpl idiom in
///             FCPUInfo_Private to abstract the platform specifics.
struct FCPUInfo_Private
{
    FCPUInfo_Private();
    uint64 features_bitfield;
    uint32 max_workers;
    uint32 l1_cache_size;
    uint32 l1_cache_line_size;
};

ULIS_NAMESPACE_END

