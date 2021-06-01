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
#include "Image/Color.h"
#include "Memory/Tree.h"
#include "String/CRC32.h"

ULIS_NAMESPACE_BEGIN
class ILayer;
template class ULIS_API TNode< ILayer >;
template class ULIS_API TArray< TNode< ILayer >* >;
template class ULIS_API TRoot< ILayer >;
typedef TRoot< ILayer > ILayerRoot;

/////////////////////////////////////////////////////
/// @class      ILayer
/// @brief      The ILayer class provides a base abstract class to store a layer
///             in a layer stack for painting applications.
class ULIS_API ILayer
    : public virtual TNode< ILayer >
{
    typedef TRoot< ILayer > tParent;
    typedef TNode< ILayer > tSuperClass;

public:
    virtual ~ILayer() override = 0;

    ILayer(
          const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , tParent* iParent = nullptr
    );

    const FString& Name() const;
    bool Locked() const;
    bool Visible() const;
    const FColor& Color() const;

    void SetName( const FString& iName );
    void SetLocked( bool iValue );
    void SetVisible( bool iValue );
    void SetColor( const FColor& iColor );

    virtual const FString Type() const = 0;
    virtual const uint32 TypeID() const = 0;

private:
    FString mName;
    bool mLocked;
    bool mVisible;
    FColor mColor;
};

ULIS_NAMESPACE_END

