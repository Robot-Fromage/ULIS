// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AXI.h
* @author       Clement Berthaud
* @brief        This file provides declarations for API and ABI infos.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "String/String.h"

ULIS_NAMESPACE_BEGIN
/*! Retrieve the full version string "major.minor.patch". */
ULIS_API FString VersionString();

/*! Retrieve the major version string. */
ULIS_API FString VersionMajorString();

/*! Retrieve the minor version string. */
ULIS_API FString VersionMinorString();

/*! Retrieve the patch version string. */
ULIS_API FString VersionPatchString();

/*! Retrieve the major version number. */
ULIS_API uint64 VersionMajor();

/*! Retrieve the minor version number. */
ULIS_API uint64 VersionMinor();

/*! Retrieve the patch version number. */
ULIS_API uint64 VersionPatch();

/*! Retrieve the built configuration ( Debug, Release, RelWithDebInfo ). */
ULIS_API FString ConfigurationString();

/*! Retrieve the compilation date and time e.g: "Aug 15 2020, 15:12:04". */
ULIS_API FString CompilationTimeStampString();

/*! Retrieve the compiler name string */
ULIS_API FString CompilerNameString();

/*! Retrieve the compiler version string */
ULIS_API FString CompilerVersionString();

/*! Retrieve the full information string */
ULIS_API FString CompilerInformationString();

/*! Retrieve wether the library was compiled for x64 target */
ULIS_API bool CompiledFor64Bit();

/*! Retrieve wether the library was compiled with AVX2 support */
ULIS_API bool CompiledWithAVX2();

/*! Retrieve wether the library was compiled with SSE42 support */
ULIS_API bool CompiledWithSSE42();

/*! Retrieve the full library identification string */
ULIS_API FString FullLibraryInformationString();

ULIS_NAMESPACE_END

