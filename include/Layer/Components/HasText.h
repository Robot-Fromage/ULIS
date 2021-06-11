// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasText.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the IHasText class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Components/TextInfo.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      IHasText
/// @brief      Simple HasText class.
class ULIS_API IHasText
{
protected:
    IHasText( const FWString& string = L"hello world", const FFont& font = FFont::DefaultFont, int size = 12, const FColor& color = FColor::Black );

public:
    FWString Text() const;
    FFont Font() const;
    int FontSize() const;
    FColor TextColor() const;

    void SetText( const FWString& iValue );
    void SetFont( const FFont& iValue );
    void SetFontSize(  int iValue );
    void SetTextColor( const FColor& iValue );

private:
    FTextInfo mInfo;
};

ULIS_NAMESPACE_END

