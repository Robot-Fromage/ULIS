// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerText.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FLayerText class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Layer.h"
#include "Layer/LayerImage.h"
#include "Font/Font.h"
#include "Image/Color.h"
#include "Math/Geometry/Matrix.h"

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
/////////////////////////////////////////////////////
/// @class      FLayerText
/// @brief      The FLayerText class provides a class to store text in a layer
///             of a layer stack for painting applications.
class ULIS_API FLayerText final
    : public FLayerImage
{
    // Typedefs
    typedef TRoot< ILayer > tParent;
    typedef FLayerImage     tSuperClass;

public:
    // DTor
    ~FLayerText() override;

    // CTors
    FLayerText(
          const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , uint16 iWidth = 1
        , uint16 iHeight = 1
        , eFormat iFormat = Format_RGBA8
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iAlphaLocked = false
        , const FFont& iFont = FFont::DefaultFont
        , const std::wstring& iText = L""
        , uint32 iFontSize = 12
        , const FMat3F& iTransform = FMat3F()
        , const ISample& iTextColor = FColor::Black
        , bool iAA = true
        , tParent* iParent = nullptr
    );

    FLayerText(
          FBlock* iBlock
        , const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iAlphaLocked = false
        , const FFont& iFont = FFont::DefaultFont
        , const std::wstring& iText = L""
        , uint32 iFontSize = 12
        , const FMat3F& iTransform = FMat3F()
        , const ISample& iTextColor = FColor::Black
        , bool iAA = true
        , tParent* iParent = nullptr
    );

    // Disable copy
    FLayerText( const FLayerText& ) = delete;
    FLayerText& operator=( const FLayerText& ) = delete;

public:
    // Getters
    const std::wstring& Text() const;
    const FFont& Font() const;
    uint32 FontSize() const;
    const FMat3F& Transform() const;
    const FColor& TextColor() const;
    bool AA() const;

    // Setters
    void SetText( const std::wstring& iValue );
    void SetFont( const FFont& iValue );
    void SetFontSize( uint32 iValue );
    void SetTransform( const FMat3F& iValue );
    void SetTextColor( const ISample& iValue );
    void SetAA( bool iValue );

    // Static Interface
    constexpr static const char* StaticType() { return  mType; }
    constexpr static const uint32 StaticTypeID() { return  crc32b( mType); }

    // ILayer Interface
    const FString Type() const override { return  StaticType(); }
    const uint32 TypeID() const override { return  StaticTypeID(); }

    // ICachedImageRendering Interface
    FEvent RenderCache( FContext& iCtx ) override;

private:
    // Private data members
    std::wstring mText;
    FFont mFont;
    uint32 mFontSize;
    FMat3F mTransform;
    FColor mTextColor;
    bool mAA;
    constexpr static const char* mType = "Text";
};
#pragma warning(pop)

ULIS_NAMESPACE_END

