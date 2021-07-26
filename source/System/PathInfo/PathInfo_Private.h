// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PathInfo_Private.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FPathInfo_Private tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "String/String.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
/// @class      FPathInfo_Private
/// @brief      The FPathInfo_Private provides the internal mechanisms for
///             FPathInfo.
/// @details    FPathInfo_Private uses a variation of the pimpl idiom in
///             FPathInfo_Private to abstract the platform specifics.
struct FPathInfo_Private
{
    FPathInfo_Private();
    FString fontPath;
    FString appDataPath;
    TArray< FString > logicalDisksPaths;
    FString homePath;
};

ULIS_NAMESPACE_END

