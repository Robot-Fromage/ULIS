// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerText.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FAnimatedLayerText class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/AnimatedLayer/AnimatedLayerText.h"
#include "Context/Context.h"

ULIS_NAMESPACE_BEGIN
FAnimatedLayerText::~FAnimatedLayerText()
{
}

FAnimatedLayerText::FAnimatedLayerText(
      const FString& iName
    , bool iPaintLocked
    , bool iTimeLocked
    , bool iVisible
    , const FColor& iColor
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iAlphaLocked
    , const FFont& iFont
    , const std::wstring& iText
    , uint32 iFontSize
    , const FMat3F& iTransform
    , const ISample& iTextColor
    , bool iAA
    , tParent* iParent
)
    : tSuperClass(
          iName
        , iPaintLocked
        , iTimeLocked
        , iVisible
        , iColor
        , iWidth
        , iHeight
        , iFormat
        , iBlendMode
        , iAlphaMode
        , iOpacity
        , iAlphaLocked
        , iParent
    )
    , mText( iText )
    , mFont( iFont )
    , mFontSize( iFontSize )
    , mTransform( iTransform )
    , mTextColor( iTextColor )
    , mAA( iAA )
{
}

FAnimatedLayerText::FAnimatedLayerText(
      FBlock* iBlock
    , const FString& iName
    , bool iPaintLocked
    , bool iTimeLocked
    , bool iVisible
    , const FColor& iColor
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , bool iAlphaLocked
    , const FFont& iFont
    , const std::wstring& iText
    , uint32 iFontSize
    , const FMat3F& iTransform
    , const ISample& iTextColor
    , bool iAA
    , tParent* iParent
)
    : tSuperClass(
          iBlock
        , iName
        , iPaintLocked
        , iTimeLocked
        , iVisible
        , iColor
        , iBlendMode
        , iAlphaMode
        , iOpacity
        , iAlphaLocked
        , iParent
    )
    , mText( iText )
    , mFont( iFont )
    , mFontSize( iFontSize )
    , mTransform( iTransform )
    , mTextColor( iTextColor )
    , mAA( iAA )
{
}

const std::wstring&
FAnimatedLayerText::Text() const
{
    return  mText;
}

const FFont&
FAnimatedLayerText::Font() const
{
    return  mFont;
}

uint32
FAnimatedLayerText::FontSize() const
{
    return  mFontSize;
}

const FMat3F&
FAnimatedLayerText::Transform() const
{
    return  mTransform;
}

const FColor&
FAnimatedLayerText::TextColor() const
{
    return  mTextColor;
}

bool
FAnimatedLayerText::AA() const
{
    return  mAA;
}

void
FAnimatedLayerText::SetText( const std::wstring& iValue )
{
    mText = iValue;
    InvalidImageCache();
}

void
FAnimatedLayerText::SetFont( const FFont& iValue )
{
    mFont = iValue;
    InvalidImageCache();
}

void
FAnimatedLayerText::SetFontSize( uint32 iValue )
{
    mFontSize = iValue;
    InvalidImageCache();
}

void
FAnimatedLayerText::SetTransform( const FMat3F& iValue )
{
    mTransform = iValue;
    InvalidImageCache();
}

void
FAnimatedLayerText::SetTextColor( const ISample& iValue )
{
    mTextColor = iValue;
    InvalidImageCache();
}

void
FAnimatedLayerText::SetAA( bool iValue )
{
    mAA = iValue;
    InvalidImageCache();
}

FEvent
FAnimatedLayerText::RenderCache( FContext& iCtx ) {
    if( IsImageCacheValid() )
        return  FEvent::NoOP();

    FEvent clr;
    FEvent ev;
    iCtx.Clear( Block(), FRectI::Auto, FSchedulePolicy::CacheEfficient, 0, nullptr, &clr );
    if( mAA )
        iCtx.RasterTextAA( Block(), mText, mFont, mFontSize, mTransform, mTextColor, FSchedulePolicy::MonoChunk, 1, &clr, &ev );
    else
        iCtx.RasterText( Block(), mText, mFont, mFontSize, mTransform, mTextColor, FSchedulePolicy::MonoChunk, 1, &clr, &ev );

    ValidateImageCache();
    return  ev;
}

ULIS_NAMESPACE_END

