// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         HostDeviceInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FCPU struct.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Base/HostDeviceInfo.h"
#include "cpu_x86.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// FHostDeviceInfo
FHostDeviceInfo::FHostDeviceInfo( bool iVendor_AMD
                                , bool iVendor_Intel
                                , bool iOS_x64
                                , bool iOS_AVX
                                , bool iOS_AVX512
                                , bool iHW_MMX
                                , bool iHW_x64
                                , bool iHW_ABM
                                , bool iHW_RDRAND
                                , bool iHW_BMI1
                                , bool iHW_BMI2
                                , bool iHW_ADX
                                , bool iHW_PREFETCHWT1
                                , bool iHW_MPX
                                , bool iHW_SSE
                                , bool iHW_SSE2
                                , bool iHW_SSE3
                                , bool iHW_SSSE3
                                , bool iHW_SSE41
                                , bool iHW_SSE42
                                , bool iHW_SSE4a
                                , bool iHW_AES
                                , bool iHW_SHA
                                , bool iHW_AVX
                                , bool iHW_XOP
                                , bool iHW_FMA3
                                , bool iHW_FMA4
                                , bool iHW_AVX2
                                , bool iHW_AVX512_F
                                , bool iHW_AVX512_PF
                                , bool iHW_AVX512_ER
                                , bool iHW_AVX512_CD
                                , bool iHW_AVX512_VL
                                , bool iHW_AVX512_BW
                                , bool iHW_AVX512_DQ
                                , bool iHW_AVX512_IFMA
                                , bool iHW_AVX512_VBMI )
    : Vendor_AMD        ( iVendor_AMD       )
    , Vendor_Intel      ( iVendor_Intel     )
    , OS_x64            ( iOS_x64           )
    , OS_AVX            ( iOS_AVX           )
    , OS_AVX512         ( iOS_AVX512        )
    , HW_MMX            ( iHW_MMX           )
    , HW_x64            ( iHW_x64           )
    , HW_ABM            ( iHW_ABM           )
    , HW_RDRAND         ( iHW_RDRAND        )
    , HW_BMI1           ( iHW_BMI1          )
    , HW_BMI2           ( iHW_BMI2          )
    , HW_ADX            ( iHW_ADX           )
    , HW_PREFETCHWT1    ( iHW_PREFETCHWT1   )
    , HW_MPX            ( iHW_MPX           )
    , HW_SSE            ( iHW_SSE           )
    , HW_SSE2           ( iHW_SSE2          )
    , HW_SSE3           ( iHW_SSE3          )
    , HW_SSSE3          ( iHW_SSSE3         )
    , HW_SSE41          ( iHW_SSE41         )
    , HW_SSE42          ( iHW_SSE42         )
    , HW_SSE4a          ( iHW_SSE4a         )
    , HW_AES            ( iHW_AES           )
    , HW_SHA            ( iHW_SHA           )
    , HW_AVX            ( iHW_AVX           )
    , HW_XOP            ( iHW_XOP           )
    , HW_FMA3           ( iHW_FMA3          )
    , HW_FMA4           ( iHW_FMA4          )
    , HW_AVX2           ( iHW_AVX2          )
    , HW_AVX512_F       ( iHW_AVX512_F      )
    , HW_AVX512_PF      ( iHW_AVX512_PF     )
    , HW_AVX512_ER      ( iHW_AVX512_ER     )
    , HW_AVX512_CD      ( iHW_AVX512_CD     )
    , HW_AVX512_VL      ( iHW_AVX512_VL     )
    , HW_AVX512_BW      ( iHW_AVX512_BW     )
    , HW_AVX512_DQ      ( iHW_AVX512_DQ     )
    , HW_AVX512_IFMA    ( iHW_AVX512_IFMA   )
    , HW_AVX512_VBMI    ( iHW_AVX512_VBMI   )
{}

FHostDeviceInfo
FHostDeviceInfo::Detect() {
    ::FeatureDetector::cpu_x86 tmp;
    tmp.detect_host();
    return  FHostDeviceInfo( tmp.Vendor_AMD    
                           , tmp.Vendor_Intel  
                           , tmp.OS_x64        
                           , tmp.OS_AVX        
                           , tmp.OS_AVX512     
                           , tmp.HW_MMX        
                           , tmp.HW_x64        
                           , tmp.HW_ABM        
                           , tmp.HW_RDRAND     
                           , tmp.HW_BMI1       
                           , tmp.HW_BMI2       
                           , tmp.HW_ADX        
                           , tmp.HW_PREFETCHWT1
                           , tmp.HW_MPX        
                           , tmp.HW_SSE        
                           , tmp.HW_SSE2       
                           , tmp.HW_SSE3       
                           , tmp.HW_SSSE3      
                           , tmp.HW_SSE41      
                           , tmp.HW_SSE42      
                           , tmp.HW_SSE4a      
                           , tmp.HW_AES        
                           , tmp.HW_SHA        
                           , tmp.HW_AVX        
                           , tmp.HW_XOP        
                           , tmp.HW_FMA3       
                           , tmp.HW_FMA4       
                           , tmp.HW_AVX2       
                           , tmp.HW_AVX512_F   
                           , tmp.HW_AVX512_PF  
                           , tmp.HW_AVX512_ER  
                           , tmp.HW_AVX512_CD  
                           , tmp.HW_AVX512_VL  
                           , tmp.HW_AVX512_BW  
                           , tmp.HW_AVX512_DQ  
                           , tmp.HW_AVX512_IFMA
                           , tmp.HW_AVX512_VBMI );
}

ULIS3_NAMESPACE_END

