// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Font.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FFont class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Font/Font.h"
#include "Font/FontEngine.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FFont
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FFont::~FFont()
{
    ULIS_ASSERT( mFontHandle, "Bad state" );
    FT_Done_Face( reinterpret_cast< FT_Face >( mFontHandle ) );
}

FFont::FFont(
      const FFontEngine& iFontEngine
    , const std::string& iRequestedFamily
    , const std::string& iRequestedStyle
    )
    : mFontHandle( nullptr )
    , mFontEngine( iFontEngine )
    , mFamily()
    , mStyle()
{
    const FFontStyleEntry* entry = mFontEngine.FuzzyFindFontStyle( iRequestedFamily, iRequestedStyle );
    ULIS_ASSERT( entry, "Error loading font" );
    mFamily = entry->Family();
    mStyle = entry->Style();
    std::string path = entry->Path();
    FT_Error error = FT_New_Face( reinterpret_cast< FT_Library>( mFontEngine.LibraryHandle() ), path.c_str(), 0, reinterpret_cast< FT_Face* >( &mFontHandle ) );
    ULIS_ASSERT( !error, "Error initializing font handle" );
}

FFont::FFont(
      const FFontEngine& iFontEngine
    , const FFontStyleEntry* iRequestedEntry
    )
    : mFontHandle( nullptr )
    , mFontEngine( iFontEngine )
    , mFamily()
    , mStyle()
{
    ULIS_ASSERT( iRequestedEntry, "Error loading font" );
    mFamily = iRequestedEntry->Family();
    mStyle = iRequestedEntry->Style();
    std::string path = iRequestedEntry->Path();
    FT_Error error = FT_New_Face( reinterpret_cast< FT_Library>( mFontEngine.LibraryHandle() ), path.c_str(), 0, reinterpret_cast< FT_Face* >( &mFontHandle ) );
    ULIS_ASSERT( !error, "Error initializing font handle" );
}

FFont::FFont(
      const FFontEngine& iFontEngine
    , const std::string& iRequestedPath
    )
    : mFontHandle( nullptr )
    , mFontEngine( iFontEngine )
    , mFamily()
    , mStyle()
{
    FT_Face face = reinterpret_cast< FT_Face >( &mFontHandle );
    FT_Error error = FT_New_Face( reinterpret_cast< FT_Library>( mFontEngine.LibraryHandle() ), iRequestedPath.c_str(), 0, &face );
    ULIS_ASSERT( !error, "Error initializing font handle" );
    mFamily = std::string( face->family_name );
    mStyle = std::string( face->style_name );
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void*
FFont::FontHandle() const
{
    return  mFontHandle;
}


const FFontEngine&
FFont::FontEngine() const
{
    return  mFontEngine;
}

const std::string&
FFont::Family() const
{
    return  mFamily;
}

const std::string&
FFont::Style() const
{
    return  mStyle;
}

ULIS_NAMESPACE_END

