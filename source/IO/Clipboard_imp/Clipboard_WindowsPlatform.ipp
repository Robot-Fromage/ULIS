// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Clipboard_Windows.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for the clipboard IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "IO/Clipboard.h"
#include "Image/Block.h"
#include "System/Device.h"
#include "Conv/Conv.h"
#include "Conv/ConvertFormatInvocations.h"
#include "Thread/ThreadPool.h"

#include <array>
#include <algorithm>

#include <Windows.h>

#include <clip.h>

ULIS_NAMESPACE_BEGIN
FBlock* XLoadFromClipboard( FOldThreadPool*            iOldThreadPool
                          , bool                    iBlocking
                          , uint32                  iPerfIntent
                          , const FHardwareMetrics&  iHostDeviceInfo
                          , bool                    iCallCB
                          , eFormat                 iDesiredFormat )
{
    ULIS_ASSERT( iOldThreadPool, "Bad pool." );

    if( !ClipboardHasImageData() )
        return  nullptr;

    clip::image_spec spec;
    clip::get_image_spec( spec );

    bool locked = OpenClipboard( nullptr );
    if( !locked ) return  nullptr;
    BITMAPINFO* bi = (BITMAPINFO*)GetClipboardData( CF_DIB );
    if(!bi) return  nullptr;

    int w = bi->bmiHeader.biWidth;
    int h = bi->bmiHeader.biHeight;
    int bits_per_pixel = bi->bmiHeader.biBitCount;
    if( w <= 0 || h <= 0 )                              return  nullptr;
    if( bits_per_pixel != 24 && bits_per_pixel != 32 )  return  nullptr;
    if( bi->bmiHeader.biCompression != BI_RGB && bi->bmiHeader.biCompression != BI_BITFIELDS )  return  nullptr;

    eFormat srcFormat = static_cast< eFormat >( 0 );
    if( bits_per_pixel == 24 ) srcFormat = eFormat::Format_BGR8;
    if( bits_per_pixel == 32 ) srcFormat = eFormat::Format_BGRA8;

    uint8* src = ( ((uint8*)bi) + bi->bmiHeader.biSize );
    eFormat dstFormat = iDesiredFormat;
    if( dstFormat <= 0 ) dstFormat = srcFormat;

    FBlock tmp( src, w, h, srcFormat );
    FBlock* ret = new FBlock( w, h, dstFormat );

    // Assertions
    fpConvertFormat fptr = QueryDispatchedConvertFormatInvocation( srcFormat, dstFormat );
    ULIS_ASSERT( fptr, "No Conversion invocation found" );

    // Bake Params
    int             dc          = bits_per_pixel == 24 ? 3 & w : 0;
    uint32           src_bps     = ( bits_per_pixel / 8 ) * tmp.Width() + dc;
    uint32           dst_bps     = ret->BytesPerScanLine();
    const int       numLines    = h;
    const uint32     len         = w;
    const uint8*    srb         = tmp.Bits() + src_bps * ( numLines - 1 );
    uint8*          dsb         = ret->Bits();
    #define SRC srb - ( pLINE * src_bps )
    #define DST dsb + ( pLINE * dst_bps )
    const FFormatMetrics& srcnfo = tmp.FormatMetrics();
    const FFormatMetrics& dstnfo = ret->FormatMetrics();

    // Call
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, ULIS_BLOCKING
                                   , numLines
                                   , fptr, srcnfo, SRC, dstnfo, DST, len );

    CloseClipboard();
    return  ret;
}

void SaveToClipboard( FOldThreadPool*              iOldThreadPool
                    , bool                      iBlocking
                    , uint32                    iPerfIntent
                    , const FHardwareMetrics&    iHostDeviceInfo
                    , bool                      iCallCB
                    , const FBlock*             iSource )
{
    // Assertions
    ULIS_ASSERT( iSource,                  "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,              "Bad pool."                                             );

    FBlock* tmpConv = XConv( iOldThreadPool, ULIS_BLOCKING, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, eFormat::Format_BGRA8 );

    clip::image_spec spec;
    spec.width = tmpConv->Width();
    spec.height = tmpConv->Height();
    spec.bits_per_pixel = tmpConv->BytesPerPixel() * 8;
    spec.bytes_per_row = tmpConv->BytesPerScanLine();
    spec.red_mask = 0xff0000;
    spec.green_mask = 0xff00;
    spec.blue_mask = 0xff;
    spec.alpha_mask = 0xff000000;
    spec.red_shift = 16;
    spec.green_shift = 8;
    spec.blue_shift = 0;
    spec.alpha_shift = 24;

  clip::image img( tmpConv->Bits(), spec );
  clip::set_image(img);
}

bool ClipboardHasImageData() {
    bool locked = OpenClipboard( nullptr );
    bool hasImageData = locked && IsClipboardFormatAvailable(CF_DIB);
    CloseClipboard();
    return  hasImageData;
}

ULIS_NAMESPACE_END

