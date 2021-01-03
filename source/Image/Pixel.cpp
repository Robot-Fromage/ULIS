// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Pixel.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FPixel class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/Pixel.h"
#include "Image/Color.h"

ULIS_NAMESPACE_BEGIN
FPixel::~FPixel()
{
}

FPixel::FPixel( uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace )
    : ISample( iData, iFormat, iColorSpace )
{
    ULIS_ASSERT( iData, "Bad data provided." );
}

FPixel::FPixel( const uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace )
    : ISample( const_cast< uint8* >( iData ), iFormat, iColorSpace )
{
    ULIS_ASSERT( iData, "Bad data provided." );
}

FPixel::FPixel( const FPixel& iValue )
    : ISample( const_cast< uint8* >( iValue.mSignal ), iValue.Format(), iValue.ColorSpace() )
{
}

FPixel::FPixel( const ISample& iSample )
    : ISample( const_cast< uint8* >( iSample.Bits() ), iSample.Format(), iSample.ColorSpace() )
{
}

FPixel::FPixel( FPixel&& iValue )
    : ISample( iValue.mSignal, iValue.Format(), iValue.ColorSpace() )
{
}

FPixel&
FPixel::operator=( const FPixel& iValue ) {
    ReinterpretFormat( iValue.Format() );
    AssignColorSpace( iValue.ColorSpace() );
    mSignal = iValue.mSignal;
    return  *this;
}

void
FPixel::SetPointer( uint8* iPtr )
{
    mSignal = iPtr;
}

ULIS_NAMESPACE_END

