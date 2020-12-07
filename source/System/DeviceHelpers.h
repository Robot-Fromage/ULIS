// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         DeviceHelpers.h
* @author       Clement Berthaud
* @brief        This file provides the definition for FHardwareMetrics macro helpers tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
/////////////////////////////////////////////////////
// Device info bit field "bField" indexed in base 64
//
//   64|      60|  56|  52|  48|      44|  40|  36|  32|      28|  24|  20|  16|      12|   8|   4|   0
//          •••• •••• •••• ••••     •••• •••• •••• ••••     •••• •••• •••• ••••     •••• •••• •••• ••••
//          /+98 7654 3210 zyxw     vuts rqpo nmlk jihg     fedc baZY XWVU TSRQ     PONM LKJI HGFE DCBA
//
//          bit index caption:
//          A: Is Hardware AMD;
//          B: Is Hardware Intel;
//          C: Is OS x64;
//          D: Has OS AVX;
//          E: Has OS AVX512;
//          F: Has Hardware x64;
//          G: Has Hardware MMX;
//          H: Has Hardware ABM;
//          I: Has Hardware RDRAND;
//          J: Has Hardware BMI1;
//          K: Has Hardware BMI2;
//          L: Has Hardware ADX;
//          M: Has Hardware PREFETCHWT1;
//          N: Has Hardware MPX;
//          O: Has Hardware SSE;
//          P: Has Hardware SSE2;
//          Q: Has Hardware SSE3;
//          R: Has Hardware SSSE3;
//          S: Has Hardware SSE41;
//          T: Has Hardware SSE42;
//          U: Has Hardware SSE4a;
//          V: Has Hardware AES;
//          W: Has Hardware SHA;
//          X: Has Hardware AVX;
//          Y: Has Hardware XOP;
//          Z: Has Hardware FMA3;
//          a: Has Hardware FMA4;
//          b: Has Hardware AVX2;
//          c: Has Hardware AVX512_F;
//          d: Has Hardware AVX512_PF;
//          e: Has Hardware AVX512_ER;
//          f: Has Hardware AVX512_CD;
//          g: Has Hardware AVX512_VL;
//          h: Has Hardware AVX512_BW;
//          i: Has Hardware AVX512_DQ;
//          j: Has Hardware AVX512_IFMA;
//          k: Has Hardware AVX512_VBMI;
//          0: __reserved__
//          1: __reserved__
//          2: __reserved__
//          3: __reserved__
//          4: __reserved__
//          5: __reserved__
//          6: __reserved__
//          7: __reserved__
//          8: __reserved__
//          9: __reserved__
//          +: __reserved__
//          /: __reserved__
/////////////////////////////////////////////////////
// Bitfield Macro Operators
//Write
#define ULIS_W_HW_AMD( i )          ( ( ( i ) & 1 ) << 0    )
#define ULIS_W_HW_INTEL( i )        ( ( ( i ) & 1 ) << 1    )
#define ULIS_W_OS_X64( i )          ( ( ( i ) & 1 ) << 2    )
#define ULIS_W_OS_AVX( i )          ( ( ( i ) & 1 ) << 3    )
#define ULIS_W_OS_AVX512( i )       ( ( ( i ) & 1 ) << 4    )
#define ULIS_W_HW_X64( i )          ( ( ( i ) & 1 ) << 5    )
#define ULIS_W_HW_MMX( i )          ( ( ( i ) & 1 ) << 6    )
#define ULIS_W_HW_ABM( i )          ( ( ( i ) & 1 ) << 7    )
#define ULIS_W_HW_RDRAND( i )       ( ( ( i ) & 1 ) << 8    )
#define ULIS_W_HW_BMI1( i )         ( ( ( i ) & 1 ) << 9    )
#define ULIS_W_HW_BMI2( i )         ( ( ( i ) & 1 ) << 10   )
#define ULIS_W_HW_ADX( i )          ( ( ( i ) & 1 ) << 11   )
#define ULIS_W_HW_PREFETCHWT1( i )  ( ( ( i ) & 1 ) << 12   )
#define ULIS_W_HW_MPX( i )          ( ( ( i ) & 1 ) << 13   )
#define ULIS_W_HW_SSE( i )          ( ( ( i ) & 1 ) << 14   )
#define ULIS_W_HW_SSE2( i )         ( ( ( i ) & 1 ) << 15   )
#define ULIS_W_HW_SSE3( i )         ( ( ( i ) & 1 ) << 16   )
#define ULIS_W_HW_SSSE3( i )        ( ( ( i ) & 1 ) << 17   )
#define ULIS_W_HW_SSE41( i )        ( ( ( i ) & 1 ) << 18   )
#define ULIS_W_HW_SSE42( i )        ( ( ( i ) & 1 ) << 19   )
#define ULIS_W_HW_SSE4A( i )        ( ( ( i ) & 1 ) << 20   )
#define ULIS_W_HW_AES( i )          ( ( ( i ) & 1 ) << 21   )
#define ULIS_W_HW_SHA( i )          ( ( ( i ) & 1 ) << 22   )
#define ULIS_W_HW_AVX( i )          ( ( ( i ) & 1 ) << 23   )
#define ULIS_W_HW_XOP( i )          ( ( ( i ) & 1 ) << 24   )
#define ULIS_W_HW_FMA3( i )         ( ( ( i ) & 1 ) << 25   )
#define ULIS_W_HW_FMA4( i )         ( ( ( i ) & 1 ) << 26   )
#define ULIS_W_HW_AVX2( i )         ( ( ( i ) & 1 ) << 27   )
#define ULIS_W_HW_AVX512_F( i )     ( ( ( i ) & 1 ) << 28   )
#define ULIS_W_HW_AVX512_PF( i )    ( ( ( i ) & 1 ) << 29   )
#define ULIS_W_HW_AVX512_ER( i )    ( ( ( i ) & 1 ) << 30   )
#define ULIS_W_HW_AVX512_CD( i )    ( ( ( i ) & 1 ) << 31   )
#define ULIS_W_HW_AVX512_VL( i )    ( ( ( i ) & 1 ) << 32   )
#define ULIS_W_HW_AVX512_BW( i )    ( ( ( i ) & 1 ) << 33   )
#define ULIS_W_HW_AVX512_DQ( i )    ( ( ( i ) & 1 ) << 34   )
#define ULIS_W_HW_AVX512_IFMA( i )  ( ( ( i ) & 1 ) << 35   )
#define ULIS_W_HW_AVX512_VBMI( i )  ( ( ( i ) & 1 ) << 36   )
// Read
#define ULIS_R_HW_AMD( i )          ( ( ( i ) >> 0 )    & 1 )
#define ULIS_R_HW_INTEL( i )        ( ( ( i ) >> 1 )    & 1 )
#define ULIS_R_OS_X64( i )          ( ( ( i ) >> 2 )    & 1 )
#define ULIS_R_OS_AVX( i )          ( ( ( i ) >> 3 )    & 1 )
#define ULIS_R_OS_AVX512( i )       ( ( ( i ) >> 4 )    & 1 )
#define ULIS_R_HW_X64( i )          ( ( ( i ) >> 5 )    & 1 )
#define ULIS_R_HW_MMX( i )          ( ( ( i ) >> 6 )    & 1 )
#define ULIS_R_HW_ABM( i )          ( ( ( i ) >> 7 )    & 1 )
#define ULIS_R_HW_RDRAND( i )       ( ( ( i ) >> 8 )    & 1 )
#define ULIS_R_HW_BMI1( i )         ( ( ( i ) >> 9 )    & 1 )
#define ULIS_R_HW_BMI2( i )         ( ( ( i ) >> 10 )   & 1 )
#define ULIS_R_HW_ADX( i )          ( ( ( i ) >> 11 )   & 1 )
#define ULIS_R_HW_PREFETCHWT1( i )  ( ( ( i ) >> 12 )   & 1 )
#define ULIS_R_HW_MPX( i )          ( ( ( i ) >> 13 )   & 1 )
#define ULIS_R_HW_SSE( i )          ( ( ( i ) >> 14 )   & 1 )
#define ULIS_R_HW_SSE2( i )         ( ( ( i ) >> 15 )   & 1 )
#define ULIS_R_HW_SSE3( i )         ( ( ( i ) >> 16 )   & 1 )
#define ULIS_R_HW_SSSE3( i )        ( ( ( i ) >> 17 )   & 1 )
#define ULIS_R_HW_SSE41( i )        ( ( ( i ) >> 18 )   & 1 )
#define ULIS_R_HW_SSE42( i )        ( ( ( i ) >> 19 )   & 1 )
#define ULIS_R_HW_SSE4A( i )        ( ( ( i ) >> 20 )   & 1 )
#define ULIS_R_HW_AES( i )          ( ( ( i ) >> 21 )   & 1 )
#define ULIS_R_HW_SHA( i )          ( ( ( i ) >> 22 )   & 1 )
#define ULIS_R_HW_AVX( i )          ( ( ( i ) >> 23 )   & 1 )
#define ULIS_R_HW_XOP( i )          ( ( ( i ) >> 24 )   & 1 )
#define ULIS_R_HW_FMA3( i )         ( ( ( i ) >> 25 )   & 1 )
#define ULIS_R_HW_FMA4( i )         ( ( ( i ) >> 26 )   & 1 )
#define ULIS_R_HW_AVX2( i )         ( ( ( i ) >> 27 )   & 1 )
#define ULIS_R_HW_AVX512_F( i )     ( ( ( i ) >> 28 )   & 1 )
#define ULIS_R_HW_AVX512_PF( i )    ( ( ( i ) >> 29 )   & 1 )
#define ULIS_R_HW_AVX512_ER( i )    ( ( ( i ) >> 30 )   & 1 )
#define ULIS_R_HW_AVX512_CD( i )    ( ( ( i ) >> 31 )   & 1 )
#define ULIS_R_HW_AVX512_VL( i )    ( ( ( i ) >> 32 )   & 1 )
#define ULIS_R_HW_AVX512_BW( i )    ( ( ( i ) >> 33 )   & 1 )
#define ULIS_R_HW_AVX512_DQ( i )    ( ( ( i ) >> 34 )   & 1 )
#define ULIS_R_HW_AVX512_IFMA( i )  ( ( ( i ) >> 35 )   & 1 )
#define ULIS_R_HW_AVX512_VBMI( i )  ( ( ( i ) >> 36 )   & 1 )

