// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerText.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerText class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/LayerText.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
FLayerText::~FLayerText()
{
}

FLayerText::FLayerText(
      const FString& iName
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FFont& iFont
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , const std::wstring& iText
    , uint32 iFontSize
    , const FMat3F& iTransform
    , const ISample& iColor
    , bool iAA
    , ILayerRoot* iParent
)
    : ILayer(
          iName
        , iParent
    )
    , FLayerImage(
          iName
        , iWidth
        , iHeight
        , iFormat
        , iBlendMode
        , iAlphaMode
        , iOpacity
        , iParent
    )
    , mText( iText )
    , mFont( iFont )
    , mFontSize( iFontSize )
    , mTransform( iTransform )
    , mColor( iColor )
    , mAA( iAA )
{
}

FLayerText::FLayerText(
      FBlock* iBlock
    , const FString& iName
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FFont& iFont
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , const std::wstring& iText
    , uint32 iFontSize
    , const FMat3F& iTransform
    , const ISample& iColor
    , bool iAA
    , ILayerRoot* iParent
)
    : ILayer(
          iName
        , iParent
    )
    , FLayerImage(
          iBlock
        , iName
        , iWidth
        , iHeight
        , iFormat
        , iBlendMode
        , iAlphaMode
        , iOpacity
        , iParent
    )
    , mText( iText )
    , mFont( iFont )
    , mFontSize( iFontSize )
    , mTransform( iTransform )
    , mColor( iColor )
    , mAA( iAA )
{
}

eLayerType
FLayerText::Type() const
{
    return  Layer_Text;
}

const std::wstring&
FLayerText::Text() const
{
    return  mText;
}

const FFont&
FLayerText::Font() const
{
    return  mFont;
}

uint32
FLayerText::FontSize() const
{
    return  mFontSize;
}

const FMat3F&
FLayerText::Transform() const
{
    return  mTransform;
}

const FColor&
FLayerText::Color() const
{
    return  mColor;
}

bool
FLayerText::AA() const
{
    return  mAA;
}

void
FLayerText::SetText( const std::wstring& iValue )
{
    mText = iValue;
}

void
FLayerText::SetFont( const FFont& iValue )
{
    mFont = iValue;
}

void
FLayerText::SetFontSize( uint32 iValue )
{
    mFontSize = iValue;
}

void
FLayerText::SetTransform( const FMat3F& iValue )
{
    mTransform = iValue;
}

void
FLayerText::SetColor( const ISample& iValue )
{
    mColor = iValue;
}

void
FLayerText::SetAA( bool iValue )
{
    mAA = iValue;
}

ULIS_NAMESPACE_END

