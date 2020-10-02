// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Device_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FHardwareMetrics tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/Device.h"
#include "System/DeviceHelpers.h"

ULIS_NAMESPACE_BEGIN

FHardwareMetrics::FHardwareMetrics()
    : bField( 0 )
    , mMaxWorkers( 1 )
    , mL1CacheSize( 65536 )
    , mL1CacheLineSize( 64 )
{
    bField =
          ULIS_W_HW_AMD( 0 )
        | ULIS_W_HW_INTEL( 0 )
        | ULIS_W_OS_X64( 1 )
        | ULIS_W_OS_AVX( 0 )
        | ULIS_W_OS_AVX512( 0 )
        | ULIS_W_HW_X64( 1 )
        | ULIS_W_HW_MMX( 0 )
        | ULIS_W_HW_ABM( 0 )
        | ULIS_W_HW_RDRAND( 0 )
        | ULIS_W_HW_BMI1( 0 )
        | ULIS_W_HW_BMI2( 0 )
        | ULIS_W_HW_ADX( 0 )
        | ULIS_W_HW_PREFETCHWT1( 0 )
        | ULIS_W_HW_MPX( 0 )
        | ULIS_W_HW_SSE( 0 )
        | ULIS_W_HW_SSE2( 0 )
        | ULIS_W_HW_SSE3( 0 )
        | ULIS_W_HW_SSSE3( 0 )
        | ULIS_W_HW_SSE41( 0 )
        | ULIS_W_HW_SSE42( 0 )
        | ULIS_W_HW_SSE4A( 0 )
        | ULIS_W_HW_AES( 0 )
        | ULIS_W_HW_SHA( 0 )
        | ULIS_W_HW_AVX( 0 )
        | ULIS_W_HW_XOP( 0 )
        | ULIS_W_HW_FMA3( 0 )
        | ULIS_W_HW_FMA4( 0 )
        | ULIS_W_HW_AVX2( 0 )
        | ULIS_W_HW_AVX512_F( 0 )
        | ULIS_W_HW_AVX512_PF( 0 )
        | ULIS_W_HW_AVX512_ER( 0 )
        | ULIS_W_HW_AVX512_CD( 0 )
        | ULIS_W_HW_AVX512_VL( 0 )
        | ULIS_W_HW_AVX512_BW( 0 )
        | ULIS_W_HW_AVX512_DQ( 0 )
        | ULIS_W_HW_AVX512_IFMA( 0 )
        | ULIS_W_HW_AVX512_VBMI( 0 );
}

ULIS_NAMESPACE_END

