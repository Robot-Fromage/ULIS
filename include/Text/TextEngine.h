// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TextEngine.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FTextEngine class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FTextEngine
/// @brief      The FTextEngine class provides a thin wrapper around the freetype library.
class ULIS_API FTextEngine
{
public:
    // Construction / Destruction
    ~FTextEngine();
    FTextEngine();
    FTextEngine( const FTextEngine& ) = delete;

public:
    // Public API
    void* Handle() const;

private:
    // Private Data Members
    void* mHandle;
};
ULIS_NAMESPACE_END

