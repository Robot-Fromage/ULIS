// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Types.h
* @author       Clement Berthaud
* @brief        This file provides core types definitions for the ULIS library.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Platform.h"
#include <cstdint>
#include <cstring>
#include <memory>

ULIS_NAMESPACE_BEGIN
// Domain Range Typdefs
typedef  uint8_t    uint8;      ///< Used for the smallest adressable type, unsigned 8bit images domain range values and main type for image bits memory storage.
typedef  uint16_t   uint16;     ///< Used for unsigned 16bit images domain range values.
typedef  uint32_t   uint32;     ///< Used for unsigned 32bit images domain range values and sizes such as width and height.
typedef  uint64_t   uint64;     ///< Used for unsigned 64bit images domain range values and whole buffer sizes that might overflow 32bit range.
typedef  int8_t     int8;       ///< Used for signed 8bit images domain range values.
typedef  int16_t    int16;      ///< Used for signed 16bit images domain range values.
typedef  int32_t    int32;      ///< Used for signed 32bit images domain range values.
typedef  int64_t    int64;      ///< Used for signed 64bit images domain range values.
typedef  float      ufloat;     ///< Used for float images domain range values, indicates normalized range [0;1]
typedef  double     udouble;    ///< Used for double images domain range values, indicates normalized range [0;1]

// Perf codes
#define ULIS_PERF_MT           0x1
#define ULIS_PERF_SSE42        0x2
#define ULIS_PERF_AVX2         0x4

// Type codes
#define ULIS_TYPE_UINT8    0x0
#define ULIS_TYPE_UINT16   0x1
#define ULIS_TYPE_UINT32   0x2
#define ULIS_TYPE_UFLOAT   0x3
//DISABLED:DOUBLE#define ULIS_TYPE_UDOUBLE  0x4

// Model codes
#define ULIS_CM_ANY    0
#define ULIS_CM_GREY   1
#define ULIS_CM_RGB    2
#define ULIS_CM_HSV    3
#define ULIS_CM_HSL    4
#define ULIS_CM_CMY    5
#define ULIS_CM_CMYK   6
#define ULIS_CM_YUV    7
#define ULIS_CM_Lab    8
#define ULIS_CM_XYZ    9
#define ULIS_CM_Yxy    10

// Profile codes
#define ULIS_None                  0
#define ULIS_LinearGrey            1
#define ULIS_sGrey                 2
#define ULIS_Grey2_2_D65           3
#define ULIS_LinearRGB             4
#define ULIS_sRGB                  5
#define ULIS_AdobeRGB              6
#define ULIS_YUV                   7
#define ULIS_XYZ                   8
#define ULIS_LabD50                9
#define ULIS_LabD65                10
#define ULIS_Yxy                   11

// Channels codes
#define ULIS_MAX_CHANNELS          5
#define ULIS_CHANNEL_1_BIT         1
#define ULIS_CHANNEL_2_BIT         2
#define ULIS_CHANNEL_3_BIT         4
#define ULIS_CHANNEL_4_BIT         8
#define ULIS_CHANNEL_5_BIT         16

// Named booleans literals
#define ULIS_BLOCKING              1
#define ULIS_NONBLOCKING           0
#define ULIS_CALLCB                1
#define ULIS_NOCB                  0
#define ULIS_AA                    1
#define ULIS_NOAA                  0
#define ULIS_PREMULTIPLIED         1
#define ULIS_UNPREMULTIPLIED       0
#define ULIS_GAMMA_COMPRESSED      1
#define ULIS_GAMMA_UNCOMPRESSED    0

// Type enum
enum eType {
      TYPE_UINT8     = ULIS_TYPE_UINT8
    , TYPE_UINT16    = ULIS_TYPE_UINT16
    , TYPE_UINT32    = ULIS_TYPE_UINT32
    , TYPE_UFLOAT    = ULIS_TYPE_UFLOAT
    //DISABLED:DOUBLE, TYPE_UDOUBLE   = ULIS_TYPE_UDOUBLE
};

// Model enum
enum eColorModel {
      CM_GREY   = ULIS_CM_GREY
    , CM_RGB    = ULIS_CM_RGB
    , CM_HSV    = ULIS_CM_HSV
    , CM_HSL    = ULIS_CM_HSL
    , CM_CMY    = ULIS_CM_CMY
    , CM_CMYK   = ULIS_CM_CMYK
    , CM_YUV    = ULIS_CM_YUV
    , CM_Lab    = ULIS_CM_Lab
    , CM_XYZ    = ULIS_CM_XYZ
    , CM_Yxy    = ULIS_CM_Yxy
};

// Extrapolation Method
enum eExtrapolationMethod {
      EM_TRANSPARENT
    , EM_CONSTANT
    , EM_EXTEND
    , EM_WRAP
    , EM_MIRROR
};

ULIS_NAMESPACE_END

