// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         ConvHelpers.h
* @author       Clement Berthaud
* @brief        This file provides some conversion helper macros.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

/////////////////////////////////////////////////////
// Macro Helpers for Redundant Conversion Operations
// Macro utils for implementations
#define U2_DREF_RED_CHAN( T, iPtr, iFmt, iChan )    ( *( ( T* )( iPtr ) + iFmt->IDT[ iChan ] ) )
#define U2_DREF_CHAN( T, iPtr, iChan )              ( *( ( T* )( iPtr ) + iChan ) )
#define U2_FWD_ALPHA                                if( iDstFormat->HEA ) { U2_DREF_CHAN( T2, iDst, iDstFormat->AID ) = iSrcFormat->HEA? ConvType< T1, T2 >( U2_DREF_CHAN( T1, iSrc, iSrcFormat->AID ) ) : MaxType< T2 >(); }

#define U2_DREF_SRC( iChan )                        U2_DREF_RED_CHAN( T1, iSrc, iSrcFormat, iChan )
#define U2_DREF_DST( iChan )                        U2_DREF_RED_CHAN( T2, iDst, iDstFormat, iChan )
#define U2_DREF_TEMP( iChan )                       U2_DREF_RED_CHAN( ufloat, temp.Ptr(), &temp.FormatInfo(), iChan )
