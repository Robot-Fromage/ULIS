// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Draw.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the raster draw entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Draw/Draw.h"
#include "Image/Block.h"
#include "Conv/ConvertFormatInvocations.h"
#include "Conv/Conv.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
void DrawDotNoAA( FBlock* iDst, const FColor& iColor, const FVec2I iPos ) {
    if( !iDst->Rect().HitTest( iPos ) )
        return;

    uint8* ptr = iDst->PixelBits( iPos.x, iPos.y );
    fpConvertFormat fptr = QueryDispatchConvertFormatInvocation( iColor.Format(), iDst->Format() );
    fptr( iColor.FormatMetrics(), iColor.Bits(), iDst->FormatMetrics(), ptr, 1 );
}

void DrawHorizontalLineNoAA_UnsafeColor( FBlock* iDst, const uint8* iCorrectColor, int iX1, int iX2, int iY ) {
    FRectI rect = iDst->Rect();
    if( !rect.InVerticalRange( iY ) )
        return;

    int x1 = FMath::Clamp( iX1, rect.x, rect.x + rect.w - 1 );
    int x2 = FMath::Clamp( iX2, rect.x, rect.x + rect.w - 1 );
    int xa = FMath::Min( x1, x2 );
    int xb = FMath::Max( x1, x2 );

    uint8* ptr = iDst->PixelBits( xa, iY );
    uint32 bpp = iDst->BytesPerPixel();
    for( int i = xa; i <= xb; ++i ) {
        memcpy( ptr, iCorrectColor, bpp );
        ptr += bpp;
    }
}

void DrawHorizontalLineNoAA( FBlock* iDst, const FColor& iColor, int iX1, int iX2, int iY ) {
    FColor color( iDst->Format() );
    Conv( iColor, color );
    uint8* src = color.Bits();
    DrawHorizontalLineNoAA_UnsafeColor( iDst, src, iX1, iX2, iY );
}

void DrawVerticalLineNoAA_UnsafeColor( FBlock* iDst, const uint8* iCorrectColor, int iY1, int iY2, int iX ) {
    FRectI rect = iDst->Rect();
    if( !rect.InHorizontalRange( iX ) )
        return;

    int y1 = FMath::Clamp( iY1, rect.y, rect.y + rect.h - 1 );
    int y2 = FMath::Clamp( iY2, rect.y, rect.y + rect.h - 1 );
    int ya = FMath::Min( y1, y2 );
    int yb = FMath::Max( y1, y2 );

    uint8* ptr = iDst->PixelBits( iX, ya );
    uint32 bpp = iDst->BytesPerPixel();
    uint32 bps = iDst->BytesPerScanLine();
    for( int i = ya; i <= yb; ++i ) {
        memcpy( ptr, iCorrectColor, bpp );
        ptr += bps;
    }
}

void DrawVerticalLineNoAA( FBlock* iDst, const FColor& iColor, int iY1, int iY2, int iX ) {
    FColor color( iDst->Format() );
    Conv( iColor, color );
    uint8* src = color.Bits();
    DrawVerticalLineNoAA_UnsafeColor( iDst, src, iY1, iY2, iX );
}


void DrawRectOutlineNoAA( FBlock* iDst, const FColor& iColor, const FRectI& iRect ) {
    FColor color( iDst->Format() );
    Conv( iColor, color );
    uint8* src = color.Bits();
    DrawHorizontalLineNoAA_UnsafeColor( iDst, src, iRect.x, iRect.x + iRect.w, iRect.y );
    DrawHorizontalLineNoAA_UnsafeColor( iDst, src, iRect.x, iRect.x + iRect.w, iRect.y + iRect.h );
    DrawVerticalLineNoAA_UnsafeColor( iDst, src, iRect.y, iRect.y + iRect.h, iRect.x );
    DrawVerticalLineNoAA_UnsafeColor( iDst, src, iRect.y, iRect.y + iRect.h, iRect.x + iRect.w );
}

void DrawUniformGridOutlineNoAA( FBlock* iDst, const FColor& iColor, const FRectI& iRect, int iNumSubdiv ) {
    FColor color( iDst->Format() );
    Conv( iColor, color );
    uint8* src = color.Bits();
    int stepX = iRect.w / iNumSubdiv;
    int stepY = iRect.h / iNumSubdiv;
    for( int x = iRect.x; x <= iRect.x + iRect.w; x += stepX )
        DrawVerticalLineNoAA_UnsafeColor( iDst, src, iRect.y, iRect.y + iRect.h, x );

    for( int y = iRect.y; y <= iRect.y + iRect.h; y += stepY )
        DrawHorizontalLineNoAA_UnsafeColor( iDst, src, iRect.x, iRect.x + iRect.w, y );

}

ULIS_NAMESPACE_END

