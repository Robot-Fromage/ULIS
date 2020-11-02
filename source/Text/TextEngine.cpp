// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         TextEngine.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FTextEngine class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Text/TextEngine.h"

#include <ft2build.h>
#include FT_FREETYPE_H

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FTextEngine
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FTextEngine::~FTextEngine()
{
    FT_Done_FreeType( reinterpret_cast< FT_Library >( mHandle ) );
}


FTextEngine::FTextEngine()
    : mHandle( nullptr )
{
    FT_Error error = FT_Init_FreeType( reinterpret_cast< FT_Library* >( &mHandle ) );
    ULIS_ASSERT( !error, "Error initializing freetype2" );
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
void*
FTextEngine::Handle() const
{
    return  mHandle;
}


ULIS_NAMESPACE_END

