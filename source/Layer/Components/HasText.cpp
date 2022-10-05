// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasText.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasText class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasText.h"

ULIS_NAMESPACE_BEGIN

IHasText::IHasText(
      const FWString& iString
    , const FFont& iFont
    , int iSize
    , const FColor& iColor
    , bool iAntiAliased
    , const FOnTextInfoChanged& iDelegate
)
    : FOnTextInfoChanged( iDelegate )
    , mInfo{
          iString
        , iFont
        , iSize
        , iColor
        , iAntiAliased
    }
{
    Invoke( mInfo );
}

FWString IHasText::Text() const
{
    return mInfo.string;
}

const FFont& IHasText::Font() const
{
    return mInfo.font;
}

int IHasText::FontSize() const
{
    return mInfo.size;
}

const FColor& IHasText::TextColor() const
{
    return mInfo.color;
}

bool IHasText::IsAntiAliased() const
{
    return mInfo.isAntiAliased;
}

void IHasText::SetText( const FWString& iValue )
{
    mInfo.string = iValue;
    Invoke( mInfo );
}

void IHasText::SetFont( const FFont& iValue )
{
    mInfo.font = iValue;
    Invoke( mInfo );
}

void IHasText::SetFontSize(  int iValue )
{
    mInfo.size = iValue;
    Invoke( mInfo );
}

void IHasText::SetTextColor( const FColor& iValue )
{
    mInfo.color = iValue;
    Invoke( mInfo );
}

void IHasText::SetAntiAliased( bool iValue )
{
    mInfo.isAntiAliased = iValue;
    Invoke( mInfo );
}

ULIS_NAMESPACE_END

