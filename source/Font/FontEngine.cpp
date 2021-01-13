// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FontEngine.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FFontEngine class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Font/FontEngine.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS_NAMESPACE_BEGIN
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
    ULIS_ASSERT( !error, "Error initializing freetype2" );
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void*
FFontEngine::Handle() const
{
    return  mHandle;
}


ULIS_NAMESPACE_END

