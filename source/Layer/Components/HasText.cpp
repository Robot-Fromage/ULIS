// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasText.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasText class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasText.h"

ULIS_NAMESPACE_BEGIN

IHasText::IHasText(
      const FWString& iString
    , const FFont& iFont
    , int iSize
    , const FColor& iColor
    , const FOnTextInfoChanged& iDelegate
)
    : TCallbackCapable< FOnTextInfoChanged >( iDelegate )
    , mInfo{
          iString
        , iFont
        , iSize
        , iColor
    }
{
    OnChanged( mInfo );
}

FWString IHasText::Text() const
{
    return mInfo.string;
}

FFont IHasText::Font() const
{
    return mInfo.font;
}

int IHasText::FontSize() const
{
    return mInfo.size;
}

FColor IHasText::TextColor() const
{
    return mInfo.color;
}

void IHasText::SetText( const FWString& iValue )
{
    mInfo.string = iValue;
    OnChanged( mInfo );
}

void IHasText::SetFont( const FFont& iValue )
{
    mInfo.font = iValue;
    OnChanged( mInfo );
}

void IHasText::SetFontSize(  int iValue )
{
    mInfo.size = iValue;
    OnChanged( mInfo );
}

void IHasText::SetTextColor( const FColor& iValue )
{
    mInfo.color = iValue;
    OnChanged( mInfo );
}

ULIS_NAMESPACE_END

