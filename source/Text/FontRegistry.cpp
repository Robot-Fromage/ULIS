// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         FontRegistry.cpp
* @author       Clement Berthaud
* @brief        This file provides the registry classes for the Font Registry tools.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Text/FontRegistry.h"
#include "Base/FilePathRegistry.h"
#include "Base/StringUtils.h"
#include "Text/FontEngine.h"
#include "Text/Font.h"

#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <tuple>

#ifdef ULIS2_WIN
#include <Windows.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FFontStyleKey
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FFontStyleKey::~FFontStyleKey()
{
}


FFontStyleKey::FFontStyleKey( const std::string& iFamilyName, const std::string& iStyleName, const std::string& iFontPath )
    : mFamilyName( iFamilyName )
    , mStyleName( iStyleName )
    , mFontPath( iFontPath )
{
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
const std::string&
FFontStyleKey::GetFamilyName() const
{
    return  mFamilyName;
}


const std::string&
FFontStyleKey::GetStyleName() const
{
    return  mStyleName;
}


const std::string&
FFontStyleKey::GetFontPath() const
{
    return  mFontPath;
}


/////////////////////////////////////////////////////
// FFontFamilyKey
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FFontFamilyKey::~FFontFamilyKey()
{
}


FFontFamilyKey::FFontFamilyKey( const std::string& iFamilyName )
    : mFamilyName( iFamilyName )
{
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void
FFontFamilyKey::AddFontStyleKey( const std::string& iStyle, const FFontStyleKey& iFontStyleKey )
{
    mStyles.emplace( iStyle, iFontStyleKey );
}


int
FFontFamilyKey::NumStyles() const
{
    return  static_cast< int >( mStyles.size() );
}


const std::map< std::string, FFontStyleKey >&
FFontFamilyKey::GetStyles() const
{
    return  mStyles;
}


const std::string&
FFontFamilyKey::GetFamilyName() const
{
    return  mFamilyName;
}


const FFontStyleKey*
FFontFamilyKey::FuzzyFindFontStyleKey( const std::string& iName ) const
{
    std::string lowercase_name = iName;
    std::transform( lowercase_name.begin(), lowercase_name.end(), lowercase_name.begin(), ::tolower);
    std::vector< std::tuple< int, std::string > > matches;
    for( auto it : mStyles ) {
        std::string key = it.first;
        std::string lowercase_key = key;
        std::transform( lowercase_key.begin(), lowercase_key.end(), lowercase_key.begin(), ::tolower);
        int dist = static_cast< int >( LevenshteinDistance( lowercase_name.c_str(), lowercase_name.size(), lowercase_key.c_str(), lowercase_key.size() ) );
        matches.push_back( std::tuple< int, std::string >( dist, key ) );
    }
    std::sort( matches.begin(), matches.end() );

    auto it = mStyles.find( std::get< 1 >( matches[0] ) );
    return  it == mStyles.end() ? nullptr : &mStyles.at( std::get< 1 >( matches[0] ) );
}

/////////////////////////////////////////////////////
// FFontRegistry
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FFontRegistry::~FFontRegistry()
{
}


FFontRegistry::FFontRegistry( const FFontEngine& iFontEngine )
    : mFontEngine( iFontEngine )
{
    #ifdef ULIS2_WIN
        std::string sysfpath;
        CHAR windir[MAX_PATH];
        auto err = GetWindowsDirectoryA( windir, MAX_PATH );
        ULIS2_ASSERT( err, "Error loading Windows directory path during font retrieval." );
        sysfpath = std::string( windir );
        ReplaceAllOccurences( sysfpath, "\\", "/" );
        sysfpath += "/Fonts/";
        mLookupPaths.push_back( sysfpath );
    #elif defined ULIS2_MACOS
        mLookupPaths.push_back( "/System/Library/Fonts/" );
        mLookupPaths.push_back( "/Library/Fonts/" );
        mLookupPaths.push_back( "~/Library/Fonts/" );
        mLookupPaths.push_back( "/Network/Library/Fonts/" );
    #elif defined ULIS2_LINUX
        mLookupPaths.push_back( "/usr/share/fonts/" );
        mLookupPaths.push_back( "/usr/local/share/fonts/" );
        mLookupPaths.push_back( "~/.fonts/" );
        mLookupPaths.push_back( "/etc/fonts/fonts.conf/" );
        mLookupPaths.push_back( "/etc/fonts/local.conf./" );
    #endif

    Load();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void
FFontRegistry::AddLookupPath( const std::string& iPath )
{
    mLookupPaths.push_back( iPath );
}


void
FFontRegistry::AddLookupPaths( const std::vector< std::string >& iPaths )
{
    for( auto it : iPaths )
        mLookupPaths.push_back( it );
}


void
FFontRegistry::Load() {
    mFamilies.clear();
    FFilePathRegistry reg;
    reg.AddLookupPaths( mLookupPaths );
    reg.AddFilter( ".ttf" );
    reg.AddFilter( ".ttc" );
    reg.AddFilter( ".otf" );
    reg.Parse();

    for( auto it : reg.GetMap() ) {
        FT_Face face;
        FT_Error load_error = FT_New_Face( reinterpret_cast< FT_Library >( mFontEngine.Handle() ), it.second.c_str(), 0, &face );
        ULIS2_ASSERT( !load_error, "An error occured during freetype loading of font information: " << it.second.c_str() );
        if( load_error ) continue;
        std::string familyName( face->family_name );
        std::string style( face->style_name );
        mFamilies.emplace( familyName, FFontFamilyKey( familyName ) );
        mFamilies.at( familyName ).AddFontStyleKey( style, FFontStyleKey( familyName, style, it.second ) );
        // Free face
        FT_Done_Face( face );
    }
}


int
FFontRegistry::NumFamilies() const
{
    return  static_cast< int >( mFamilies.size() );
}


int
FFontRegistry::NumStyles() const
{
    int ret = 0;
    for( auto it : mFamilies ) {
        ret += it.second.NumStyles();
    }
    return  ret;
}


const std::map< std::string, FFontFamilyKey >&
FFontRegistry::GetFamilies() const
{
    return  mFamilies;
}


const std::vector< std::string >&
FFontRegistry::GetLookupPaths() const
{
    return  mLookupPaths;
}


const FFontFamilyKey*
FFontRegistry::FuzzyFindFontFamily( const std::string& iName ) const
{
    std::string lowercase_name = iName;
    std::transform( lowercase_name.begin(), lowercase_name.end(), lowercase_name.begin(), ::tolower);
    std::vector< std::tuple< int, std::string > > matches;
    for( auto it : mFamilies ) {
        std::string key = it.first;
        std::string lowercase_key = key;
        std::transform( lowercase_key.begin(), lowercase_key.end(), lowercase_key.begin(), ::tolower);
        int dist = static_cast< int >( LevenshteinDistance( lowercase_name.c_str(), lowercase_name.size(), lowercase_key.c_str(), lowercase_key.size() ) );
        matches.push_back( std::tuple< int, std::string >( dist, key ) );
    }
    std::sort( matches.begin(), matches.end() );

    auto it = mFamilies.find( std::get< 1 >( matches[0] ) );
    return  it == mFamilies.end() ? nullptr : &( mFamilies.at( std::get< 1 >( matches[0] ) ) );
}


const FFontStyleKey*
FFontRegistry::FuzzyFindFontStyleKey( const std::string& iFamily, const std::string& iStyle ) const
{
    const FFontFamilyKey* fam = FuzzyFindFontFamily( iFamily );

    if( !fam )
        return  nullptr;

    return  fam->FuzzyFindFontStyleKey( iStyle );
}


std::string
FFontRegistry::FuzzyFindFontPath( const std::string& iFamily, const std::string& iStyle ) const
{
    const FFontStyleKey* stk = FuzzyFindFontStyleKey( iFamily, iStyle );

    if( !stk )
        return  "";

    return  stk->GetFontPath();
}

const FFontEngine&
FFontRegistry::FontEngine() const
{
    return  mFontEngine;
}

ULIS2_NAMESPACE_END

