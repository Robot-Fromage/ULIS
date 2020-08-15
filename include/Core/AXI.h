// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         AXI.h
* @author       Clement Berthaud
* @brief        This file provides declarations for API and ABI infos.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
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

/*! Retrieve the compilation date and time e.g: "Aug 15 2020 15:12:04. */
ULIS_API FString CompilationTimeStamp();

ULIS_NAMESPACE_END

