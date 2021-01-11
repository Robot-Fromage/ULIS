// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Device.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FHardwareMetrics tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FHardwareMetrics
/// @brief      The FHardwareMetrics class provides a mean of fetching and reading
///             informations about the CPU in a single-CPU, multicore environment.
///
/// @details    FHardwareMetrics provides functions to fetch wether support for 64bit
///             features or SIMD extensions are supported on the CPU that runs
///             the compiled code, at runtime. It has no relation wathsoever with
///             the computer that compiled the code in the first place, it is a
///             cross platform way of detecting wether the consumer of the library
///             code is able to rely on the implementations compiled with these
///             SIMD features in mind. Additionnaly, it provides extra information
///             about the CPU vendor, and some information about the OS, in relation
///             to its support for the features sets or for 64bit features, although
///             64bit is always assumed to be true within ULIS.
///
///             FHardwareMetrics also provides some insight about the hardware such as the
///             number of available cores, or the size of cache lines.
class ULIS_API FHardwareMetrics
{
public:
    /*! Constructor. */
    FHardwareMetrics();

public:
    bool IsHardwareAMD() const;
    bool IsHardwareIntel() const;
    bool IsOSx64() const;
    bool HasOSAVX() const;
    bool HasOSAVX512() const;
    bool HasHardwarex64() const;
    bool HasHardwareMMX() const;
    bool HasHardwareABM() const;
    bool HasHardwareRDRAND() const;
    bool HasHardwareBMI1() const;
    bool HasHardwareBMI2() const;
    bool HasHardwareADX() const;
    bool HasHardwarePREFETCHWT1() const;
    bool HasHardwareMPX() const;
    bool HasHardwareSSE() const;
    bool HasHardwareSSE2() const;
    bool HasHardwareSSE3() const;
    bool HasHardwareSSSE3() const;
    bool HasHardwareSSE41() const;
    bool HasHardwareSSE42() const;
    bool HasHardwareSSE4a() const;
    bool HasHardwareAES() const;
    bool HasHardwareSHA() const;
    bool HasHardwareAVX() const;
    bool HasHardwareXOP() const;
    bool HasHardwareFMA3() const;
    bool HasHardwareFMA4() const;
    bool HasHardwareAVX2() const;
    bool HasHardwareAVX512_F() const;
    bool HasHardwareAVX512_PF() const;
    bool HasHardwareAVX512_ER() const;
    bool HasHardwareAVX512_CD() const;
    bool HasHardwareAVX512_VL() const;
    bool HasHardwareAVX512_BW() const;
    bool HasHardwareAVX512_DQ() const;
    bool HasHardwareAVX512_IFMA() const;
    bool HasHardwareAVX512_VBMI() const;
    uint32 MaxWorkers() const;
    uint64 L1CacheSize() const;
    uint64 L1CacheLineSize() const;

private:
    uint64 bField;
    uint32 mMaxWorkers;
    uint64 mL1CacheSize;
    uint64 mL1CacheLineSize;
};

enum ePerformanceIntent : uint32 {
      PerformanceIntent_MEM   = 0b0000
    , PerformanceIntent_SSE   = 0b0001
    , PerformanceIntent_AVX   = 0b0010
    , PerformanceIntent_Max   = 0b1111
};

ULIS_NAMESPACE_END

