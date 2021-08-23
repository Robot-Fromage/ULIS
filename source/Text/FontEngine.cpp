// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         FontEngine.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FFontEngine class.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Text/FontEngine.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FFontEngine
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FFontEngine::~FFontEngine()
{
    FT_Done_FreeType( reinterpret_cast< FT_Library >( mHandle ) );
}


FFontEngine::FFontEngine()
    : mHandle( nullptr )
{
    FT_Error error = FT_Init_FreeType( reinterpret_cast< FT_Library* >( &mHandle ) );
    ULIS2_ASSERT( !error, "Error initializing freetype2" );
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void*
FFontEngine::Handle() const
{
    return  mHandle;
}


ULIS2_NAMESPACE_END

