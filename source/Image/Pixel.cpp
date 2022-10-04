// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Pixel.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FPixel class.
* @license      Please refer to LICENSE.md
*/
#include "Image/Pixel.h"
#include "Image/Color.h"

ULIS_NAMESPACE_BEGIN
FPixel::~FPixel()
{
}

FPixel::FPixel( uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace, uint64 iPlaneSize )
    : ISample( iData, iFormat, iColorSpace, iPlaneSize )
{
    ULIS_ASSERT( iData, "Bad data provided." );
}

FPixel::FPixel( const uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace, uint64 iPlaneSize )
    : ISample( const_cast< uint8* >( iData ), iFormat, iColorSpace, iPlaneSize )
{
    ULIS_ASSERT( iData, "Bad data provided." );
}

FPixel::FPixel( const FPixel& iValue )
    : ISample( const_cast< uint8* >( iValue.mSignal ), iValue.Format(), iValue.ColorSpace(), iValue.mPlaneSize )
{
}

FPixel::FPixel( const ISample& iSample )
    : ISample( const_cast< uint8* >( iSample.Bits() ), iSample.Format(), iSample.ColorSpace(), iSample.PlaneSize() )
{
}

FPixel::FPixel( FPixel&& iValue )
    : ISample( iValue.mSignal, iValue.Format(), iValue.ColorSpace(), iValue.mPlaneSize )
{
}

FPixel&
FPixel::operator=( const FPixel& iValue ) {
    ReinterpretFormat( iValue.Format() );
    AssignColorSpace( iValue.ColorSpace() );
    mSignal = iValue.mSignal;
    mPlaneSize = iValue.mPlaneSize;
    return  *this;
}

void
FPixel::SetPointer( uint8* iPtr )
{
    mSignal = iPtr;
}

ULIS_NAMESPACE_END

