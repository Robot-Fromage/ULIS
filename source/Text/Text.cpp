// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Text.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Text entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Text/Text.h"
#include "Base/HostDeviceInfo.h"
#include "Conv/Conv.h"
#include "Conv/ConvBuffer.h"
#include "Data/Block.h"
#include "Data/Pixel.h"
#include "Maths/Geometry.h"
#include "Maths/Transform2D.h"
#include "Text/Font.h"
#include "Text/Dispatch/TextInfo.h"
#include "Text/Dispatch/Dispatch.ipp"

#include FT_GLYPH_H

ULIS2_NAMESPACE_BEGIN
void
RenderText( FThreadPool*            iThreadPool
          , bool                    iBlocking
          , uint32                  iPerfIntent
          , const FHostDeviceInfo&  iHostDeviceInfo
          , bool                    iCallCB
          , FBlock*                 iDestination
          , const std::wstring      iText
          , const FFont&            iFont
          , int                     iSize
          , const IPixel&           iColor
          , const FTransform2D&     iTransform
          , bool                    iAntialiasing )
{
    // Assertions
    ULIS2_ASSERT( iDestination,             "Bad source."                                           );
    ULIS2_ASSERT( iThreadPool,              "Bad pool."                                             );
    ULIS2_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    std::shared_ptr< _FPrivateTextInfo > forwardTextParams = std::make_shared< _FPrivateTextInfo >();
    _FPrivateTextInfo& alias = *forwardTextParams;
    alias.pool              = iThreadPool;
    alias.blocking          = iBlocking;
    alias.hostDeviceInfo    = &iHostDeviceInfo;
    alias.perfIntent        = iPerfIntent;
    alias.destination       = iDestination;
    alias.text              = &iText;
    alias.font              = &iFont;
    alias.size              = iSize;
    alias.antialiasing      = iAntialiasing;

    { // Conv
        fpDispatchedConvInvoke fptrconv = QueryDispatchedConvInvokeForParameters( iColor.Format(), iDestination->Format() );
        ULIS2_ASSERT( fptrconv, "No Conversion invocation found" );
        fptrconv( &iColor.FormatInfo(), iColor.Ptr(), &iDestination->FormatInfo(), alias.color, 1 );
    }

    { // Mat
        const glm::mat3& _mat = iTransform.Matrix();
        alias.matrix.xx = (FT_Fixed)( _mat[0].x * 0x10000L );
        alias.matrix.xy = (FT_Fixed)( _mat[0].y * 0x10000L );
        alias.matrix.yx = (FT_Fixed)( _mat[1].x * 0x10000L );
        alias.matrix.yy = (FT_Fixed)( _mat[1].y * 0x10000L );
        alias.position = FVec2I( static_cast< int >( _mat[2].x ), static_cast< int >( _mat[2].y )  );
    }

    // Query
    fpDispatchedTextFunc fptr = QueryDispatchedTextFunctionForParameters( iDestination->Type() );
    ULIS2_ASSERT( fptr, "No invocation found" );
    fptr( forwardTextParams );

    // Invalidate
    iDestination->Invalidate( iCallCB );
}


FRect
TextMetrics( std::wstring           iText
           , const FFont&           iFont
           , int                    iSize
           , const FTransform2D&    iTransform ) {
    const glm::mat3& _mat = iTransform.Matrix();
    FT_Matrix matrix;
    matrix.xx = (FT_Fixed)( _mat[0].x * 0x10000L );
    matrix.xy = (FT_Fixed)( _mat[0].y * 0x10000L );
    matrix.yx = (FT_Fixed)( _mat[1].x * 0x10000L );
    matrix.yy = (FT_Fixed)( _mat[1].y * 0x10000L );
    int dx = static_cast< int >( _mat[2].x );
    int dy = static_cast< int >( _mat[2].y );

    FRect result;
    result.x = static_cast< int >( dx );
    result.y = static_cast< int >( dy );
    result.w = 1;
    result.h = 1;

    const wchar_t* str = iText.c_str();
    int len = (int)iText.size();

    FT_GlyphSlot  slot;
    FT_Vector     pen;

    FT_Error error = 0;
    FT_Face face = reinterpret_cast< FT_Face >( iFont.Handle() );
    error = FT_Set_Pixel_Sizes( face, 0, iSize );
    ULIS2_ASSERT( !error, "Error setting face size" );
    slot = face->glyph;
    pen.x = 0;
    pen.y = 0;
    int autobaseline = (int)( iSize * 0.7 );

    for( int n = 0; n < len; ++n ) {
        FT_Set_Transform( face, &matrix, &pen );
        FT_UInt glyph_index = FT_Get_Char_Index( face, str[n] );
        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_BITMAP_METRICS_ONLY );
        ULIS2_ASSERT( !error, "Error loading glyph" );

        FRect box = FRect::FromXYWH( dx + slot->bitmap_left, dy + ( autobaseline - slot->bitmap_top ), slot->bitmap.width, slot->bitmap.rows );
        result = result | box;

        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    return  result;
}

ULIS2_NAMESPACE_END

