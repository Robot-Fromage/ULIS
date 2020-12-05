// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Sample.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the ISample classes.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/Sample.h"
#include "Image/Color.h"
#include "Conv/ConvertFormatInvocations.h"

ULIS_NAMESPACE_BEGIN
ISample::~ISample()
{}

ISample::ISample( uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace )
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , mSignal( iData )
{
}

bool
ISample::operator==( const  ISample& iOther )  const {

    if( Format() != iOther.Format() )
        return  false;

    bool bytePerfectMatch = true;
    for( int i = 0; i < BytesPerPixel(); ++i ) {
        if( mSignal[i] != iOther.mSignal[i] ) {
            bytePerfectMatch = false;
            break;
        }
    }

    return  bytePerfectMatch;
}

bool
ISample::operator!=( const  ISample& iOther )  const {
    return  !( *this == iOther );
}

uint8*
ISample::Bits()
{
    return  mSignal;
}

const uint8*
ISample::Bits() const
{
    return  mSignal;
}

FColor
ISample::ToFormat( eFormat iDstFormat ) const
{
    FColor dst( iDstFormat );
    if( Format() == iDstFormat ) {
        memcpy( dst.Bits(), Bits(), dst.BytesPerPixel() );
    } else {
        fpConvertFormat fptr = QueryDispatchConvertFormatInvocation( Format(), iDstFormat );
        fptr( FormatMetrics(), Bits(), dst.FormatMetrics(), dst.Bits(), 1 );
    }
    return  dst;
}

//static
void
ISample::ConvertFormat( const ISample& iSrc, ISample& iDst )
{
    if( iSrc.Format() == iDst.Format() ) {
        memcpy( iDst.Bits(), iSrc.Bits(), iDst.BytesPerPixel() );
    } else {
        fpConvertFormat fptr = QueryDispatchConvertFormatInvocation( iSrc.Format(), iDst.Format() );
        fptr( iSrc.FormatMetrics(), iSrc.Bits(), iDst.FormatMetrics(), iDst.Bits(), 1 );
    }
}

ULIS_NAMESPACE_END

