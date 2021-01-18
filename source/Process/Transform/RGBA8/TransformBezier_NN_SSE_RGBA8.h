// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformBezier_NN_SSE_RGBA8.h
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Transform/TransformArgs.h"

ULIS_NAMESPACE_BEGIN
void
InvokeTransformBezierMT_NN_SSE_RGBA8( uint8* iDst, int32 iLine, std::shared_ptr< const FTransformArgs > iInfo, std::shared_ptr< const FBlock > iField, std::shared_ptr< const FBlock > iMask );

void
TransformBezierMT_NN_SSE_RGBA8( std::shared_ptr< const FTransformArgs > iInfo, std::shared_ptr< const FBlock > iField, std::shared_ptr< const FBlock > iMask ) {
    const FTransformArgs&   info        = *iInfo;
    uint8*                  dst         = info.destination->Bits();
    const uint32             dst_bps     = info.destination->BytesPerScanLine();
    const uint32             dst_decal_y = info.dst_roi.y;
    const uint32             dst_decal_x = info.dst_roi.x * info.destination->BytesPerPixel();
    ULIS_MACRO_INLINE_PARALLEL_FOR( info.perfIntent, info.pool, info.blocking
                                   , info.dst_roi.h
                                   , InvokeTransformBezierMT_NN_SSE_RGBA8
                                   , dst + ( ( dst_decal_y + pLINE ) * dst_bps ) + dst_decal_x, pLINE, iInfo, iField, iMask );
}

ULIS_NAMESPACE_END

