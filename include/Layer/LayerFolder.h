// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerFolder.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FLayerFolder class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Layer.h"
#include "Layer/LayerImage.h"

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4250) // Shut dominance of inheritance for ULIS::FLayerImage::InitFromParent
/////////////////////////////////////////////////////
/// @class      FLayerFolder
/// @brief      The FLayerFolder class provides a class to store a folder of
///             layers in a layer stack for painting applications.
class ULIS_API FLayerFolder final
    : public FLayerImage
    , public TRoot< ILayer >
{
    // Typedefs
    typedef TRoot< ILayer > tParent;
    typedef FLayerImage     tSuperClass;

public:
    // DTor
    ~FLayerFolder() override;

    // CTors
    FLayerFolder(
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
        , bool iCollapsed = false
        , tParent* iParent = nullptr
    );

    FLayerFolder(
          FBlock* iBlock
        , const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iAlphaLocked = false
        , bool iCollapsed = false
        , tParent* iParent = nullptr
    );

    // Disable copy
    FLayerFolder( const FLayerFolder& ) = delete;
    FLayerFolder& operator=( const FLayerFolder& ) = delete;

public:
    // Getters
    bool IsCollapsed() const;

    // Setters
    void SetCollapsed( bool iValue );

    // Utils
    template< typename T > T& Find( const FString& iName ) { return  dynamic_cast< T& >( (*this)[ iName ] ); }
    template< typename T > const T& Find( const FString& iName ) const { return  dynamic_cast< const T& >( (*this)[ iName ] ); }

    // Static Interface
    constexpr static const char* StaticType() { return  mType; }
    constexpr static const uint32 StaticTypeID() { return  crc32b( mType); }

    // ILayer Interface
    const FString Type() const override { return  StaticType(); }
    const uint32 TypeID() const override { return  StaticTypeID(); }

    // TRoot overload-shadow Interface
    using TRoot< ILayer >::operator[];
    ILayer& operator[]( const FString& iName );
    const ILayer& operator[]( const FString& iName ) const;

    // ICachedImageRendering Interface
    FEvent RenderCache( FContext& iCtx ) override;

private:
    // Private data members
    bool mCollapsed;
    constexpr static const char* mType = "Folder";
};
#pragma warning(pop)

ULIS_NAMESPACE_END

