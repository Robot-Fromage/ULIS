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
#include "Font/FontRegistry.h"

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


FFont::FFont( void* iFontHandle, const FFontEngine& iFontEngine )
    : mFontHandle( iFontHandle )
    , mFontEngine( iFontEngine )
{
    /*
    std::string fpath = mFontRegistry.FuzzyFindFontPath( iFamily, iStyle );
    FT_Error error = FT_New_Face( reinterpret_cast< FT_Library>( mFontEngine.Handle() ), fpath.c_str(), 0, reinterpret_cast< FT_Face* >( &mHandle ) );
    ULIS_ASSERT( !error, "Error initializing font handle" );
    */
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

ULIS_NAMESPACE_END

