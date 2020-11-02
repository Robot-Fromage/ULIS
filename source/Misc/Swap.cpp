// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Swap.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Swap entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Swap.h"
#include "System/Device.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Thread/ThreadPool.h"

ULIS_NAMESPACE_BEGIN
void
InvokeSwapMTProcessScanline_MEM( uint8* iDst, uint32 iCount, uint8 iC1, uint8 iC2, uint32 iBPC, uint32 iBPP ) {
    uint8* dst = iDst;
    uint8* tmp = new uint8[iBPC];
    for( uint32 i = 0; i < iCount; ++i ) {
        memcpy( tmp, dst + iC1, iBPC );
        memcpy( dst + iC1, dst + iC2, iBPC );
        memcpy( dst + iC2, tmp, iBPC );
        dst += iBPP;
    }
    delete [] tmp;
}

void
Swap( FOldThreadPool*              iOldThreadPool
    , bool                      iBlocking
    , uint32                    iPerfIntent
    , const FHostDeviceInfo&    iHostDeviceInfo
    , bool                      iCallCB
    , FBlock*                   iDestination
    , uint8                     iChannel1
    , uint8                     iChannel2 )
{
    // Assertions
    // Assertions
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS_ASSERT( iChannel1 < iDestination->SamplesPerPixel(),  "Bad channel"                                           );
    ULIS_ASSERT( iChannel2 < iDestination->SamplesPerPixel(),  "Bad channel"                                           );
    if( iChannel1 == iChannel2 )
        return;

    FBlock* dst = iDestination;
    const uint32 bpc = dst->BytesPerSample();
    const uint32 bpp = dst->BytesPerPixel();
    const uint32 w   = dst->Width();
    const int64 bps = dst->BytesPerScanLine();
    uint8*      dsb = dst->Bits();
    #define DST dsb + ( pLINE * bps )
    const int max = dst->Height();
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , max
                                   , InvokeSwapMTProcessScanline_MEM, DST, w, iChannel1, iChannel2, bpc, bpp )

    dst->Dirty( iCallCB );
}

ULIS_NAMESPACE_END

