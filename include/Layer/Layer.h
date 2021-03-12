// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Layer.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ILayer class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "String/String.h"

ULIS_NAMESPACE_BEGIN
enum eLayerType
{
      Layer_Root
    , Layer_Image
    , Layer_Folder
    , Layer_Text
    , Layer_Vector
    , Layer_FX
    , Layer_Mask
    , NumLayerTypes
};

class FLayerRoot;
/////////////////////////////////////////////////////
/// @class      ILayer
/// @brief      The ILayer class provides a base abstract class to store a layer
///             in a layer stack for painting applications.
class ULIS_API ILayer
{
public:
    virtual ~ILayer() = 0;
    ILayer();
    ILayer( const FString& iName, FLayerRoot* iParent );

public:
    virtual eLayerType Type() const = 0;
    const FString& Name() const;
    void SetName( const FString& iName );
    bool Locked() const;
    bool Visible() const;
    void SetLocked( bool iValue );
    void SetVisible( bool iValue );
    FLayerRoot* Parent();
    const FLayerRoot* Parent() const;

private:
    FString mName;
    bool mLocked;
    bool mVisible;
    FLayerRoot* mParent;
};

ULIS_NAMESPACE_END

