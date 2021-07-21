// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Tile.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FTile class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Units.h"
#include <atomic>

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4251) // Shut warning C4251 dll export of stl classes
/////////////////////////////////////////////////////
/// @class      FTile
/// @brief      The FTile class provides a mean of keeping track of tile data
///             in a sparse image context.
struct ULIS_API FTile
{
    ~FTile();
    FTile();
    FTile( tClient iPtr );
    void DecreaseRefCount();
    void IncreaseRefCount();

    tClient mClient;
    uint32  mHash;
    std::atomic< uint32 >   mRefCount;
    std::atomic< bool >     mDirty;
};
#pragma warning(pop)

ULIS_NAMESPACE_END

