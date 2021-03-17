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
#include "Layer/LayerRoot.h"
#include "Layer/LayerImage.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FLayerFolder
/// @brief      The FLayerFolder class provides a class to store a folder of
///             layers in a layer stack for painting applications.
class ULIS_API FLayerFolder final
    : public FLayerRoot
    , public FLayerImage
{
public:
    ~FLayerFolder() override final;
    FLayerFolder(
          const FString& iName
        , uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat
        , eBlendMode iBlendMode
        , eAlphaMode iAlphaMode
        , ufloat iOpacity
        , FLayerRoot* iParent
    );

    FLayerFolder(
          FBlock* iBlock
        , const FString& iName
        , uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat
        , eBlendMode iBlendMode
        , eAlphaMode iAlphaMode
        , ufloat iOpacity
        , FLayerRoot* iParent
    );

    FLayerFolder( const FLayerFolder& ) = delete;
    FLayerFolder& operator=( const FLayerFolder& ) = delete;

public:
    eLayerType Type() const override;
};

ULIS_NAMESPACE_END

