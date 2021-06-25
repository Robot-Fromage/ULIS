// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MemoryInfo_Generic.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FMemoryInfo tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/MemoryInfo/MemoryInfo.h"

ULIS_NAMESPACE_BEGIN
namespace detail {

uint64 TotalVirtualMemory() {
    return  0;
}

uint64 VirtualMemoryCurrentlyUsed() {
    return  0;
}

uint64 VirtualMemoryCurrentlyUsedByProcess() {
    return  0;
}

uint64 TotalRAM() {
    return  0;
}

uint64 TotalRAMCurrentlyUsed() {
    return  0;
}

uint64 TotalRAMCurrentlyUsedByProcess() {
    return  0;
}

double CPUCurrentlyUsed(){
    return  0;
}

double CPUCurrentlyUsedByProcess(){
    return  0;
}

} // namespace detail
ULIS_NAMESPACE_END

