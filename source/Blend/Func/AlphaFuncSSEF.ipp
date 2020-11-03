// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         AlphaFuncSSEF.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Vec4f Blending functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Maths/Maths.h"
#include <vectorclass.h>

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Standard Alpha Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaNormalSSEF( Vec4f iCs, Vec4f iCb ) {
    return ( iCb + iCs ) - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Erase
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaEraseSSEF( Vec4f iCs, Vec4f iCb ) {
    return ( 1.f - iCs ) * iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Back
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaBackSSEF( Vec4f iCs, Vec4f iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaTopSSEF( Vec4f iCs, Vec4f iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Sub
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSubSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( iCb - iCs, Vec4f( 0 ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaAddSSEF( Vec4f iCs, Vec4f iCb ) {
    return  min( iCb + iCs, Vec4f( 1.f ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Mul
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaMulSSEF( Vec4f iCs, Vec4f iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Min
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaMinSSEF( Vec4f iCs, Vec4f iCb ) {
    return  min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Max
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaMaxSSEF( Vec4f iCs, Vec4f iCb ) {
    return  max( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------- AlphaF Template Selector
template< eAlphaMode _AM >
ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF( Vec4f iCs, Vec4f iCb ) {
    ULIS2_ASSERT( false, "Alpha Specialization Not Implemented" );
    return  Vec4f( 0.f );
}
template<> ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF< AM_NORMAL   >( Vec4f iCs, Vec4f iCb ) { return  AlphaNormalSSEF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF< AM_ERASE    >( Vec4f iCs, Vec4f iCb ) { return  AlphaEraseSSEF(  iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF< AM_TOP      >( Vec4f iCs, Vec4f iCb ) { return  AlphaTopSSEF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF< AM_BACK     >( Vec4f iCs, Vec4f iCb ) { return  AlphaBackSSEF(   iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF< AM_SUB      >( Vec4f iCs, Vec4f iCb ) { return  AlphaSubSSEF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF< AM_ADD      >( Vec4f iCs, Vec4f iCb ) { return  AlphaAddSSEF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF< AM_MUL      >( Vec4f iCs, Vec4f iCb ) { return  AlphaMulSSEF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF< AM_MIN      >( Vec4f iCs, Vec4f iCb ) { return  AlphaMinSSEF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec4f ULIS2_VECTORCALL AlphaSSEF< AM_MAX      >( Vec4f iCs, Vec4f iCb ) { return  AlphaMaxSSEF(    iCs, iCb ); }
ULIS2_NAMESPACE_END

