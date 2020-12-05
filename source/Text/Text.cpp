// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Text.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Text entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Text/Text.h"
#include "System/Device.h"
#include "Conv/Conv.h"
#include "Conv/Conv.h"
#include "Image/Block.h"
#include "Image/Pixel.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Geometry/Transformation2D.h"
#include "Text/Font.h"
#include "Text/Dispatch/TextInfo.h"
#include "Text/Dispatch/Dispatch.ipp"

#include FT_GLYPH_H

ULIS_NAMESPACE_BEGIN
void
RenderText( FOldThreadPool*            iOldThreadPool
          , bool                    iBlocking
          , uint32                  iPerfIntent
          , const FHardwareMetrics&  iHostDeviceInfo
          , bool                    iCallCB
          , FBlock*                 iDestination
          , const std::wstring      iText
          , const FFont&            iFont
          , int                     iSize
          , const ISample&           iColor
          , const FTransformation2D&     iTransform
          , bool                    iAntialiasing )
{
    // Assertions
    ULIS_ASSERT( iDestination,             "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,              "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    std::shared_ptr< _FPrivateTextInfo > forwardTextParams = std::make_shared< _FPrivateTextInfo >();
    _FPrivateTextInfo& alias = *forwardTextParams;
    alias.pool              = iOldThreadPool;
    alias.blocking          = iBlocking;
    alias.hostDeviceInfo    = &iHostDeviceInfo;
    alias.perfIntent        = iPerfIntent;
    alias.destination       = iDestination;
    alias.text              = &iText;
    alias.font              = &iFont;
    alias.size              = iSize;
    alias.antialiasing      = iAntialiasing;

    { // Conv
        fpConvertFormat fptrconv = QueryDispatchedConvertFormatInvocation( iColor.Format(), iDestination->Format() );
        ULIS_ASSERT( fptrconv, "No Conversion invocation found" );
        fptrconv( iColor.FormatMetrics(), iColor.Bits(), iDestination->FormatMetrics(), alias.color, 1 );
    }

    { // Mat
        const FMat3F& mat = iTransform.Matrix();
        alias.matrix.xx = (FT_Fixed)( mat[0].x * 0x10000L );
        alias.matrix.xy = (FT_Fixed)( mat[0].y * 0x10000L );
        alias.matrix.yx = (FT_Fixed)( mat[1].x * 0x10000L );
        alias.matrix.yy = (FT_Fixed)( mat[1].y * 0x10000L );
        alias.position = FVec2I( static_cast< int >( mat[2].x ), static_cast< int >( mat[2].y )  );
    }

    // Query
    fpDispatchedTextFunc fptr = QueryDispatchedTextFunctionForParameters( iDestination->Type() );
    ULIS_ASSERT( fptr, "No invocation found" );
    fptr( forwardTextParams );

    // Invalidate
    iDestination->Dirty( iCallCB );
}


FRectI
TextMetrics( std::wstring           iText
           , const FFont&           iFont
           , int                    iSize
           , const FTransformation2D&    iTransform ) {
    const FMat3F& mat = iTransform.Matrix();
    FT_Matrix matrix;
    matrix.xx = (FT_Fixed)( mat[0].x * 0x10000L );
    matrix.xy = (FT_Fixed)( mat[0].y * 0x10000L );
    matrix.yx = (FT_Fixed)( mat[1].x * 0x10000L );
    matrix.yy = (FT_Fixed)( mat[1].y * 0x10000L );
    int dx = static_cast< int >( mat[2].x );
    int dy = static_cast< int >( mat[2].y );

    FRectI result;
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
    ULIS_ASSERT( !error, "Error setting face size" );
    slot = face->glyph;
    pen.x = 0;
    pen.y = 0;
    int autobaseline = (int)( iSize * 0.7 );

    for( int n = 0; n < len; ++n ) {
        FT_Set_Transform( face, &matrix, &pen );
        FT_UInt glyph_index = FT_Get_Char_Index( face, str[n] );
        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_BITMAP_METRICS_ONLY );
        ULIS_ASSERT( !error, "Error loading glyph" );

        FRectI box = FRectI::FromXYWH( dx + slot->bitmap_left, dy + ( autobaseline - slot->bitmap_top ), slot->bitmap.width, slot->bitmap.rows );
        result = result | box;

        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }

    return  result;
}

ULIS_NAMESPACE_END

