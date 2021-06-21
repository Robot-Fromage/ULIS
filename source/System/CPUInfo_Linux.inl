// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Device_Linux.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FHardwareMetrics tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/CPUInfo.h"
#include "System/CPUInfoHelpers.h"

#include <cpuid.h>
//#include <intrin.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <thread>
#include <stdio.h>

ULIS_NAMESPACE_BEGIN

namespace detail {

#define _XCR_XFEATURE_ENABLED_MASK  0

void
cpuid( int32_t out[4], int32_t x ) {
    __cpuid_count( x, 0, out[0], out[1], out[2], out[3] );
}

uint64_t
xgetbv( unsigned int index ) {
    uint32_t eax, edx;
    __asm__ __volatile__("xgetbv" : "=a"(eax), "=d"(edx) : "c"(index));
    return ( ( uint64_t )edx << 32 ) | eax;
}

bool
detect_OS_x64(){
    //  We only support x64
    return true;
}

bool
detect_OS_AVX() {
    //  Copied from: http://stackoverflow.com/a/22521619/922184
    bool avxSupported = false;

    int cpuInfo[4];
    cpuid(cpuInfo, 1);

    bool osUsesXSAVE_XRSTORE = (cpuInfo[2] & (1 << 27)) != 0;
    bool cpuAVXSuport = (cpuInfo[2] & (1 << 28)) != 0;

    if (osUsesXSAVE_XRSTORE && cpuAVXSuport)
    {
        uint64_t xcrFeatureMask = xgetbv( _XCR_XFEATURE_ENABLED_MASK );
        avxSupported = ( xcrFeatureMask & 0x6 ) == 0x6;
    }

    return  avxSupported;
}

bool
detect_OS_AVX512() {
    if (!detect_OS_AVX())
        return  false;

    uint64_t xcrFeatureMask = xgetbv( _XCR_XFEATURE_ENABLED_MASK );
    return  ( xcrFeatureMask & 0xe6 ) == 0xe6;
}

std::string
get_vendor_string() {
    int32_t CPUInfo[4];
    char name[13];

    cpuid(CPUInfo, 0);
    memcpy(name + 0, &CPUInfo[1], 4);
    memcpy(name + 4, &CPUInfo[3], 4);
    memcpy(name + 8, &CPUInfo[2], 4);
    name[12] = '\0';

    return  name;
}

size_t cache_line_size() {
    FILE * p = 0;
    p = fopen("/sys/devices/system/cpu/cpu0/cache/index0/coherency_line_size", "r");
    unsigned int i = 0;
    if (p) {
        fscanf(p, "%d", &i);
        fclose(p);
    }
    return i;
}


size_t cache_size() {
    FILE * p = 0;
    p = fopen("/sys/devices/system/cpu/cpu0/cache/index0/size", "r");
    unsigned int i = 0;
    if (p) {
        fscanf(p, "%d", &i);
        fclose(p);
    }
    return i;
}

void cache_info( uint8 iLevel, uint32 *oCacheSize, uint32* oLineSize ) {
    *oCacheSize = static_cast< uint32 >( cache_size() );
    *oLineSize = static_cast< uint32 >( cache_line_size() );
}

uint32 num_workers() {
    return  std::thread::hardware_concurrency();
}

} // namespace detail

ULIS_NAMESPACE_END

