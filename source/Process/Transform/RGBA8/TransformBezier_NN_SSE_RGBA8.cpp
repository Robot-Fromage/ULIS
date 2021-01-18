// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformBezier_NN_SSE_RGBA8.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for a Transform specialization as described in the title.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Transform/RGBA8/TransformBezier_NN_SSE_RGBA8.h"
#include "Process/Transform/TransformHelpers.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
void
InvokeTransformBezierMT_NN_SSE_RGBA8( uint8* iDst, int32 iLine, std::shared_ptr< const FTransformCommandArgs > iInfo, std::shared_ptr< const FBlock > iField, std::shared_ptr< const FBlock > iMask ) {
    const FTransformCommandArgs&   info    = *iInfo;
    const FFormatMetrics&      fmt     = info.destination->FormatMetrics();
    uint8*                  dst     = iDst;
    const float*            field   = reinterpret_cast< const float* >( iField->ScanlineBits( iLine ) );
    const uint8*            mask    = reinterpret_cast< const uint8* >( iMask->ScanlineBits( iLine ) );
    const int rangex = info.src_roi.w - 1;
    const int rangey = info.src_roi.h - 1;
    for( int x = 0; x < info.dst_roi.w; ++x ) {
        if( *mask & 0xFF ) {
            int src_x = static_cast< int >( field[0] * rangex );
            int src_y = static_cast< int >( field[1] * rangey );
            memcpy( dst, info.source->PixelBits( src_x, src_y ), fmt.BPP );
        }

        dst += fmt.BPP;
        field += 2;
        ++mask;
    }
}

ULIS_DEFINE_TRANSFORM_COMMAND_SPECIALIZATION( TransformBezierMT_NN_SSE_RGBA8 )

ULIS_NAMESPACE_END

