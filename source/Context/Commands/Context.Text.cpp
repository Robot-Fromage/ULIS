// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Text.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the Text API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Text/Text.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Font/Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

ULIS_NAMESPACE_BEGIN
ulError
FContext::RasterText(
      FBlock& iBlock
    , const std::wstring& iText
    , const FFont& iFont
    , uint32 iFontSize
    , const FMat3F& iTransform
    , const ISample& iColor
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI rect = iBlock.Rect();
    const FRectI roi = TextMetrics( iText, iFont, iFontSize, iTransform ) & rect;

    // Check no-op
    if( roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward Arguments Baking
    FT_Matrix matrix;
    matrix.xx = (FT_Fixed)( iTransform[0].x * 0x10000L );
    matrix.xy = (FT_Fixed)( iTransform[0].y * 0x10000L );
    matrix.yx = (FT_Fixed)( iTransform[1].x * 0x10000L );
    matrix.yy = (FT_Fixed)( iTransform[1].y * 0x10000L );
    int dx = static_cast< int >( iTransform[2].x );
    int dy = static_cast< int >( iTransform[2].y );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleRasterText
            , new FTextCommandArgs(
                  iBlock
                , roi
                , iText
                , iFont
                , iFontSize
                , iColor.ToFormat( iBlock.Format() )
                , matrix
                , FVec2I( dx, dy )
            )
            , iPolicy
            , false
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::RasterTextAA(
      FBlock& iBlock
    , const std::wstring& iText
    , const FFont& iFont
    , uint32 iFontSize
    , const FMat3F& iTransform
    , const ISample& iColor
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI rect = iBlock.Rect();
    const FRectI roi = TextMetrics( iText, iFont, iFontSize, iTransform ) & rect;

    // Check no-op
    if( roi.Area() <= 0 )
        return  FinishEventNo_OP( iNumWait, iWaitList, iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Forward Arguments Baking
    FT_Matrix matrix;
    matrix.xx = (FT_Fixed)( iTransform[0].x * 0x10000L );
    matrix.xy = (FT_Fixed)( iTransform[0].y * 0x10000L );
    matrix.yx = (FT_Fixed)( iTransform[1].x * 0x10000L );
    matrix.yy = (FT_Fixed)( iTransform[1].y * 0x10000L );
    int dx = static_cast< int >( iTransform[2].x );
    int dy = static_cast< int >( iTransform[2].y );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleRasterTextAA
            , new FTextCommandArgs(
                  iBlock
                , roi
                , iText
                , iFont
                , iFontSize
                , iColor.ToFormat( iBlock.Format() )
                , matrix
                , FVec2I( dx, dy )
            )
            , iPolicy
            , false
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , roi
        )
    );

    return  ULIS_NO_ERROR;
}

//static
FRectI
FContext::TextMetrics(
      const std::wstring& iText
    , const FFont& iFont
    , uint32 iFontSize
    , const FMat3F& iTransform
)
{
    FT_Matrix matrix;
    matrix.xx = (FT_Fixed)( iTransform[0].x * 0x10000L );
    matrix.xy = (FT_Fixed)( iTransform[0].y * 0x10000L );
    matrix.yx = (FT_Fixed)( iTransform[1].x * 0x10000L );
    matrix.yy = (FT_Fixed)( iTransform[1].y * 0x10000L );
    int dx = static_cast< int >( iTransform[2].x );
    int dy = static_cast< int >( iTransform[2].y );

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
    FT_Face face = reinterpret_cast< FT_Face >( iFont.FontHandle() );
    error = FT_Set_Pixel_Sizes( face, 0, iFontSize );
    ULIS_ASSERT( !error, "Error setting face size" );
    slot = face->glyph;
    pen.x = 0;
    pen.y = 0;
    int autobaseline = (int)( iFontSize * 0.7 );

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

