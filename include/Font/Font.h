// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Font.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FFont class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FFont
/// @brief      The FFont class provides a thin wrapper around the freetype face type.
class ULIS_API FFont
{
public:
    // Construction / Destruction
    ~FFont();
    FFont( const FFont& ) = delete;
    FFont( FFont&& ) = delete;
    FFont& operator=( const FFont& ) = delete;
    FFont& operator=( FFont&& iOther ) = delete;

private:
    FFont( void* iFontHandle, const FFontEngine& iFontEngine );

public:
    // Public API
    void* FontHandle() const;
    const FFontEngine& FontEngine() const;

private:
    // Private Data Members
    void* mFontHandle;
    const FFontEngine& mFontEngine;
};

ULIS_NAMESPACE_END

