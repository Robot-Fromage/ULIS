// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasPrettyColor.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasPrettyColor class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasPrettyColor.h"

ULIS_NAMESPACE_BEGIN
IHasPrettyColor::IHasPrettyColor(
      const FColor& iColor
    , const FOnColorChanged& iDelegate
)
    : FOnColorChanged( iDelegate )
    , mColor( iColor )
{
    Invoke( mColor );
}

const FColor&
IHasPrettyColor::PrettyColor() const {
    // supposed to be always RGBA8
    return  mColor;
}

void
IHasPrettyColor::SetPrettyColor( const FColor& iValue ) {
    // Ensure conversion to local mPrettyColor format,
    // supposed to be always RGBA8
    FColor::ConvertFormat( iValue, mColor );
    Invoke( mColor );
}

ULIS_NAMESPACE_END

