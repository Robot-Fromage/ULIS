// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PathInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FPathInfo tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "System/PathInfo/PathInfo.h"
#include "System/PathInfo/PathInfoHelpers.h"
#include "System/PathInfo/PathInfo_Private.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
namespace detail {
extern FPathInfo_Private sgPathInfo_private_instance;
} // namespace detail

//static
FString
FPathInfo::FontPath() {
    return  sgPathInfo_private_instance.fontPath;
}

//static
FString
FPathInfo::AppDataPath() {
    return  sgPathInfo_private_instance.appDataPath;
}

//static
void
FPathInfo::LogicalDisksPaths( TArray< FString >& oDisks ) {
    oDisks.Clear();
    oDisks.Resize( sgPathInfo_private_instance.logicalDisksPaths.Size() );
    for( int i = 0; i < sgPathInfo_private_instance.logicalDisksPaths.Size(); ++i )
        oDisks[i] = sgPathInfo_private_instance.logicalDisksPaths[ i ];
}

//static
FString
FPathInfo::HomePath() {
    return  sgPathInfo_private_instance.homePath;
}


ULIS_NAMESPACE_END

