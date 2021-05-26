// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerFolder.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerFolder class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/LayerFolder.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
FLayerFolder::~FLayerFolder()
{
}

FLayerFolder::FLayerFolder(
      const FString& iName
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , ILayerRoot* iParent
)
    : ILayer(
          iName
        , iParent
    )
    , ILayerRoot(
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
{
}

FLayerFolder::FLayerFolder(
      FBlock* iBlock
    , const FString& iName
    , uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , ILayerRoot* iParent
)
    : ILayer(
          iName
        , iParent
    )
    , ILayerRoot(
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
{
}

eLayerType
FLayerFolder::Type() const
{
    return  Layer_Folder;
}

ULIS_NAMESPACE_END

