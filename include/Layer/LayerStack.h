// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FLayerStack class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Size2D.h"
#include "Layer/Layer.h"
#include "Layer/LayerImage.h"
#include "Layer/LayerFolder.h"
#include "Layer/LayerText.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FLayerStack
/// @brief      The FLayerStack class provides a class to store a layer stack
///             for painting applications.
class ULIS_API FLayerStack final
    : public ILayer
    , public TRoot< ILayer >
    , public IHasFormat
    , public IHasColorSpace
    , public IHasSize2D
{
public:
    // DTor
    ~FLayerStack() override;

    // CTor
    FLayerStack(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
    );

    // Disable copy
    FLayerStack( const FLayerStack& ) = delete;
    FLayerStack& operator=( const FLayerStack& ) = delete;

public:
    // Utils
    template< typename T > T& Find( const FString& iName ) { return  dynamic_cast< T& >( (*this)[ iName ] ); }
    template< typename T > const T& Find( const FString& iName ) const { return  dynamic_cast< const T& >( (*this)[ iName ] ); }
    void Reset(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
    );

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
    void RenderImage( FBlock& ioBlock, const FRectI& iRect = FRectI::Auto, const FVec2I& iPos = FVec2I( 0 ) ) override;

private:
    // Private data members
    constexpr static const char* mType = "Stack";
};

ULIS_NAMESPACE_END

