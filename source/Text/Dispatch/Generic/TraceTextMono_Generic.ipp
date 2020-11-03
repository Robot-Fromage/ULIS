// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         TraceTextMono_Generic.ipp
* @author       Clement Berthaud
* @brief        This file provides the declaration for the generic trace text entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Text/Dispatch/TextInfo.h"
#include "Maths/Geometry.h"

ULIS2_NAMESPACE_BEGIN
float AlphaBlendAlpha( float iAs, float iAb ) {
    return  iAs + iAb * ( 1.f - iAs );
}

float AlphaBlendChannel( float iCs, float iCb, float iAs, float iAb, float iAr ) {
    return  iAr == 0.f ? 0.f : ( iCs * iAs + iCb * iAb * ( 1.f - iAs ) ) / iAr;
}

template< typename T >
void
RasterBitmap( std::shared_ptr< _FPrivateTextInfo > iTextParams, FT_Bitmap* iBitmap, FT_Int iX, FT_Int iY ) {
    const FFormatInfo& fmtInfo = iTextParams->destination->FormatInfo();
    int width = iTextParams->destination->Width();
    int height = iTextParams->destination->Height();
    int bps = iTextParams->destination->BytesPerScanLine();

    int xmax = iX + iBitmap->width;
    int ymax = iY + iBitmap->rows;

    tByte* dst = iTextParams->destination->DataPtr() + ( iY * (int64)bps ) + ( iX * (int64)fmtInfo.BPP );
    tSize jmp = ( width - iBitmap->width ) * fmtInfo.BPP; 

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

template< typename T >
void
TraceTextMono_Generic( std::shared_ptr< _FPrivateTextInfo > iTextParams ) {
    const wchar_t* str = iTextParams->text->c_str();
    size_t len = static_cast< size_t >( iTextParams->text->size() );

    FT_GlyphSlot  slot;
    FT_Vector     pen;

    FT_Error error = 0;
    FT_Face face = reinterpret_cast< FT_Face >( iTextParams->font->Handle() );
    error = FT_Set_Pixel_Sizes( face, 0, iTextParams->size );
    ULIS2_ASSERT( !error, "Error setting face size" );

    slot = face->glyph;
    pen.x = 0;
    pen.y = 0;
    int autobaseline = (int)( iTextParams->size * 0.7 );

    for( int n = 0; n < len; ++n ) {
        FT_Set_Transform( face, &(iTextParams->matrix), &pen );
        FT_UInt glyph_index = FT_Get_Char_Index( face, str[n] );
        error = FT_Load_Glyph( face, glyph_index, FT_LOAD_DEFAULT );
        ULIS2_ASSERT( !error, "Error loading glyph" );
        error = FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
        ULIS2_ASSERT( !error, "Error rendering glyph" );
        RasterBitmap< T >( iTextParams, &slot->bitmap, iTextParams->position.x + slot->bitmap_left, iTextParams->position.y + ( autobaseline - slot->bitmap_top ) );
        pen.x += slot->advance.x;
        pen.y += slot->advance.y;
    }
}

ULIS2_NAMESPACE_END

