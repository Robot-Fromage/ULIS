// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PathInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FPathInfo tools.
* @license      Please refer to LICENSE.md
*/
#include "System/PathInfo/PathInfo.h"
#include "System/PathInfo/PathInfo_Private.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
namespace detail {
FPathInfo_Private sgPathInfo_private_instance;
} // namespace detail

//static
void
FPathInfo::FontPaths( TArray< FString >& oPaths ) {
    oPaths.Clear();
    oPaths.Resize( detail::sgPathInfo_private_instance.fontPaths.Size() );
    for( int i = 0; i < detail::sgPathInfo_private_instance.fontPaths.Size(); ++i )
        oPaths[i] = detail::sgPathInfo_private_instance.fontPaths[ i ];
}

//static
FString
FPathInfo::AppDataPath() {
    return  detail::sgPathInfo_private_instance.appDataPath;
}

//static
void
FPathInfo::DiskPaths( TArray< FString >& oPaths ) {
    oPaths.Clear();
    oPaths.Resize( detail::sgPathInfo_private_instance.diskPaths.Size() );
    for( int i = 0; i < detail::sgPathInfo_private_instance.diskPaths.Size(); ++i )
        oPaths[i] = detail::sgPathInfo_private_instance.diskPaths[ i ];
}

//static
FString
FPathInfo::HomePath() {
    return  detail::sgPathInfo_private_instance.homePath;
}


ULIS_NAMESPACE_END

