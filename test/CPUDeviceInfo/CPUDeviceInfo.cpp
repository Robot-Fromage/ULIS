// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CPUDeviceInfo.cpp
* @author       Clement Berthaud
* @brief        CPUDeviceInfo application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>

int main( int argc, char *argv[] ) {
    ::ul3::FHardwareMetrics deviceInfo = ::ul3::FHardwareMetrics::Detect();
    const char* out[] = { "NO", "YES" };
    std::cout << "Vendor_AMD        : " << out[ deviceInfo.Vendor_AMD       ] << std::endl;
    std::cout << "Vendor_Intel      : " << out[ deviceInfo.Vendor_Intel     ] << std::endl;
    std::cout << "OS_x64            : " << out[ deviceInfo.OS_x64           ] << std::endl;
    std::cout << "OS_AVX            : " << out[ deviceInfo.OS_AVX           ] << std::endl;
    std::cout << "OS_AVX512         : " << out[ deviceInfo.OS_AVX512        ] << std::endl;
    std::cout << "HW_MMX            : " << out[ deviceInfo.HW_MMX           ] << std::endl;
    std::cout << "HW_x64            : " << out[ deviceInfo.HW_x64           ] << std::endl;
    std::cout << "HW_ABM            : " << out[ deviceInfo.HW_ABM           ] << std::endl;
    std::cout << "HW_RDRAND         : " << out[ deviceInfo.HW_RDRAND        ] << std::endl;
    std::cout << "HW_BMI1           : " << out[ deviceInfo.HW_BMI1          ] << std::endl;
    std::cout << "HW_BMI2           : " << out[ deviceInfo.HW_BMI2          ] << std::endl;
    std::cout << "HW_ADX            : " << out[ deviceInfo.HW_ADX           ] << std::endl;
    std::cout << "HW_PREFETCHWT1    : " << out[ deviceInfo.HW_PREFETCHWT1   ] << std::endl;
    std::cout << "HW_MPX            : " << out[ deviceInfo.HW_MPX           ] << std::endl;
    std::cout << "HW_SSE            : " << out[ deviceInfo.HW_SSE           ] << std::endl;
    std::cout << "HW_SSE2           : " << out[ deviceInfo.HW_SSE2          ] << std::endl;
    std::cout << "HW_SSE3           : " << out[ deviceInfo.HW_SSE3          ] << std::endl;
    std::cout << "HW_SSSE3          : " << out[ deviceInfo.HW_SSSE3         ] << std::endl;
    std::cout << "HW_SSE41          : " << out[ deviceInfo.HW_SSE41         ] << std::endl;
    std::cout << "HW_SSE42          : " << out[ deviceInfo.HW_SSE42         ] << std::endl;
    std::cout << "HW_SSE4a          : " << out[ deviceInfo.HW_SSE4a         ] << std::endl;
    std::cout << "HW_AES            : " << out[ deviceInfo.HW_AES           ] << std::endl;
    std::cout << "HW_SHA            : " << out[ deviceInfo.HW_SHA           ] << std::endl;
    std::cout << "HW_AVX            : " << out[ deviceInfo.HW_AVX           ] << std::endl;
    std::cout << "HW_XOP            : " << out[ deviceInfo.HW_XOP           ] << std::endl;
    std::cout << "HW_FMA3           : " << out[ deviceInfo.HW_FMA3          ] << std::endl;
    std::cout << "HW_FMA4           : " << out[ deviceInfo.HW_FMA4          ] << std::endl;
    std::cout << "HW_AVX2           : " << out[ deviceInfo.HW_AVX2          ] << std::endl;
    std::cout << "HW_AVX512_F       : " << out[ deviceInfo.HW_AVX512_F      ] << std::endl;
    std::cout << "HW_AVX512_PF      : " << out[ deviceInfo.HW_AVX512_PF     ] << std::endl;
    std::cout << "HW_AVX512_ER      : " << out[ deviceInfo.HW_AVX512_ER     ] << std::endl;
    std::cout << "HW_AVX512_CD      : " << out[ deviceInfo.HW_AVX512_CD     ] << std::endl;
    std::cout << "HW_AVX512_VL      : " << out[ deviceInfo.HW_AVX512_VL     ] << std::endl;
    std::cout << "HW_AVX512_BW      : " << out[ deviceInfo.HW_AVX512_BW     ] << std::endl;
    std::cout << "HW_AVX512_DQ      : " << out[ deviceInfo.HW_AVX512_DQ     ] << std::endl;
    std::cout << "HW_AVX512_IFMA    : " << out[ deviceInfo.HW_AVX512_IFMA   ] << std::endl;
    std::cout << "HW_AVX512_VBMI    : " << out[ deviceInfo.HW_AVX512_VBMI   ] << std::endl;
    std::cout << "MAX WORKERS       : " << ::ul3::MaxWorkers()                << std::endl;
    return  0;
}

