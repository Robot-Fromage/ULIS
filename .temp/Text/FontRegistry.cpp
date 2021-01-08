// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FontRegistry.cpp
* @author       Clement Berthaud
* @brief        This file provides the registry classes for the Font Registry tools.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Text/FontRegistry.h"
#include "System/FilePathRegistry.h"
#include "String/Utils.h"
#include "Text/TextEngine.h"
#include "Text/Font.h"

#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>
#include <tuple>

#ifdef ULIS_WIN
#include <Windows.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FFontStyleEntry
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FFontStyleEntry::~FFontStyleEntry()
{
}

FFontStyleEntry::FFontStyleEntry( const std::string& iFamily, const std::string& iStyle, const std::string& iPath )
    : mFamily( iFamily )
    , mStyle( iStyle )
    , mPath( iPath )
{
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
const std::string&
FFontStyleEntry::Family() const
{
    return  mFamily;
}

const std::string&
FFontStyleEntry::Style() const
{
    return  mStyle;
}

const std::string&
FFontStyleEntry::Path() const
{
    return  mPath;
}

/////////////////////////////////////////////////////
// FFontFamilyEntry
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FFontFamilyEntry::~FFontFamilyEntry()
{
}

FFontFamilyEntry::FFontFamilyEntry( const std::string& iName )
    : mFamily( iName )
{
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void
FFontFamilyEntry::AddFontStyleKey( const std::string& iStyle, const FFontStyleEntry& iFontStyleKey )
{
    mStyles.emplace( iStyle, iFontStyleKey );
}

int
FFontFamilyEntry::StyleCount() const
{
    return  static_cast< int >( mStyles.size() );
}


const std::map< std::string, FFontStyleEntry >&
FFontFamilyEntry::Styles() const
{
    return  mStyles;
}


const std::string&
FFontFamilyEntry::Family() const
{
    return  mFamily;
}


const FFontStyleEntry*
FFontFamilyEntry::FuzzyFindFontStyleKey( const std::string& iName ) const
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

FFontRegistry::FFontRegistry( const FTextEngine& iTextEngine )
    : mTextEngine( iTextEngine )
{
    #ifdef ULIS_WIN
        std::string sysfpath;
        CHAR windir[MAX_PATH];
        auto err = GetWindowsDirectoryA( windir, MAX_PATH );
        ULIS_ASSERT( err, "Error loading Windows directory path during font retrieval." );
        sysfpath = std::string( windir );
        ReplaceAllOccurences( sysfpath, "\\", "/" );
        sysfpath += "/Fonts/";
        mLookupPaths.push_back( sysfpath );
    #elif defined ULIS_MACOS
        mLookupPaths.push_back( "/System/Library/Fonts/" );
        mLookupPaths.push_back( "/Library/Fonts/" );
        mLookupPaths.push_back( "~/Library/Fonts/" );
        mLookupPaths.push_back( "/Network/Library/Fonts/" );
    #elif defined ULIS_LINUX
        mLookupPaths.push_back( "/usr/share/fonts/" );
        mLookupPaths.push_back( "/usr/local/share/fonts/" );
        mLookupPaths.push_back( "~/.fonts/" );
        mLookupPaths.push_back( "/etc/fonts/fonts.conf/" );
        mLookupPaths.push_back( "/etc/fonts/local.conf./" );
    #endif

    Refresh();
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void
FFontRegistry::AddLookupPath( const std::string& iPath )
{
    mLookupPaths.push_back( iPath );
}

void
FFontRegistry::AddLookupPaths( const std::list< std::string >& iPaths )
{
    for( auto it : iPaths )
        mLookupPaths.push_back( it );
}

void
FFontRegistry::Refresh() {
    mRecords.clear();
    FFilePathRegistry reg;
    reg.AddLookupPaths( mLookupPaths );
    reg.AddFilter( ".ttf" );
    reg.AddFilter( ".ttc" );
    reg.AddFilter( ".otf" );
    reg.Parse();

    for( auto it : reg.Records() ) {
        FT_Face face;
        FT_Error load_error = FT_New_Face( reinterpret_cast< FT_Library >( mTextEngine.Handle() ), it.second.c_str(), 0, &face );
        ULIS_ASSERT( !load_error, "An error occured during freetype loading of font information: " << it.second.c_str() );
        if( load_error ) continue;
        std::string familyName( face->family_name );
        std::string style( face->style_name );
        mRecords.emplace( familyName, FFontFamilyEntry( familyName ) );
        mRecords.at( familyName ).AddFontStyleKey( style, FFontStyleEntry( familyName, style, it.second ) );
        FT_Done_Face( face );
    }
}

int
FFontRegistry::FamilyCount() const
{
    return  static_cast< int >( mRecords.size() );
}

const std::map< std::string, FFontFamilyEntry >&
FFontRegistry::Records() const
{
    return  mRecords;
}

const std::list< std::string >&
FFontRegistry::LookupPaths() const
{
    return  mLookupPaths;
}

const FFontFamilyEntry*
FFontRegistry::FuzzyFindFontFamily( const std::string& iName ) const
{
    std::string lowercase_name = iName;
    std::transform( lowercase_name.begin(), lowercase_name.end(), lowercase_name.begin(), ::tolower);
    std::vector< std::tuple< int, std::string > > matches;
    for( auto it : mRecords ) {
        std::string key = it.first;
        std::string lowercase_key = key;
        std::transform( lowercase_key.begin(), lowercase_key.end(), lowercase_key.begin(), ::tolower);
        int dist = static_cast< int >( LevenshteinDistance( lowercase_name.c_str(), lowercase_name.size(), lowercase_key.c_str(), lowercase_key.size() ) );
        matches.push_back( std::tuple< int, std::string >( dist, key ) );
    }
    std::sort( matches.begin(), matches.end() );

    auto it = mRecords.find( std::get< 1 >( matches[0] ) );
    return  it == mRecords.end() ? nullptr : &( mRecords.at( std::get< 1 >( matches[0] ) ) );
}


const FFontStyleEntry*
FFontRegistry::FuzzyFindFontStyle( const std::string& iFamily, const std::string& iStyle ) const
{
    const FFontFamilyEntry* fam = FuzzyFindFontFamily( iFamily );

    if( !fam )
        return  nullptr;

    return  fam->FuzzyFindFontStyleKey( iStyle );
}

std::string
FFontRegistry::FuzzyFindFontPath( const std::string& iFamily, const std::string& iStyle ) const
{
    const FFontStyleEntry* stk = FuzzyFindFontStyle( iFamily, iStyle );

    if( !stk )
        return  "";

    return  stk->Path();
}

const FTextEngine&
FFontRegistry::TextEngine() const
{
    return  mTextEngine;
}

ULIS_NAMESPACE_END

