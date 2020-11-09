// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Clear.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Clear invocations.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX2_SUPPORT
void InvokeFillMTProcessScanline_AX2( uint8* iDst, const uint32 iCount, const uint32 iStride );
#endif // ULIS_COMPILETIME_AVX2_SUPPORT
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE42_SUPPORT
void InvokeFillMTProcessScanline_SSE4_2( uint8* iDst, const uint32 iCount, const uint32 iStride );
#endif // __SE4_2__
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void InvokeFillMTProcessScanline_MEM( uint8* iDst, const uint32 iCount, const uint32 iStride );
ULIS_NAMESPACE_END

