// Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         FontEngine.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FFontEngine class.
* @copyright    Copyright � 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Base/Core.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FFontEngine
/// @brief      The FFontEngine class provides a thin wrapper around the freetype library.
class ULIS2_API FFontEngine
{
public:
    // Construction / Destruction
    ~FFontEngine();
    FFontEngine();
    FFontEngine( const FFontEngine& ) = delete;

public:
    // Public API
    FT_Library Handle() const;

private:
    // Private Data Members
    FT_Library mHandle;
};
ULIS2_NAMESPACE_END

