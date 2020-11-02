// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
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
#define U2_DREF_RED_CHAN( T, iPtr, iFmt, iChan )    ( *( ( T* )( iPtr ) + iFmt.IDT[ iChan ] ) )
#define U2_DREF_CHAN( T, iPtr, iChan )              ( *( ( T* )( iPtr ) + iChan ) )
#define U2_FWD_ALPHA                                if( iDstFormat.HEA ) { U2_DREF_CHAN( U, iDst, iDstFormat.AID ) = iSrcFormat.HEA? ConvType< T, U >( U2_DREF_CHAN( T, iSrc, iSrcFormat.AID ) ) : MaxType< U >(); }

#define U2_DREF_SRC( iChan )                        U2_DREF_RED_CHAN( T, iSrc, iSrcFormat, iChan )
#define U2_DREF_DST( iChan )                        U2_DREF_RED_CHAN( U, iDst, iDstFormat, iChan )
#define U2_DREF_TEMP( iChan )                       U2_DREF_RED_CHAN( ufloat, temp.Bits(), temp.FormatMetrics(), iChan )
