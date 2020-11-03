// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         AlphaFuncF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the float Alpha Modes functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Maths/Maths.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Standard Alpha Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS3_FORCEINLINE ufloat AlphaNormalF( ufloat iCs, ufloat iCb ) {
    return ( iCb + iCs ) - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Erase
ULIS3_FORCEINLINE ufloat AlphaEraseF( ufloat iCs, ufloat iCb ) {
    return ( 1.f - iCs ) * iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Back
ULIS3_FORCEINLINE ufloat AlphaBackF( ufloat iCs, ufloat iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS3_FORCEINLINE ufloat AlphaTopF( ufloat iCs, ufloat iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Sub
ULIS3_FORCEINLINE ufloat AlphaSubF( ufloat iCs, ufloat iCb ) {
    return  iCb > iCs ? iCb - iCs : 0;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS3_FORCEINLINE ufloat AlphaAddF( ufloat iCs, ufloat iCb ) {
    return  ( 1.f - iCb ) > iCs ? iCb + iCs : 1.f;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Mul
ULIS3_FORCEINLINE ufloat AlphaMulF( ufloat iCs, ufloat iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Min
ULIS3_FORCEINLINE ufloat AlphaMinF( ufloat iCs, ufloat iCb ) {
    return  FMaths::Min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Max
ULIS3_FORCEINLINE ufloat AlphaMaxF( ufloat iCs, ufloat iCb ) {
    return  FMaths::Max( iCs, iCb );
}

/////////////////////////////////////////////////////
// AlphaF Template Selector
//--------------------------------------------------------------------------------------
//----------------------------------------------------- Generic AlphaF Template Selector
template< eAlphaMode _AM >
ULIS3_FORCEINLINE ufloat AlphaF( ufloat iCs, ufloat iCb ) {
    ULIS3_ASSERT( false, "Alpha Specialization Not Implemented" );
    return  0.f;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------- AlphaF Template Selector Specializations
template<> ULIS3_FORCEINLINE ufloat AlphaF< AM_NORMAL   >( ufloat iCs, ufloat iCb ) { return  AlphaNormalF( iCs, iCb ); }
template<> ULIS3_FORCEINLINE ufloat AlphaF< AM_ERASE    >( ufloat iCs, ufloat iCb ) { return  AlphaEraseF(  iCs, iCb ); }
template<> ULIS3_FORCEINLINE ufloat AlphaF< AM_TOP      >( ufloat iCs, ufloat iCb ) { return  AlphaTopF(    iCs, iCb ); }
template<> ULIS3_FORCEINLINE ufloat AlphaF< AM_BACK     >( ufloat iCs, ufloat iCb ) { return  AlphaBackF(   iCs, iCb ); }
template<> ULIS3_FORCEINLINE ufloat AlphaF< AM_SUB      >( ufloat iCs, ufloat iCb ) { return  AlphaSubF(    iCs, iCb ); }
template<> ULIS3_FORCEINLINE ufloat AlphaF< AM_ADD      >( ufloat iCs, ufloat iCb ) { return  AlphaAddF(    iCs, iCb ); }
template<> ULIS3_FORCEINLINE ufloat AlphaF< AM_MUL      >( ufloat iCs, ufloat iCb ) { return  AlphaMulF(    iCs, iCb ); }
template<> ULIS3_FORCEINLINE ufloat AlphaF< AM_MIN      >( ufloat iCs, ufloat iCb ) { return  AlphaMinF(    iCs, iCb ); }
template<> ULIS3_FORCEINLINE ufloat AlphaF< AM_MAX      >( ufloat iCs, ufloat iCb ) { return  AlphaMaxF(    iCs, iCb ); }

ULIS3_NAMESPACE_END

