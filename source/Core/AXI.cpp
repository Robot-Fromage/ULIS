// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AXI.cpp
* @author       Clement Berthaud
* @brief        This file provides definitions for API and ABI infos.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Core/Core.h"
#include "Core/AXI.h"

ULIS_NAMESPACE_BEGIN
// Help links:
// https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html
FString
VersionString()
{
    return  VersionMajorString() + "." + VersionMinorString() + "." + VersionPatchString();
}

FString
VersionMajorString()
{
    return  FString( ULIS_VERSION_MAJOR_STR );
}

FString
VersionMinorString()
{
    return  FString( ULIS_VERSION_MINOR_STR );
}

FString
VersionPatchString()
{
    return  FString( ULIS_VERSION_PATCH_STR );
}

uint64
VersionMajor()
{
    return  ULIS_VERSION_MAJOR;
}

uint64
VersionMinor()
{
    return  ULIS_VERSION_MINOR;
}

uint64
VersionPatch()
{
    return  ULIS_VERSION_PATCH;
}

FString
ConfigurationString()
{
#if defined( ULIS_DEBUG )
    return  FString( "Debug" );
#elif defined( ULIS_RELEASE )
    return  FString( "Release" );
#elif defined( ULIS_RELWITHDEBINFO )
    return  FString( "RelWithDebInfo" );
#endif
}

FString
CompilationTimeStampString()
{
    return  FString( __DATE__ ) + ", " + FString( __TIME__ );
}

FString
CompilerNameString()
{
#if defined( ULIS_EMSCRIPTEN )
    return  FString( "EMSCRIPTEN" );
#elif defined( ULIS_CLANG )
    return  FString( "CLANG" );
#elif defined( ULIS_GCC )
    return  FString( "GCC" );
#elif defined( ULIS_MSVC )
    return  FString( "MSVC" );
#elif defined( ULIS_MINGW64 )
    return  FString( "MINGW64" );
#else
    return  FString( "UNKNOWN" );
#endif
}

FString
CompilerVersionString()
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

FString
CompilerInformationString()
{
    return  CompilerNameString() + " " + CompilerVersionString();
}

bool
CompiledFor64Bit()
{
    // We just assume this to be always true for now.
    // Compilation for 32 bits should fail anyways.
    return  true;
}

bool
CompiledWithAVX2()
{
#ifdef ULIS_COMPILETIME_AVX_SUPPORT
    return  true;
#else
    return  false;
#endif
}

bool
CompiledWithSSE42()
{
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
    return  true;
#else
    return  false;
#endif
}

FString
BranchName()
{
    return  FString( ULIS_GIT_BRANCH_NAME_STR );
}

FString
CommitHash()
{
    return  FString( ULIS_GIT_COMMIT_HASH_STR );
}

FString CommitAbbreviatedHash()
{
    return  FString( ULIS_GIT_COMMIT_ABBREVIATED_HASH_STR );
}

bool
CompiledWithMT() {
#ifdef ULIS_COMPILED_WITH_THREAD_SUPPORT
    return  true;
#else
    return  false;
#endif
}

FString
FullLibraryInformationString()
{
    FString on( "on" );
    FString off( "off" );
    FString sse = CompiledWithSSE42() ? on : off;
    FString avx = CompiledWithAVX2() ? on : off;
    FString mt = CompiledWithMT() ? on : off;
    // 4.0.0 (Aug 15 2020, 15:12:04) [MSVC v.1916 x64] {Release}
    return  ConfigurationString() + " " + VersionString() + " "
            + "(" + BranchName() + ":" + CommitAbbreviatedHash() + ", " + CompilationTimeStampString() + ") "
            + "[" + CompilerInformationString() + " x64] "
            + "{SSE4.2:" + sse + ", AVX2:" + avx + ", MT: " + mt + "};";
}

ULIS_NAMESPACE_END

