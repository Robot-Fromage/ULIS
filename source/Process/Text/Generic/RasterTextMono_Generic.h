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

/*
template< typename T >
void
RasterBitmap( std::shared_ptr< _FPrivateTextInfo > iTextParams, FT_Bitmap* iBitmap, FT_Int iX, FT_Int iY ) {
    const FFormatMetrics& fmtInfo = iTextParams->destination->FormatMetrics();
    int width = iTextParams->destination->Width();
    int height = iTextParams->destination->Height();
    int bps = iTextParams->destination->BytesPerScanLine();

    int xmax = iX + iBitmap->width;
    int ymax = iY + iBitmap->rows;

    uint8* dst = iTextParams->destination->Bits() + ( iY * (int64)bps ) + ( iX * (int64)fmtInfo.BPP );
    uint32 jmp = ( width - iBitmap->width ) * fmtInfo.BPP; 

    int x, y, p, q;
    for( y = iY, q = 0; y < ymax; ++y, ++q ) {
        for( x = iX, p = 0; x < xmax; ++x, ++p ) {
            if( x < 0 || y < 0 || x >= width || y >= height ) {
                dst += fmtInfo.BPP;
                continue;
            }

            float srcAlpha = ConvType< uint8, float >( iBitmap->buffer[ q * iBitmap->width + p ] );
            if( fmtInfo.HEA ) srcAlpha = srcAlpha * TYPE2FLOAT( iTextParams->color, fmtInfo.AID );
            float dstAlpha = fmtInfo.HEA ? TYPE2FLOAT( dst, fmtInfo.AID ) : 1.f;
            float resAlpha = AlphaBlendAlpha( srcAlpha, dstAlpha );
            for( uint8 j = 0; j < fmtInfo.NCC; ++j )
            {
                uint8 r = fmtInfo.IDT[j];
                float srcvf = TYPE2FLOAT( iTextParams->color, r );
                float dstvf = TYPE2FLOAT( dst, r );
                FLOAT2TYPE( dst, r, AlphaBlendChannel( srcvf, dstvf, srcAlpha, dstAlpha, resAlpha ) );
            }
            if( fmtInfo.HEA ) FLOAT2TYPE( dst, fmtInfo.AID, resAlpha );
            dst += fmtInfo.BPP;
        }

        dst += jmp;
    }
}
*/
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
}

template< typename T >
void
InvokeRasterTextAAMono_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FTextCommandArgs* cargs
)
{
    const wchar_t* str = cargs->text.c_str();
    size_t len = static_cast< size_t >( cargs->text.size() );

    FT_GlyphSlot  slot;
    FT_Vector     pen;

    FT_Error error = 0;
    FT_Face face = reinterpret_cast< FT_Face >( cargs->font.FontHandle() );
    error = FT_Set_Pixel_Sizes( face, 0, cargs->fontSize );
    ULIS_ASSERT( !error, "Error setting face size" );

    slot = face->glyph;
    pen.x = 0;
    pen.y = 0;
    //int autobaseline = (int)( iTextParams->size * 0.7 );
    int autobaseline = int( 0 );

    for( int n = 0; n < len; ++n ) {
        FT_Set_Transform( face, const_cast< FT_Matrix* >( &( cargs->matrix ) ), &pen );
        FT_UInt glyph_index = FT_Get_Char_Index( face, str[n] );

        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
        ULIS_ASSERT( !error, "Error loading glyph" );

        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        ULIS_ASSERT( !error, "Error rendering glyph" );

        //RasterBitmap< T >( iTextParams, &slot->bitmap, iTextParams->position.x + slot->bitmap_left, iTextParams->position.y + ( autobaseline - slot->bitmap_top ) );
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }
}


ULIS_NAMESPACE_END

