// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         srgb2linear.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for the srgb2linear functions.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
ufloat    srgb2linear( ufloat iValue );
ufloat    linear2srgb( ufloat iValue );
uint8     fast_srgb2linearU8( uint8 iValue );
uint8     fast_linear2srgbU8( uint8 iValue );
ufloat    fast_srgb2linearF( uint8 iValue );
ufloat    fast_linear2srgbF( uint8 iValue );

template< typename T > ULIS_FORCEINLINE T sel_srgb2linearT( T iValue ) { return  ConvType< float, T >( srgb2linear( ConvType< T, float >( iValue ) ) ); }
template<> ULIS_FORCEINLINE float sel_srgb2linearT< float >( float iValue ) { return  srgb2linear( iValue ); }
template<> ULIS_FORCEINLINE uint8 sel_srgb2linearT< uint8 >( uint8 iValue ) { return  fast_srgb2linearU8( iValue ); }
template< typename T > ULIS_FORCEINLINE T sel_linear2srgbT( T iValue ) { return  ConvType< float, T >( linear2srgb( ConvType< T, float >( iValue ) ) ); }
template<> ULIS_FORCEINLINE float sel_linear2srgbT< float >( float iValue ) { return  linear2srgb( iValue ); }
template<> ULIS_FORCEINLINE uint8 sel_linear2srgbT< uint8 >( uint8 iValue ) { return  fast_linear2srgbU8( iValue ); }
ULIS_NAMESPACE_END

