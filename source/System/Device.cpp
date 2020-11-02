// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Device.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FHardwareMetrics tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "System/Device.h"
#include "System/DeviceHelpers.h"

#if defined( ULIS_WIN )
#include "System/Device_Windows.ipp"
#elif defined( ULIS_MACOS )
#include "System/Device_macOS.ipp"
#elif defined( ULIS_LINUX )
#include "System/Device_Linux.ipp"
#else
#include "System/Device_Generic.ipp"
#endif

ULIS_NAMESPACE_BEGIN
bool    FHardwareMetrics::IsHardwareAMD() const {            return  ULIS_R_HW_AMD(          bField ); }
bool    FHardwareMetrics::IsHardwareIntel() const {          return  ULIS_R_HW_INTEL(        bField ); }
bool    FHardwareMetrics::IsOSx64() const {                  return  ULIS_R_OS_X64(          bField ); }
bool    FHardwareMetrics::HasOSAVX() const {                 return  ULIS_R_OS_AVX(          bField ); }
bool    FHardwareMetrics::HasOSAVX512() const {              return  ULIS_R_OS_AVX512(       bField ); }
bool    FHardwareMetrics::HasHardwarex64() const {           return  ULIS_R_HW_X64(          bField ); }
bool    FHardwareMetrics::HasHardwareMMX() const {           return  ULIS_R_HW_MMX(          bField ); }
bool    FHardwareMetrics::HasHardwareABM() const {           return  ULIS_R_HW_ABM(          bField ); }
bool    FHardwareMetrics::HasHardwareRDRAND() const {        return  ULIS_R_HW_RDRAND(       bField ); }
bool    FHardwareMetrics::HasHardwareBMI1() const {          return  ULIS_R_HW_BMI1(         bField ); }
bool    FHardwareMetrics::HasHardwareBMI2() const {          return  ULIS_R_HW_BMI2(         bField ); }
bool    FHardwareMetrics::HasHardwareADX() const {           return  ULIS_R_HW_ADX(          bField ); }
bool    FHardwareMetrics::HasHardwarePREFETCHWT1() const {   return  ULIS_R_HW_PREFETCHWT1(  bField ); }
bool    FHardwareMetrics::HasHardwareMPX() const {           return  ULIS_R_HW_MPX(          bField ); }
bool    FHardwareMetrics::HasHardwareSSE() const {           return  ULIS_R_HW_SSE(          bField ); }
bool    FHardwareMetrics::HasHardwareSSE2() const {          return  ULIS_R_HW_SSE2(         bField ); }
bool    FHardwareMetrics::HasHardwareSSE3() const {          return  ULIS_R_HW_SSE3(         bField ); }
bool    FHardwareMetrics::HasHardwareSSSE3() const {         return  ULIS_R_HW_SSSE3(        bField ); }
bool    FHardwareMetrics::HasHardwareSSE41() const {         return  ULIS_R_HW_SSE41(        bField ); }
bool    FHardwareMetrics::HasHardwareSSE42() const {         return  ULIS_R_HW_SSE42(        bField ); }
bool    FHardwareMetrics::HasHardwareSSE4a() const {         return  ULIS_R_HW_SSE4A(        bField ); }
bool    FHardwareMetrics::HasHardwareAES() const {           return  ULIS_R_HW_AES(          bField ); }
bool    FHardwareMetrics::HasHardwareSHA() const {           return  ULIS_R_HW_SHA(          bField ); }
bool    FHardwareMetrics::HasHardwareAVX() const {           return  ULIS_R_HW_AVX(          bField ); }
bool    FHardwareMetrics::HasHardwareXOP() const {           return  ULIS_R_HW_XOP(          bField ); }
bool    FHardwareMetrics::HasHardwareFMA3() const {          return  ULIS_R_HW_FMA3(         bField ); }
bool    FHardwareMetrics::HasHardwareFMA4() const {          return  ULIS_R_HW_FMA4(         bField ); }
bool    FHardwareMetrics::HasHardwareAVX2() const {          return  ULIS_R_HW_AVX2(         bField ); }
bool    FHardwareMetrics::HasHardwareAVX512_F() const {      return  ULIS_R_HW_AVX512_F(     bField ); }
bool    FHardwareMetrics::HasHardwareAVX512_PF() const {     return  ULIS_R_HW_AVX512_PF(    bField ); }
bool    FHardwareMetrics::HasHardwareAVX512_ER() const {     return  ULIS_R_HW_AVX512_ER(    bField ); }
bool    FHardwareMetrics::HasHardwareAVX512_CD() const {     return  ULIS_R_HW_AVX512_CD(    bField ); }
bool    FHardwareMetrics::HasHardwareAVX512_VL() const {     return  ULIS_R_HW_AVX512_VL(    bField ); }
bool    FHardwareMetrics::HasHardwareAVX512_BW() const {     return  ULIS_R_HW_AVX512_BW(    bField ); }
bool    FHardwareMetrics::HasHardwareAVX512_DQ() const {     return  ULIS_R_HW_AVX512_DQ(    bField ); }
bool    FHardwareMetrics::HasHardwareAVX512_IFMA() const {   return  ULIS_R_HW_AVX512_IFMA(  bField ); }
bool    FHardwareMetrics::HasHardwareAVX512_VBMI() const {   return  ULIS_R_HW_AVX512_VBMI(  bField ); }
uint32  FHardwareMetrics::MaxWorkers() const {               return  mMaxWorkers; }
uint64  FHardwareMetrics::L1CacheSize() const {              return  mL1CacheSize; }
uint64  FHardwareMetrics::L1CacheLineSize() const {          return  mL1CacheLineSize; }
ULIS_NAMESPACE_END

