// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Font.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FFont class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Text/Font.h"
#include "Text/TextEngine.h"
#include "Text/FontRegistry.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS_NAMESPACE_BEGIN
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
    , mTextEngine( iFontRegistry.TextEngine() )
    , mFontRegistry( iFontRegistry )
{
    std::string fpath = mFontRegistry.FuzzyFindFontPath( iFamily, iStyle );
    FT_Error error = FT_New_Face( reinterpret_cast< FT_Library>( mTextEngine.Handle() ), fpath.c_str(), 0, reinterpret_cast< FT_Face* >( &mHandle ) );
    ULIS_ASSERT( !error, "Error initializing font handle" );
}


FFont::FFont( FFont& iOther )
    : mHandle( iOther.Handle() )
    , mTextEngine( iOther.TextEngine() )
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


const FTextEngine&
FFont::TextEngine() const
{
    return  mTextEngine;
}


const FFontRegistry&
FFont::FontRegistry() const
{
    return  mFontRegistry;
}


ULIS_NAMESPACE_END

