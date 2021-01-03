// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FilePathRegistry.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FFilePathRegistry class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "System/FilePathRegistry.h"
#include "String/Utils.h"

#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FilePath.h>

#include <iostream>
#include <cstring>
#include <cassert>
#include <algorithm>

using namespace cppfs;

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// GatherEntriesRecursive
void GatherEntriesRecursive( const cppfs::FileHandle& iDir, const std::list< std::string >& iFilters, std::list< std::string >* oList )
{
    std::vector< std::string > entries = iDir.listFiles();
    std::string dir_path = iDir.path();

    for( auto it : entries ) {
        std::string full_entry_path = dir_path + it;
        FileHandle entry = fs::open( full_entry_path );

        if( entry.isDirectory() )
            GatherEntriesRecursive( entry, iFilters, oList );

        if( entry.isFile() ) {
            FilePath path( entry.path() );
            std::string ext = path.extension();
            bool match = false;

            for( auto filter : iFilters ) {
                if( ext == filter )
                    match = true;
            }

            if( match )
                oList->push_back( full_entry_path );
        }
    }
}

/////////////////////////////////////////////////////
// FFilePathRegistry
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FFilePathRegistry::~FFilePathRegistry()
{
}

FFilePathRegistry::FFilePathRegistry()
{
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
const FFilePathRegistry::tStringList&
FFilePathRegistry::LookupPaths() const
{
    return  mLookupPaths;
}

const FFilePathRegistry::tStringList&
FFilePathRegistry::Filters() const
{
    return  mFilters;
}

const FFilePathRegistry::tRecords&
FFilePathRegistry::Records() const
{
    return  mRecords;
}

void
FFilePathRegistry::AddLookupPath( const std::string& iPath )
{
    mLookupPaths.push_back( iPath );
}

void
FFilePathRegistry::AddLookupPaths( const tStringList& iPaths )
{
    for( auto it : iPaths )
        mLookupPaths.push_back( it );
}

void
FFilePathRegistry::AddFilter( const std::string& iFilter )
{
    mFilters.push_back( iFilter );
}

void
FFilePathRegistry::AddFilters( const tStringList& iFilters )
{
    for( auto it : iFilters )
        mFilters.push_back( it );
}

void
FFilePathRegistry::Parse()
{
    mRecords.clear();
    tStringList list;
    for( auto it : mLookupPaths ) {
        FileHandle dir = fs::open( it );

        if( ( !dir.exists() ) || ( !dir.isDirectory() ) )
            continue;

        GatherEntriesRecursive( dir, mFilters, &list );
    }

    for( auto it : list )
        mRecords.emplace( MakeName( it ), it );
}

const std::string
FFilePathRegistry::FilePathForExactMatchingName( const std::string& iName ) const
{
    auto it = mRecords.find( iName );
    return  ( it == mRecords.end() ) ? "" : it->second;
}

const std::string
FFilePathRegistry::FilePathForClosestMatchingName( const std::string& iName ) const
{
    if( mRecords.size() == 0 )
        return  "";

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
    return  FilePathForExactMatchingName( std::get< 1 >( matches[0] ) );
}

//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- Private API
std::string
FFilePathRegistry::MakeName( const std::string& iFile ) const
{
    FilePath path( iFile );
    return  path.baseName();
}

ULIS_NAMESPACE_END

