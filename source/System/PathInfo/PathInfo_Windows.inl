// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PathInfo_Windows.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FPathInfo tools.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/PathInfo/PathInfo.h"
#include "String/String.h"
#include "String/Utils.h"
#include "Memory/Array.h"

#include <string>

#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <shlobj.h>

ULIS_NAMESPACE_BEGIN
namespace detail {
void GetFontPaths( TArray< FString >& oPaths ) {
    oPaths.Clear();

    std::string sysfpath;
    CHAR windir[MAX_PATH];
    auto err = GetWindowsDirectoryA( windir, MAX_PATH );
    ULIS_ASSERT( err, "Error loading Windows directory path during font retrieval." );
    sysfpath = std::string( windir );
    ReplaceAllOccurences( sysfpath, "\\", "/" );
    sysfpath += "/Fonts/";
    oPaths.PushBack( FString( sysfpath.c_str() ) );
}

FString GetAppDataPath() {
    CHAR chpath[ MAX_PATH ];
    if( SUCCEEDED( SHGetFolderPathA( NULL, CSIDL_COMMON_APPDATA, NULL, 0, chpath ) ) )
    {
        std::string path = std::string( chpath );
        ReplaceAllOccurences( path, "\\", "/" );
        return  FString( path.c_str() );
    }
    else
    {
        ULIS_ASSERT( false, "Error retrieving App Data path" );
        return  FString();
    }
}

void GetDiskPaths( TArray< FString >& oPaths ) {
    oPaths.Clear();
    uint32 mask = static_cast< uint32 >( GetLogicalDrives() );
    ULIS_ASSERT( mask, "Error occured while finding available drives" );
    for( int i = 0; i < sizeof( uint32 ) * 8; ++i ) {
        if( mask & ( 1 << i ) ) {
            FString str;
            str.Append( 'A' + i );
            str.Append( ":/" );
            oPaths.PushBack( str );
        }
    }
}

constexpr auto a = sizeof( uint32 );

FString GetHomePath() {
    CHAR chpath[ MAX_PATH ];
    if( SUCCEEDED( SHGetFolderPathA( NULL, CSIDL_PROFILE, NULL, 0, chpath ) ) )
    {
        std::string path = std::string( chpath );
        ReplaceAllOccurences( path, "\\", "/" );
        return  FString( path.c_str() );
    }
    else
    {
        ULIS_ASSERT( false, "Error retrieving App Data path" );
        return  FString();
    }
}

} // namespace detail

ULIS_NAMESPACE_END

