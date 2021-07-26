// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PathInfo_Linux.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FPathInfo tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/PathInfo/PathInfo.h"
#include "String/String.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
namespace detail {
FString GetFontPath() {
    return  FString();
}

FString GetAppDataPath() {
    return  FString();
}

void GetLogicalDisksPaths( TArray< FString >& oDisks ) {
    oDisks.Clear();
}

FString GetHomePath() {
    return  FString();
}
} // namespace detail

ULIS_NAMESPACE_END

