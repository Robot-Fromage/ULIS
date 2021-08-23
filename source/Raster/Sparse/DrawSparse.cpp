// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Draw.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the raster draw entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Raster/Draw.h"
#include "Data/Block.h"
#include "Conv/ConvBuffer.h"

ULIS2_NAMESPACE_BEGIN
/*
void DotNoAA( FBlock* iDst, const FPixelValue& iColor, const FVec2I iPos ) {
    tByte* ptr = iDst->PixelPtr( iPos.x, iPos.y );
    fpDispatchedConvInvoke fptr = QueryDispatchedConvInvokeForParameters( iColor.Format(), iDst->Format() );
    fptr( &iColor.FormatInfo(), iColor.Ptr(), &iDst->FormatInfo(), ptr, 1 );
}
*/

ULIS2_NAMESPACE_END

