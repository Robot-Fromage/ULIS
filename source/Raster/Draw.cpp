// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Draw.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the raster draw entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Raster/Draw.h"
#include "Data/Block.h"
#include "Conv/ConvBuffer.h"
#include "Conv/Conv.h"
#include "Maths/Maths.h"

ULIS3_NAMESPACE_BEGIN
void DrawDotNoAA( FBlock* iDst, const FPixelValue& iColor, const FVec2I iPos ) {
    if( !iDst->Rect().HitTest( iPos ) )
        return;

    tByte* ptr = iDst->PixelPtr( iPos.x, iPos.y );
    fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iColor.Format(), iDst->Format() );
    fptr( &iColor.FormatInfo(), iColor.Ptr(), &iDst->FormatInfo(), ptr, 1 );
}

void DrawHorizontalLineNoAA_UnsafeColor( FBlock* iDst, const tByte* iCorrectColor, int iX1, int iX2, int iY ) {
    FRect rect = iDst->Rect();
    if( !rect.InVerticalRange( iY ) )
        return;

    int x1 = FMaths::Clamp( iX1, rect.x, rect.x + rect.w - 1 );
    int x2 = FMaths::Clamp( iX2, rect.x, rect.x + rect.w - 1 );
    int xa = FMaths::Min( x1, x2 );
    int xb = FMaths::Max( x1, x2 );

    tByte* ptr = iDst->PixelPtr( xa, iY );
    tSize bpp = iDst->BytesPerPixel();
    for( int i = xa; i <= xb; ++i ) {
        memcpy( ptr, iCorrectColor, bpp );
        ptr += bpp;
    }
}

void DrawHorizontalLineNoAA( FBlock* iDst, const FPixelValue& iColor, int iX1, int iX2, int iY ) {
    FPixelValue color( iDst->Format() );
    Conv( iColor, color );
    tByte* src = color.Ptr();
    DrawHorizontalLineNoAA_UnsafeColor( iDst, src, iX1, iX2, iY );
}

void DrawVerticalLineNoAA_UnsafeColor( FBlock* iDst, const tByte* iCorrectColor, int iY1, int iY2, int iX ) {
    FRect rect = iDst->Rect();
    if( !rect.InHorizontalRange( iX ) )
        return;

    int y1 = FMaths::Clamp( iY1, rect.y, rect.y + rect.h - 1 );
    int y2 = FMaths::Clamp( iY2, rect.y, rect.y + rect.h - 1 );
    int ya = FMaths::Min( y1, y2 );
    int yb = FMaths::Max( y1, y2 );

    tByte* ptr = iDst->PixelPtr( iX, ya );
    tSize bpp = iDst->BytesPerPixel();
    tSize bps = iDst->BytesPerScanLine();
    for( int i = ya; i <= yb; ++i ) {
        memcpy( ptr, iCorrectColor, bpp );
        ptr += bps;
    }
}

void DrawVerticalLineNoAA( FBlock* iDst, const FPixelValue& iColor, int iY1, int iY2, int iX ) {
    FPixelValue color( iDst->Format() );
    Conv( iColor, color );
    tByte* src = color.Ptr();
    DrawVerticalLineNoAA_UnsafeColor( iDst, src, iY1, iY2, iX );
}


void DrawRectOutlineNoAA( FBlock* iDst, const FPixelValue& iColor, const FRect& iRect ) {
    FPixelValue color( iDst->Format() );
    Conv( iColor, color );
    tByte* src = color.Ptr();
    DrawHorizontalLineNoAA_UnsafeColor( iDst, src, iRect.x, iRect.x + iRect.w, iRect.y );
    DrawHorizontalLineNoAA_UnsafeColor( iDst, src, iRect.x, iRect.x + iRect.w, iRect.y + iRect.h );
    DrawVerticalLineNoAA_UnsafeColor(   iDst, src, iRect.y, iRect.y + iRect.h, iRect.x );
    DrawVerticalLineNoAA_UnsafeColor(   iDst, src, iRect.y, iRect.y + iRect.h, iRect.x + iRect.w );
}

void DrawUniformGridOutlineNoAA( FBlock* iDst, const FPixelValue& iColor, const FRect& iRect, int iNumSubdiv ) {
    FPixelValue color( iDst->Format() );
    Conv( iColor, color );
    tByte* src = color.Ptr();
    int stepX = iRect.w / iNumSubdiv;
    int stepY = iRect.h / iNumSubdiv;
    for( int x = iRect.x; x <= iRect.x + iRect.w; x += stepX )
        DrawVerticalLineNoAA_UnsafeColor(   iDst, src, iRect.y, iRect.y + iRect.h, x );

    for( int y = iRect.y; y <= iRect.y + iRect.h; y += stepY )
        DrawHorizontalLineNoAA_UnsafeColor( iDst, src, iRect.x, iRect.x + iRect.w, y );

}

ULIS3_NAMESPACE_END

