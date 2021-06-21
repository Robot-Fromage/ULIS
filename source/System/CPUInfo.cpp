// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CPUInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FCPUInfo tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "System/CPUInfo.h"
#include "System/CPUInfoHelpers.h"
#include "System/CPUInfo_Private.h"

ULIS_NAMESPACE_BEGIN
namespace detail {
extern FCPUInfo_Private sgCPUInfo_private_instance;
} // namespace detail

//static
bool
FCPUInfo::IsHardwareAmd()
{
    return  ULIS_R_HW_AMD( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::IsHardwareIntel()
{
    return  ULIS_R_HW_INTEL( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::IsOSx64() {
    return  ULIS_R_OS_X64( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasOsAvx()
{
    return  ULIS_R_OS_AVX( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasOsAvx512()
{
    return  ULIS_R_OS_AVX512( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool FCPUInfo::HasHardwarex64()
{
    return  ULIS_R_HW_X64( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareMMX()
{
    return  ULIS_R_HW_MMX( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareABM()
{
    return  ULIS_R_HW_ABM( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareRDRAND()
{
    return  ULIS_R_HW_RDRAND( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareBMI1()
{
    return  ULIS_R_HW_BMI1( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareBMI2()
{
    return  ULIS_R_HW_BMI2( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareADX()
{
    return  ULIS_R_HW_ADX( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwarePREFETCHWT1()
{
    return  ULIS_R_HW_PREFETCHWT1( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareMPX()
{
    return  ULIS_R_HW_MPX( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareSSE()
{
    return  ULIS_R_HW_SSE( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareSSE2()
{
    return  ULIS_R_HW_SSE2( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareSSE3()
{
    return  ULIS_R_HW_SSE3( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareSSSE3()
{
    return  ULIS_R_HW_SSSE3( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareSSE41()
{
    return  ULIS_R_HW_SSE41( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareSSE42()
{
    return  ULIS_R_HW_SSE42( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareSSE4a()
{
    return  ULIS_R_HW_SSE4A( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAES()
{
    return  ULIS_R_HW_AES( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareSHA()
{
    return  ULIS_R_HW_SHA( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX()
{
    return  ULIS_R_HW_AVX( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareXOP()
{
    return  ULIS_R_HW_XOP( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareFMA3()
{
    return  ULIS_R_HW_FMA3( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareFMA4()
{
    return  ULIS_R_HW_FMA4( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX2()
{
    return  ULIS_R_HW_AVX2( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX512_F()
{
    return  ULIS_R_HW_AVX512_F( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX512_PF()
{
    return  ULIS_R_HW_AVX512_PF( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX512_ER()
{
    return  ULIS_R_HW_AVX512_ER( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX512_CD()
{
    return  ULIS_R_HW_AVX512_CD( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX512_VL()
{
    return  ULIS_R_HW_AVX512_VL( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX512_BW()
{
    return  ULIS_R_HW_AVX512_BW( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX512_DQ()
{
    return  ULIS_R_HW_AVX512_DQ( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX512_IFMA()
{
    return  ULIS_R_HW_AVX512_IFMA( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
bool
FCPUInfo::HasHardwareAVX512_VBMI()
{
    return  ULIS_R_HW_AVX512_VBMI( detail::sgCPUInfo_private_instance.features_bitfield );
}

//static
uint32
FCPUInfo::MaxWorkers()
{
    return  detail::sgCPUInfo_private_instance.max_workers;
}

//static
uint32
FCPUInfo::L1CacheSize()
{
    return  detail::sgCPUInfo_private_instance.l1_cache_size;
}

//static
uint32
FCPUInfo::L1CacheLineSize()
{
    return  detail::sgCPUInfo_private_instance.l1_cache_line_size;
}

ULIS_NAMESPACE_END

