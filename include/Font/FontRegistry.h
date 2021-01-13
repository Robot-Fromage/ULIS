// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FontRegistry.h
* @author       Clement Berthaud
* @brief        This file provides the classes for the Font Registry tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <map>
#include <list>

ULIS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
/// @class FFontStyleEntry
/// @brief The FFontStyleEntry class provides a mean of storing and manipulating font files entries along with their family, style and path.
class ULIS_API FFontStyleEntry
{
public:
    // Construction / Destruction
    ~FFontStyleEntry();
    FFontStyleEntry( const std::string& iFamily, const std::string& iStyle, const std::string& iFont );

public:
    // Public API
    const std::string& Family() const;
    const std::string& Style() const;
    const std::string& Path() const;

private:
    // Private Data Members
    std::string mFamily;
    std::string mStyle;
    std::string mPath;
};

/////////////////////////////////////////////////////
/// @class FFontFamilyEntry
/// @brief The FFontFamilyEntry class provides a mean of storing and manipulating font family entries.
class ULIS_API FFontFamilyEntry
{

public:
    // Construction / Destruction
    ~FFontFamilyEntry();
    FFontFamilyEntry( const std::string& iName );

public:
    // Public API
    void AddFontStyleKey( const std::string& iStyle, const FFontStyleEntry& iFontStyleKey );
    int StyleCount() const;
    const std::map< std::string, FFontStyleEntry >& Styles() const;
    const std::string& Family() const;
    const FFontStyleEntry* FuzzyFindFontStyleKey( const std::string& iStyle ) const;

private:
    // Private Data Members
    std::string mFamily;
    std::map< std::string, FFontStyleEntry > mStyles;
};

/////////////////////////////////////////////////////
/// @class      FFontRegistry
/// @brief      The FFontRegistry class provides a mean of storing and manipulating all discoverable font entries and their file paths.
class ULIS_API FFontRegistry
{
public:
    // Construction / Destruction
    ~FFontRegistry();
    FFontRegistry( const FFontEngine& iFontEngine );

public:
    // Public API
    void AddLookupPath( const std::string& iPath );
    void AddLookupPaths( const std::list< std::string >& iPaths );
    void Refresh();
    int FamilyCount() const;
    const std::map< std::string, FFontFamilyEntry >& Records() const;
    const std::list< std::string >& LookupPaths() const;
    const FFontFamilyEntry* FuzzyFindFontFamily( const std::string& iName ) const;
    const FFontStyleEntry* FuzzyFindFontStyle( const std::string& iFamily, const std::string& iStyle ) const;
    std::string FuzzyFindFontPath( const std::string& iFamily, const std::string& iStyle ) const;
    const FFontEngine& FontEngine() const;

private:
    // Private Data Members
    std::list< std::string > mLookupPaths;
    std::map< std::string, FFontFamilyEntry > mRecords;
    const FFontEngine& mFontEngine;
};

ULIS_NAMESPACE_END

