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
CompilationTimeStamp()
{
    return  FString( ULIS_STRINGIFY( __DATE__ ) ) + " " + ULIS_STRINGIFY( __TIME__ );
}

FString
CompilerNameString()
{
#if defined( ULIS_CLANG )
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
    return  FString( "v0000" );
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
FullLibraryInformationString()
{
    return  FString( "ULIS" ) + " " + VersionString() + " (" + CompilationTimeStamp() + ") [" + CompilerInformationString() + "]";
}

ULIS_NAMESPACE_END

