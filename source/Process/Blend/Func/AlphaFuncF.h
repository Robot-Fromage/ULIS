// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AlphaFuncF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the float Alpha Modes functions.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Standard Alpha Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS_FORCEINLINE ufloat AlphaNormalF( ufloat iCs, ufloat iCb ) {
    return ( iCb + iCs ) - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Erase
ULIS_FORCEINLINE ufloat AlphaEraseF( ufloat iCs, ufloat iCb ) {
    return ( 1.f - iCs ) * iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Back
ULIS_FORCEINLINE ufloat AlphaBackF( ufloat iCs, ufloat iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS_FORCEINLINE ufloat AlphaTopF( ufloat iCs, ufloat iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Sub
ULIS_FORCEINLINE ufloat AlphaSubF( ufloat iCs, ufloat iCb ) {
    return  iCb > iCs ? iCb - iCs : 0;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS_FORCEINLINE ufloat AlphaAddF( ufloat iCs, ufloat iCb ) {
    return  ( 1.f - iCb ) > iCs ? iCb + iCs : 1.f;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Mul
ULIS_FORCEINLINE ufloat AlphaMulF( ufloat iCs, ufloat iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Min
ULIS_FORCEINLINE ufloat AlphaMinF( ufloat iCs, ufloat iCb ) {
    return  FMath::Min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Max
ULIS_FORCEINLINE ufloat AlphaMaxF( ufloat iCs, ufloat iCb ) {
    return  FMath::Max( iCs, iCb );
}

/////////////////////////////////////////////////////
// AlphaF Template Selector
//--------------------------------------------------------------------------------------
//----------------------------------------------------- Generic AlphaF Template Selector
template< eAlphaMode _AM >
ULIS_FORCEINLINE ufloat AlphaF( ufloat iCs, ufloat iCb ) {
    ULIS_ASSERT( false, "Alpha Specialization Not Implemented" );
    return  0.f;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------- AlphaF Template Selector Specializations
template<> ULIS_FORCEINLINE ufloat AlphaF< Alpha_Normal   >( ufloat iCs, ufloat iCb ) { return  AlphaNormalF( iCs, iCb ); }
template<> ULIS_FORCEINLINE ufloat AlphaF< Alpha_Erase    >( ufloat iCs, ufloat iCb ) { return  AlphaEraseF(  iCs, iCb ); }
template<> ULIS_FORCEINLINE ufloat AlphaF< Alpha_Top      >( ufloat iCs, ufloat iCb ) { return  AlphaTopF(    iCs, iCb ); }
template<> ULIS_FORCEINLINE ufloat AlphaF< Alpha_Back     >( ufloat iCs, ufloat iCb ) { return  AlphaBackF(   iCs, iCb ); }
template<> ULIS_FORCEINLINE ufloat AlphaF< Alpha_Sub      >( ufloat iCs, ufloat iCb ) { return  AlphaSubF(    iCs, iCb ); }
template<> ULIS_FORCEINLINE ufloat AlphaF< Alpha_Add      >( ufloat iCs, ufloat iCb ) { return  AlphaAddF(    iCs, iCb ); }
template<> ULIS_FORCEINLINE ufloat AlphaF< Alpha_Mul      >( ufloat iCs, ufloat iCb ) { return  AlphaMulF(    iCs, iCb ); }
template<> ULIS_FORCEINLINE ufloat AlphaF< Alpha_Min      >( ufloat iCs, ufloat iCb ) { return  AlphaMinF(    iCs, iCb ); }
template<> ULIS_FORCEINLINE ufloat AlphaF< Alpha_Max      >( ufloat iCs, ufloat iCb ) { return  AlphaMaxF(    iCs, iCb ); }

ULIS_NAMESPACE_END

