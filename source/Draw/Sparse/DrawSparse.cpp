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

ULIS_NAMESPACE_BEGIN
/*
void DotNoAA( FBlock* iDst, const FColor& iColor, const FVec2I iPos ) {
    uint8* ptr = iDst->PixelBits( iPos.x, iPos.y );
    fpConversionInvocation fptr = QueryDispatchedConversionInvocation( iColor.Format(), iDst->Format() );
    fptr( &iColor.FormatMetrics(), iColor.Bits(), &iDst->FormatMetrics(), ptr, 1 );
}
*/

ULIS_NAMESPACE_END

