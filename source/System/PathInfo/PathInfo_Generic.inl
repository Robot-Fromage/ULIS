// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PathInfo_Generic.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FPathInfo tools.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/PathInfo/PathInfo.h"
#include "String/String.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
namespace detail {
void GetFontPaths( TArray< FString >& oPaths ) {
    oPaths.Clear();
}

FString GetAppDataPath() {
    return  FString();
}

void GetDiskPaths( TArray< FString >& oPaths ) {
    oPaths.Clear();
}

FString GetHomePath() {
    return  FString();
}

} // namespace detail

ULIS_NAMESPACE_END

