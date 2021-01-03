// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Device_macOS.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FHardwareMetrics tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/Device.h"
#include "System/DeviceHelpers.h"

#include <cpuid.h>
#include <intrin.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <thread>
#include <sys/sysctl.h>

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

size_t
cache_line_size() {
    size_t line_size = 0;
    size_t sizeof_line_size = sizeof(line_size);
    sysctlbyname("hw.cachelinesize", &line_size, &sizeof_line_size, 0, 0);
    return line_size;
}

size_t
cache_size() {
    size_t line_size = 0;
    size_t sizeof_line_size = sizeof(line_size);
    sysctlbyname("hw.l1dcachesize", &line_size, &sizeof_line_size, 0, 0);
    return line_size;
}


} // namespace detail

FHardwareMetrics::FHardwareMetrics()
    : bField( 0 )
    , mMaxWorkers( std::thread::hardware_concurrency() )
    , mL1CacheSize( detail::cache_size() )
    , mL1CacheLineSize( detail::cache_line_size() )
{
    bField |= ULIS_W_OS_X64( detail::detect_OS_x64() );
    bField |= ULIS_W_OS_AVX( detail::detect_OS_AVX() );
    bField |= ULIS_W_OS_AVX512( detail::detect_OS_AVX512() );

    std::string vendor( detail::get_vendor_string() );
    if( vendor == "GenuineIntel" )
        bField |= ULIS_R_HW_INTEL( 1 );
    else if( vendor == "AuthenticAMD")
        bField |= ULIS_R_HW_AMD( 1 );

    int info[4] { 0, 0, 0, 0 };
    detail::cpuid( info, 0 );
    int nIds = info[0];

    detail::cpuid( info, 0x80000000 );
    uint32_t nExIds = info[0];

    if( nIds >= 0x00000001 ) {
        detail::cpuid( info, 0x00000001 );
        bField |= ULIS_W_HW_MMX(    uint64( ( info[3] & ( uint64( 1 ) << 23 ) ) != 0 ) );
        bField |= ULIS_W_HW_SSE(    uint64( ( info[3] & ( uint64( 1 ) << 25 ) ) != 0 ) );
        bField |= ULIS_W_HW_SSE2(   uint64( ( info[3] & ( uint64( 1 ) << 26 ) ) != 0 ) );
        bField |= ULIS_W_HW_SSE3(   uint64( ( info[2] & ( uint64( 1 ) << 0  ) ) != 0 ) );
        bField |= ULIS_W_HW_SSSE3(  uint64( ( info[2] & ( uint64( 1 ) << 9  ) ) != 0 ) );
        bField |= ULIS_W_HW_SSE41(  uint64( ( info[2] & ( uint64( 1 ) << 19 ) ) != 0 ) );
        bField |= ULIS_W_HW_SSE42(  uint64( ( info[2] & ( uint64( 1 ) << 20 ) ) != 0 ) );
        bField |= ULIS_W_HW_AES(    uint64( ( info[2] & ( uint64( 1 ) << 25 ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX(    uint64( ( info[2] & ( uint64( 1 ) << 28 ) ) != 0 ) );
        bField |= ULIS_W_HW_FMA3(   uint64( ( info[2] & ( uint64( 1 ) << 12 ) ) != 0 ) );
        bField |= ULIS_W_HW_RDRAND( uint64( ( info[2] & ( uint64( 1 ) << 30 ) ) != 0 ) );
    }

    if( nIds >= 0x00000007 ) {
        detail::cpuid( info, 0x00000007 );
        bField |= ULIS_W_HW_AVX2(           uint64( ( info[1] & ( uint64( 1 ) << 5  ) ) != 0 ) );
        bField |= ULIS_W_HW_BMI1(           uint64( ( info[1] & ( uint64( 1 ) << 3  ) ) != 0 ) );
        bField |= ULIS_W_HW_BMI2(           uint64( ( info[1] & ( uint64( 1 ) << 8  ) ) != 0 ) );
        bField |= ULIS_W_HW_ADX(            uint64( ( info[1] & ( uint64( 1 ) << 19 ) ) != 0 ) );
        bField |= ULIS_W_HW_MPX(            uint64( ( info[1] & ( uint64( 1 ) << 14 ) ) != 0 ) );
        bField |= ULIS_W_HW_SHA(            uint64( ( info[1] & ( uint64( 1 ) << 29 ) ) != 0 ) );
        bField |= ULIS_W_HW_PREFETCHWT1(    uint64( ( info[2] & ( uint64( 1 ) << 0  ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX512_F(       uint64( ( info[1] & ( uint64( 1 ) << 16 ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX512_CD(      uint64( ( info[1] & ( uint64( 1 ) << 28 ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX512_PF(      uint64( ( info[1] & ( uint64( 1 ) << 26 ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX512_ER(      uint64( ( info[1] & ( uint64( 1 ) << 27 ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX512_VL(      uint64( ( info[1] & ( uint64( 1 ) << 31 ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX512_BW(      uint64( ( info[1] & ( uint64( 1 ) << 30 ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX512_DQ(      uint64( ( info[1] & ( uint64( 1 ) << 17 ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX512_IFMA(    uint64( ( info[1] & ( uint64( 1 ) << 21 ) ) != 0 ) );
        bField |= ULIS_W_HW_AVX512_VBMI(    uint64( ( info[2] & ( uint64( 1 ) << 1  ) ) != 0 ) );
    }

    if( nExIds >= 0x80000001 ) {
        detail::cpuid( info, 0x80000001 );
        bField |= ULIS_W_HW_X64(    uint64( ( info[3] & ( uint64( 1 ) << 29 ) ) != 0 ) );
        bField |= ULIS_W_HW_ABM(    uint64( ( info[2] & ( uint64( 1 ) << 5  ) ) != 0 ) );
        bField |= ULIS_W_HW_SSE4A(  uint64( ( info[2] & ( uint64( 1 ) << 6  ) ) != 0 ) );
        bField |= ULIS_W_HW_FMA4(   uint64( ( info[2] & ( uint64( 1 ) << 16 ) ) != 0 ) );
        bField |= ULIS_W_HW_XOP(    uint64( ( info[2] & ( uint64( 1 ) << 11 ) ) != 0 ) );
    }
}

ULIS_NAMESPACE_END

