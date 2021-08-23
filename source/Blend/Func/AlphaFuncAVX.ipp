// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         AlphaFuncAVX.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Vec8f Blending functions.
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
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaNormalAVXF( Vec8f iCs, Vec8f iCb ) {
    return ( iCb + iCs ) - ( iCb * iCs );
}
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Erase
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaEraseAVXF( Vec8f iCs, Vec8f iCb ) {
    return ( 1.f - iCs ) * iCb;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------- Back
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaBackAVXF( Vec8f iCs, Vec8f iCb ) {
    return iCb;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Top
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaTopAVXF( Vec8f iCs, Vec8f iCb ) {
    return iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Sub
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaSubAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( iCb - iCs, Vec8f( 0 ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Add
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAddAVXF( Vec8f iCs, Vec8f iCb ) {
    return  min( iCb + iCs, Vec8f( 1.f ) );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Mul
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaMulAVXF( Vec8f iCs, Vec8f iCb ) {
    return  iCb * iCs;
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Min
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaMinAVXF( Vec8f iCs, Vec8f iCb ) {
    return  min( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Max
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaMaxAVXF( Vec8f iCs, Vec8f iCb ) {
    return  max( iCs, iCb );
}
//--------------------------------------------------------------------------------------
//------------------------------------------------------------- AlphaF Template Selector
template< eAlphaMode _AM >
ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF( Vec8f iCs, Vec8f iCb ) {
    ULIS2_ASSERT( false, "Alpha Specialization Not Implemented" );
    return  Vec8f( 0.f );
}
template<> ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF< AM_NORMAL   >( Vec8f iCs, Vec8f iCb ) { return  AlphaNormalAVXF( iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF< AM_ERASE    >( Vec8f iCs, Vec8f iCb ) { return  AlphaEraseAVXF(  iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF< AM_TOP      >( Vec8f iCs, Vec8f iCb ) { return  AlphaTopAVXF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF< AM_BACK     >( Vec8f iCs, Vec8f iCb ) { return  AlphaBackAVXF(   iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF< AM_SUB      >( Vec8f iCs, Vec8f iCb ) { return  AlphaSubAVXF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF< AM_ADD      >( Vec8f iCs, Vec8f iCb ) { return  AlphaAddAVXF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF< AM_MUL      >( Vec8f iCs, Vec8f iCb ) { return  AlphaMulAVXF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF< AM_MIN      >( Vec8f iCs, Vec8f iCb ) { return  AlphaMinAVXF(    iCs, iCb ); }
template<> ULIS2_FORCEINLINE Vec8f ULIS2_VECTORCALL AlphaAVXF< AM_MAX      >( Vec8f iCs, Vec8f iCb ) { return  AlphaMaxAVXF(    iCs, iCb ); }
ULIS2_NAMESPACE_END

