// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AlphaFuncSSEF.h
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Vec4f Alpha Modes functions.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Standard Alpha Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaNormalSSEF( Vec4f iCs, Vec4f iCb ) {
    return ( iCb + iCs ) - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Erase
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaEraseSSEF( Vec4f iCs, Vec4f iCb ) {
    return ( 1.f - iCs ) * iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Back
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaBackSSEF( Vec4f iCs, Vec4f iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaTopSSEF( Vec4f iCs, Vec4f iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Sub
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSubSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( iCb - iCs, Vec4f( 0 ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaAddSSEF( Vec4f iCs, Vec4f iCb ) {
    return  min( iCb + iCs, Vec4f( 1.f ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Mul
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaMulSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Min
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaMinSSEF( Vec4f iCs, Vec4f iCb ) {
    return  min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Max
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaMaxSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( iCs, iCb );
}

/////////////////////////////////////////////////////
// AlphaSSEF Template Selector
//--------------------------------------------------------------------------------------
//-------------------------------------------------- Generic AlphaSSEF Template Selector
template< eAlphaMode _AM >
ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF( Vec4f iCs, Vec4f iCb ) {
    ULIS_ASSERT( false, "Alpha Specialization Not Implemented" );
    return  Vec4f( 0.f );
}

//--------------------------------------------------------------------------------------
//------------------------------------------ AlphaSSEF Template Selector Specializations
template<> ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF< Alpha_Normal    >( Vec4f iCs, Vec4f iCb ) { return  AlphaNormalSSEF(    iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF< Alpha_Erase     >( Vec4f iCs, Vec4f iCb ) { return  AlphaEraseSSEF(     iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF< Alpha_Top       >( Vec4f iCs, Vec4f iCb ) { return  AlphaTopSSEF(       iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF< Alpha_Back      >( Vec4f iCs, Vec4f iCb ) { return  AlphaBackSSEF(      iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF< Alpha_Sub       >( Vec4f iCs, Vec4f iCb ) { return  AlphaSubSSEF(       iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF< Alpha_Add       >( Vec4f iCs, Vec4f iCb ) { return  AlphaAddSSEF(       iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF< Alpha_Mul       >( Vec4f iCs, Vec4f iCb ) { return  AlphaMulSSEF(       iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF< Alpha_Min       >( Vec4f iCs, Vec4f iCb ) { return  AlphaMinSSEF(       iCs, iCb ); }
template<> ULIS_FORCEINLINE Vec4f ULIS_VECTORCALL AlphaSSEF< Alpha_Max       >( Vec4f iCs, Vec4f iCb ) { return  AlphaMaxSSEF(       iCs, iCb ); }

ULIS_NAMESPACE_END

