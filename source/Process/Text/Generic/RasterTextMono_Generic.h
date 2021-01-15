// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         RasterTextMono_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic trace text entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Block.h"
#include "Font/Font.h"
#include "Process/Text/TextArgs.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

ULIS_NAMESPACE_BEGIN
// detail
namespace detail {

static ULIS_FORCEINLINE float AlphaBlendAlpha( float iAs, float iAb ) {
    return  iAs + iAb * ( 1.f - iAs );
}

static ULIS_FORCEINLINE float AlphaBlendChannel( float iCs, float iCb, float iAs, float iAb, float iAr ) {
    return  iAr == 0.f ? 0.f : ( iCs * iAs + iCb * iAb * ( 1.f - iAs ) ) / iAr;
}

template< typename T >
void
RasterPixmap(
      const FTextCommandArgs* cargs
    , FT_Bitmap* iBitmap
    , FT_Int iX
    , FT_Int iY
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    int width   = cargs->dst.Width();
    int height  = cargs->dst.Height();
    int xmax = iX + iBitmap->width;
    int ymax = iY + iBitmap->rows;
    uint8* ULIS_RESTRICT dst = cargs->dst.Bits() + iY * cargs->dst.BytesPerScanLine() + iX * fmt.BPP;
    uint32 stride = ( width - iBitmap->width ) * fmt.BPP;

    for( int y = iY, v = 0; y < ymax; ++y, ++v ) {
        if( y < 0 || y >= height ) {
            dst += stride;
            continue;
        }

        for( int x = iX, u = 0; x < xmax; ++x, ++u ) {
            if( x < 0 || x >= width ) {
                dst += fmt.BPP;
                continue;
            }

            ufloat srcAlpha = ConvType< uint8, ufloat >( iBitmap->buffer[ v * iBitmap->width + u ] );
            ufloat dstAlpha = 1.f;
            if( fmt.HEA ) {
                srcAlpha = srcAlpha * ConvType< T, ufloat >( cargs->color.AlphaT< T >() );
                dstAlpha = TYPE2FLOAT( dst, fmt.AID );
            }
            ufloat resAlpha = AlphaBlendAlpha( srcAlpha, dstAlpha );
            for( uint8 j = 0; j < fmt.NCC; ++j )
            {
                uint8 r = fmt.IDT[j];
                ufloat srcvf = ConvType< T, ufloat >( cargs->color.ChannelT< T >( r ) );
                ufloat dstvf = TYPE2FLOAT( dst, r );
                FLOAT2TYPE( dst, r, AlphaBlendChannel( srcvf, dstvf, srcAlpha, dstAlpha, resAlpha ) );
            }
            if( fmt.HEA ) FLOAT2TYPE( dst, fmt.AID, resAlpha );
            dst += fmt.BPP;
        }

        dst += stride;
    }
}


template< typename T >
void
RasterBitmap(
      const FTextCommandArgs* cargs
    , FT_Bitmap* iBitmap
    , FT_Int iX
    , FT_Int iY
)
{
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    int width   = cargs->dst.Width();
    int height  = cargs->dst.Height();
    int xmax = iX + iBitmap->width;
    int ymax = iY + iBitmap->rows;
    uint8* ULIS_RESTRICT dst = cargs->dst.Bits() + iY * cargs->dst.BytesPerScanLine() + iX * fmt.BPP;
    uint32 stride = ( width - iBitmap->width ) * fmt.BPP;

    for( int y = iY, v = 0; y < ymax; ++y, ++v ) {
        if( y < 0 || y >= height ) {
            dst += stride;
            continue;
        }

        for( int x = iX, u = 0; x < xmax; ++x, ++u ) {
            if( x < 0 || x >= width ) {
                dst += fmt.BPP;
                continue;
            }

            ufloat srcAlpha = iBitmap->buffer[ ( v >> 0 ) * ( iBitmap->width >> 3 ) + ( u >> 3 ) ] & ( 0x80 >> ( u % 8 ) ) ? 1.f : 0.f;
            ufloat dstAlpha = 1.f;
            if( fmt.HEA ) {
                srcAlpha = srcAlpha * ConvType< T, ufloat >( cargs->color.AlphaT< T >() );
                dstAlpha = TYPE2FLOAT( dst, fmt.AID );
            }
            ufloat resAlpha = AlphaBlendAlpha( srcAlpha, dstAlpha );
            for( uint8 j = 0; j < fmt.NCC; ++j )
            {
                uint8 r = fmt.IDT[j];
                ufloat srcvf = ConvType< T, ufloat >( cargs->color.ChannelT< T >( r ) );
                ufloat dstvf = TYPE2FLOAT( dst, r );
                FLOAT2TYPE( dst, r, AlphaBlendChannel( srcvf, dstvf, srcAlpha, dstAlpha, resAlpha ) );
            }
            if( fmt.HEA ) FLOAT2TYPE( dst, fmt.AID, resAlpha );
            dst += fmt.BPP;
        }

        dst += stride;
    }
}

} // namespace detail

/////////////////////////////////////////////////////
// Invocations RasterText
template< typename T >
void
InvokeRasterTextMono_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FTextCommandArgs* cargs
)
{
    FT_Error error = 0;
    FT_Face face = reinterpret_cast< FT_Face >( cargs->font.FontHandle() );
    error = FT_Set_Pixel_Sizes( face, 0, cargs->fontSize );
    ULIS_ASSERT( !error, "Error setting face size" );
    FT_GlyphSlot slot = face->glyph;
    FT_Vector pen { 0, 0 };
    int height = cargs->dst.Height();
    const wchar_t* str = cargs->text.c_str();
    const size_t len = static_cast< size_t >( cargs->text.size() );
    for( int n = 0; n < len; ++n ) {
        FT_Set_Transform( face, const_cast< FT_Matrix* >( &( cargs->matrix ) ), &pen );
        FT_UInt glyph_index = FT_Get_Char_Index( face, str[n] );
        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
        ULIS_ASSERT( !error, "Error loading glyph" );
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_MONO );
        ULIS_ASSERT( !error, "Error rendering glyph" );
        detail::RasterBitmap< T >( cargs, &slot->bitmap, cargs->position.x + slot->bitmap_left, -slot->bitmap_top + cargs->position.y );
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }
}

template< typename T >
void
InvokeRasterTextAAMono_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FTextCommandArgs* cargs
)
{
    FT_Error error = 0;
    FT_Face face = reinterpret_cast< FT_Face >( cargs->font.FontHandle() );
    error = FT_Set_Pixel_Sizes( face, 0, cargs->fontSize );
    ULIS_ASSERT( !error, "Error setting face size" );
    FT_GlyphSlot slot = face->glyph;
    FT_Vector pen { 0, 0 };
    int height = cargs->dst.Height();
    const wchar_t* str = cargs->text.c_str();
    const size_t len = static_cast< size_t >( cargs->text.size() );
    for( int n = 0; n < len; ++n ) {
        FT_Set_Transform( face, const_cast< FT_Matrix* >( &( cargs->matrix ) ), &pen );
        FT_UInt glyph_index = FT_Get_Char_Index( face, str[n] );
        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
        ULIS_ASSERT( !error, "Error loading glyph" );
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        ULIS_ASSERT( !error, "Error rendering glyph" );
        detail::RasterPixmap< T >( cargs, &slot->bitmap, cargs->position.x + slot->bitmap_left, -slot->bitmap_top + cargs->position.y );
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }
}


ULIS_NAMESPACE_END

