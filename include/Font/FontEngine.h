// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FontEngine.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FFontEngine class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FFontEngine
/// @brief      The FFontEngine class provides a thin wrapper around the freetype library.
class ULIS_API FFontEngine
{
public:
    // Construction / Destruction
    ~FFontEngine();
    FFontEngine();
    FFontEngine( const FFontEngine& ) = delete;

public:
    // Public API
    void* Handle() const;

private:
    // Private Data Members
    void* mHandle;
};

ULIS_NAMESPACE_END

