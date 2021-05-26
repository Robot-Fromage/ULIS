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
#include "Layer/Layer.h"
#include "Layer/LayerImage.h"
#include "Layer/LayerFolder.h"
#include "Layer/LayerText.h"
#include "Layer/LayerRoot.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FLayerStack
/// @brief      The FLayerStack class provides a class to store a layer stack
///             for painting applications.
class ULIS_API FLayerStack
    : public IHasFormat
    , public IHasColorSpace
    , public ILayerRoot
{
public:
    virtual ~FLayerStack();
    FLayerStack(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
    );

    FLayerStack( const FLayerStack& ) = delete;
    FLayerStack& operator=( const FLayerStack& ) = delete;

public:
    uint16 Width() const;
    uint16 Height() const;
    uint32 Area() const;
    FRectI Rect() const;

    ILayerRoot& Root();
    const ILayerRoot& Root() const;

    void Reset(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
    );

    bool CheckSanity() const;

private:
    uint16 mWidth;
    uint16 mHeight;
};

ULIS_NAMESPACE_END

