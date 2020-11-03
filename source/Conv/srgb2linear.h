// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         srgb2linear.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for the srgb2linear functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS3_NAMESPACE_BEGIN
ULIS3_API ufloat    srgb2linear( ufloat iValue );
ULIS3_API ufloat    linear2srgb( ufloat iValue );
ULIS3_API uint8     fast_srgb2linearU8( uint8 iValue );
ULIS3_API uint8     fast_linear2srgbU8( uint8 iValue );
ULIS3_API ufloat    fast_srgb2linearF( uint8 iValue );
ULIS3_API ufloat    fast_linear2srgbF( uint8 iValue );

template< typename T > ULIS3_API ULIS3_FORCEINLINE T sel_srgb2linearT( T iValue ) { return  ConvType< float, T >( srgb2linear( ConvType< T, float >( iValue ) ) ); }
template<> ULIS3_API ULIS3_FORCEINLINE float sel_srgb2linearT< float >( float iValue ) { return  srgb2linear( iValue ); }
template<> ULIS3_API ULIS3_FORCEINLINE uint8 sel_srgb2linearT< uint8 >( uint8 iValue ) { return  fast_srgb2linearU8( iValue ); }
template< typename T > ULIS3_API ULIS3_FORCEINLINE T sel_linear2srgbT( T iValue ) { return  ConvType< float, T >( linear2srgb( ConvType< T, float >( iValue ) ) ); }
template<> ULIS3_API ULIS3_FORCEINLINE float sel_linear2srgbT< float >( float iValue ) { return  linear2srgb( iValue ); }
template<> ULIS3_API ULIS3_FORCEINLINE uint8 sel_linear2srgbT< uint8 >( uint8 iValue ) { return  fast_linear2srgbU8( iValue ); }
ULIS3_NAMESPACE_END

