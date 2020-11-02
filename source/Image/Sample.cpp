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

ULIS_NAMESPACE_END

