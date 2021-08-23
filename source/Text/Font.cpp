// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Font.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FFont class.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Text/Font.h"
#include "Text/FontEngine.h"
#include "Text/FontRegistry.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FFont
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FFont::~FFont()
{
    if( mHandle )
        FT_Done_Face( reinterpret_cast< FT_Face >( mHandle ) );
}


FFont::FFont( const FFontRegistry& iFontRegistry, const std::string& iFamily, const std::string& iStyle )
    : mHandle( nullptr )
    , mFontEngine( iFontRegistry.FontEngine() )
    , mFontRegistry( iFontRegistry )
{
    std::string fpath = mFontRegistry.FuzzyFindFontPath( iFamily, iStyle );
    FT_Error error = FT_New_Face( reinterpret_cast< FT_Library>( mFontEngine.Handle() ), fpath.c_str(), 0, reinterpret_cast< FT_Face* >( &mHandle ) );
    ULIS2_ASSERT( !error, "Error initializing font handle" );
}


FFont::FFont( FFont& iOther )
    : mHandle( iOther.Handle() )
    , mFontEngine( iOther.FontEngine() )
    , mFontRegistry( iOther.FontRegistry() )
{
    iOther.mHandle = nullptr;
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void*
FFont::Handle() const
{
    return  mHandle;
}


const FFontEngine&
FFont::FontEngine() const
{
    return  mFontEngine;
}


const FFontRegistry&
FFont::FontRegistry() const
{
    return  mFontRegistry;
}


ULIS2_NAMESPACE_END

