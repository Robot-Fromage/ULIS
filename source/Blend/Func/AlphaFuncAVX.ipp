// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         AlphaFuncAVX.ipp
* @author       Clement Berthaud
* @brief        This file provides the implementations for the Vec8f Alpha Modes functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Maths/Maths.h"
#include <vectorclass.h>

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Standard Alpha Modes
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------------- Normal
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaNormalAVXF( Vec8f iCs, Vec8f iCb ) {
    return ( iCb + iCs ) - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Erase
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaEraseAVXF( Vec8f iCs, Vec8f iCb ) {
    return ( 1.f - iCs ) * iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Back
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaBackAVXF( Vec8f iCs, Vec8f iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaTopAVXF( Vec8f iCs, Vec8f iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Sub
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaSubAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( iCb - iCs, Vec8f( 0 ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAddAVXF( Vec8f iCs, Vec8f iCb ) {
    return  min( iCb + iCs, Vec8f( 1.f ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Mul
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaMulAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Min
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaMinAVXF( Vec8f iCs, Vec8f iCb ) {
    return  min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Max
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaMaxAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( iCs, iCb );
}

/////////////////////////////////////////////////////
// AlphaAVXF Template Selector
//--------------------------------------------------------------------------------------
//-------------------------------------------------- Generic AlphaAVXF Template Selector
template< eAlphaMode _AM >
ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF( Vec8f iCs, Vec8f iCb ) {
    ULIS3_ASSERT( false, "Alpha Specialization Not Implemented" );
    return  Vec8f( 0.f );
}

//--------------------------------------------------------------------------------------
//------------------------------------------ AlphaAVXF Template Selector Specializations
template<> ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF< AM_NORMAL    >( Vec8f iCs, Vec8f iCb ) { return  AlphaNormalAVXF(    iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF< AM_ERASE     >( Vec8f iCs, Vec8f iCb ) { return  AlphaEraseAVXF(     iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF< AM_TOP       >( Vec8f iCs, Vec8f iCb ) { return  AlphaTopAVXF(       iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF< AM_BACK      >( Vec8f iCs, Vec8f iCb ) { return  AlphaBackAVXF(      iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF< AM_SUB       >( Vec8f iCs, Vec8f iCb ) { return  AlphaSubAVXF(       iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF< AM_ADD       >( Vec8f iCs, Vec8f iCb ) { return  AlphaAddAVXF(       iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF< AM_MUL       >( Vec8f iCs, Vec8f iCb ) { return  AlphaMulAVXF(       iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF< AM_MIN       >( Vec8f iCs, Vec8f iCb ) { return  AlphaMinAVXF(       iCs, iCb ); }
template<> ULIS3_FORCEINLINE Vec8f ULIS3_VECTORCALL AlphaAVXF< AM_MAX       >( Vec8f iCs, Vec8f iCb ) { return  AlphaMaxAVXF(       iCs, iCb ); }

ULIS3_NAMESPACE_END

