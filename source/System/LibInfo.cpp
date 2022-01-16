// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LibInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides definitions for API and ABI infos.
* @license      Please refer to LICENSE.md
*/
#include "Core/Core.h"
#include "System/LibInfo.h"

/////////////////////////////////////////////////////
// Macros detect def version
#ifdef  ULIS_DEF_VERSION_MAJOR
#define ULIS_VERSION_MAJOR ULIS_DEF_VERSION_MAJOR
#else
#define ULIS_VERSION_MAJOR 0
#endif //  ULIS_DEF_VERSION_MAJOR

#ifdef  ULIS_DEF_VERSION_MINOR
#define ULIS_VERSION_MINOR ULIS_DEF_VERSION_MINOR
#else
#define ULIS_VERSION_MINOR 0
#endif //  ULIS_DEF_VERSION_MINOR

#ifdef  ULIS_DEF_VERSION_PATCH
#define ULIS_VERSION_PATCH ULIS_DEF_VERSION_PATCH
#else
#define ULIS_VERSION_PATCH 0
#endif //  ULIS_DEF_VERSION_PATCH

#ifdef  ULIS_DEF_GIT_COMMIT_HASH
#define ULIS_GIT_COMMIT_HASH ULIS_DEF_GIT_COMMIT_HASH
#else
#define ULIS_GIT_COMMIT_HASH 0
#endif //  ULIS_DEF_GIT_COMMIT_HASH

#ifdef  ULIS_DEF_GIT_COMMIT_ABBREVIATED_HASH
#define ULIS_GIT_COMMIT_ABBREVIATED_HASH ULIS_DEF_GIT_COMMIT_ABBREVIATED_HASH
#else
#define ULIS_GIT_COMMIT_ABBREVIATED_HASH 0
#endif //  ULIS_DEF_GIT_COMMIT_ABBREVIATED_HASH

#ifdef  ULIS_DEF_GIT_BRANCH_NAME
#define ULIS_GIT_BRANCH_NAME ULIS_DEF_GIT_BRANCH_NAME
#else
#define ULIS_GIT_BRANCH_NAME 0
#endif //  ULIS_DEF_GIT_BRANCH_NAME

#define ULIS_VERSION_MAJOR_STR ULIS_STRINGIFY( ULIS_VERSION_MAJOR )
#define ULIS_VERSION_MINOR_STR ULIS_STRINGIFY( ULIS_VERSION_MINOR )
#define ULIS_VERSION_PATCH_STR ULIS_STRINGIFY( ULIS_VERSION_PATCH )
#define ULIS_GIT_COMMIT_HASH_STR                ULIS_STRINGIFY( ULIS_GIT_COMMIT_HASH )
#define ULIS_GIT_COMMIT_ABBREVIATED_HASH_STR    ULIS_STRINGIFY( ULIS_GIT_COMMIT_ABBREVIATED_HASH )
#define ULIS_GIT_BRANCH_NAME_STR                ULIS_STRINGIFY( ULIS_GIT_BRANCH_NAME )

ULIS_NAMESPACE_BEGIN
//static
FString
FLibInfo::VersionString()
{
    return  VersionMajorString() + "." + VersionMinorString() + "." + VersionPatchString();
}

//static
FString
FLibInfo::VersionMajorString()
{
    return  FString( ULIS_VERSION_MAJOR_STR );
}

//static
FString
FLibInfo::VersionMinorString()
{
    return  FString( ULIS_VERSION_MINOR_STR );
}

//static
FString
FLibInfo::VersionPatchString()
{
    return  FString( ULIS_VERSION_PATCH_STR );
}

//static
uint64
FLibInfo::VersionMajor()
{
    return  ULIS_VERSION_MAJOR;
}

//static
uint64
FLibInfo::VersionMinor()
{
    return  ULIS_VERSION_MINOR;
}

//static
uint64
FLibInfo::VersionPatch()
{
    return  ULIS_VERSION_PATCH;
}

//static
FString
FLibInfo::ConfigurationString()
{
#if defined( ULIS_DEBUG )
    return  FString( "Debug" );
#elif defined( ULIS_RELEASE )
    return  FString( "Release" );
#elif defined( ULIS_RELWITHDEBINFO )
    return  FString( "RelWithDebInfo" );
#endif
}

//static
FString
FLibInfo::CompilationTimeString()
{
    return  FString( __DATE__ ) + ", " + FString( __TIME__ );
}

//static
FString
FLibInfo::CompilerNameString()
{
#if defined( ULIS_CLANG )
    return  FString( "CLANG" );
#elif defined( ULIS_GCC )
    return  FString( "GCC" );
#elif defined( ULIS_MSVC )
    return  FString( "MSVC" );
#elif defined( ULIS_MINGW64 )
    return  FString( "MINGW64" );
#elif defined( ULIS_EMSCRIPTEN )
    return  FString( "EMSCRIPTEN" );
#else
    return  FString( "UNKNOWN" );
#endif
}

//static
FString
FLibInfo::CompilerVersionString()
{
#if defined( ULIS_CLANG )
    return  FString( ULIS_STRINGIFY( __clang_major__ ) ) + "." + ULIS_STRINGIFY( __clang_minor__ ) + "." + ULIS_STRINGIFY( __clang_patchlevel__ );
#elif defined( ULIS_GCC )
    return  FString( ULIS_STRINGIFY( __GNUC__ ) ) + "." + ULIS_STRINGIFY( __GNUC_MINOR__ );
#elif defined( ULIS_MSVC )
    return  FString( ULIS_STRINGIFY( _MSC_VER ) );
#elif defined( ULIS_MINGW64 )
    return  FString( ULIS_STRINGIFY( __MINGW64_VERSION_MAJOR ) ) + "." + ULIS_STRINGIFY( __MINGW64_VERSION_MINOR );
#else
    return  FString( "" );
#endif
}

//static
FString
FLibInfo::CompilerInformationString()
{
    return  CompilerNameString() + " " + CompilerVersionString();
}

//static
bool
FLibInfo::CompiledForx64()
{
    // We just assume this to be always true for now.
    // Compilation for 32 bits should fail anyways.
    return  true;
}

//static
bool
FLibInfo::CompiledWithAVX2()
{
#ifdef ULIS_COMPILETIME_AVX_SUPPORT
    return  true;
#else
    return  false;
#endif
}

//static
bool
FLibInfo::CompiledWithSSE42()
{
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
    return  true;
#else
    return  false;
#endif
}

//static
bool
FLibInfo::BuiltAsSharedLibrary()
{
#ifdef ULIS_BUILT_AS_SHARED_LIBRARY
    return  true;
#else
    return  false;
#endif
}

//static
FString
FLibInfo::BranchNameString()
{
    return  FString( ULIS_GIT_BRANCH_NAME_STR );
}

//static
FString
FLibInfo::CommitHashString()
{
    return  FString( ULIS_GIT_COMMIT_HASH_STR );
}

//static
FString
FLibInfo::CommitAbbreviatedHashString()
{
    return  FString( ULIS_GIT_COMMIT_ABBREVIATED_HASH_STR );
}

//static
bool
FLibInfo::CompiledWithMT() {
#ifdef ULIS_COMPILED_WITH_THREAD_SUPPORT
    return  true;
#else
    return  false;
#endif
}


//static
FString
FLibInfo::LibraryInformationString()
{
    FString on( "on" );
    FString off( "off" );
    FString sse = CompiledWithSSE42() ? on : off;
    FString avx = CompiledWithAVX2() ? on : off;
    FString mt = CompiledWithMT() ? on : off;
    // 4.0.0 (Aug 15 2020, 15:12:04) [MSVC v.1916 x64] {Release}
    return  ConfigurationString() + " " + VersionString() + " "
            + "(" + BranchNameString() + ":" + CommitAbbreviatedHashString() + ", " + CompilationTimeString() + ") "
            + "[" + CompilerInformationString() + " x64] "
            + "{SSE4.2:" + sse + ", AVX2:" + avx + ", MT: " + mt + "};";
}

ULIS_NAMESPACE_END

