// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Tile.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Tile class.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <atomic>

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// FTileElement
struct ULIS2_API FTileElement {
    ~FTileElement();
    FTileElement();
    FTileElement( FBlock* iPtr );
    void DecreaseRefCount();
    void IncreaseRefCount();
    FBlock* mBlock;
    uint32  mHash;
    std::atomic< uint32 >   mRefCount;
    std::atomic< bool >     mDirty;
};
ULIS2_NAMESPACE_END

