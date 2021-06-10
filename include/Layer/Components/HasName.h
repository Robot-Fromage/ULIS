// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasName.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the IHasName class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "String/String.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      IHasName
/// @brief      Simple HasName class.
class ULIS_API IHasName
{
public:
    IHasName( const FString& iName );

public:
    const FString& Name() const;
    void SetName( const FString& iName );

private:
    FString mStr;
};

ULIS_NAMESPACE_END

