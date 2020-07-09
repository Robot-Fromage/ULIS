// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Filter.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Filter entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Filter.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN

void InvokeFilter( const size_t iLen, const FBlock* iBlock, const uint8* iPtr, const size_t iSrcBPP, std::function< void( const FBlock* iBlock, const uint8* iPtr ) > iFunc ) {
    for( size_t i = 0; i < iLen; ++i ) {
        iFunc( iBlock, iPtr );
        iPtr += iSrcBPP;
    }
}

void InvokeFilterInPlace( const size_t iLen, FBlock* iBlock, uint8* iPtr, const size_t iSrcBPP, std::function< void( FBlock* iBlock, uint8* iPtr ) > iFunc ) {
    for( size_t i = 0; i < iLen; ++i ) {
        iFunc( iBlock, iPtr );
        iPtr += iSrcBPP;
    }
}

void InvokeFilterInto( const size_t iLen, const FBlock* iSrcBlock, const uint8* iSrcPtr, const size_t iSrcBPP, FBlock* iDstBlock, uint8* iDstPtr, const size_t iDstBPP, std::function< void( const FBlock* iSrcBlock, const uint8* iSrcPtr, FBlock* iDstBlock, uint8* iDstPtr ) > iFunc ) {
    for( size_t i = 0; i < iLen; ++i ) {
        iFunc( iSrcBlock, iSrcPtr, iDstBlock, iDstPtr );
        iSrcPtr += iSrcBPP;
        iDstPtr += iDstBPP;
    }
}

void Filter( FThreadPool*           iThreadPool
           , bool                   iBlocking
           , uint32                 iPerfIntent
           , const FHostDeviceInfo& iHostDeviceInfo
           , bool                   iCallCB
           , const FBlock*          iSource
           , std::function< void( const FBlock* iBlock, const uint8* iPtr ) > iFunc )
{
    // Assertions
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iFunc,                                        "No func provided"                                      );

    // Format info
    const FFormat& srcFormatInfo( iSource->FormatInfo() );

    // Bake Params and call
    const uint8*    src     = iSource->DataPtr();
    uint32           src_bps = iSource->BytesPerScanLine();
    const int       max     = iSource->Height();
    const uint32     len     = iSource->Width();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , InvokeFilter
                                   , len, iSource, src + ( pLINE * src_bps ), srcFormatInfo.BPP, iFunc );
}

void FilterInPlace( FThreadPool*            iThreadPool
                  , bool                    iBlocking
                  , uint32                  iPerfIntent
                  , const FHostDeviceInfo&  iHostDeviceInfo
                  , bool                    iCallCB
                  , FBlock*                 iSource
                  , std::function< void( FBlock* iBlock, uint8* iPtr ) > iFunc )
{
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iFunc,                                        "No func provided"                                      );

    // Format info
    const FFormat& srcFormatInfo( iSource->FormatInfo() );

    // Bake Params and call
    uint8*      src     = iSource->DataPtr();
    uint32       src_bps = iSource->BytesPerScanLine();
    const int   max     = iSource->Height();
    const uint32 len     = iSource->Width();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , InvokeFilterInPlace
                                   , len, iSource, src + ( pLINE * src_bps ), srcFormatInfo.BPP, iFunc );

    iSource->Invalidate( iCallCB );
}

void FilterInto( FThreadPool*              iThreadPool
               , bool                      iBlocking
               , uint32                    iPerfIntent
               , const FHostDeviceInfo&    iHostDeviceInfo
               , bool                      iCallCB
               , const FBlock*             iSource
               , FBlock*                   iDestination
               , std::function< void( const FBlock* iSrcBlock, const uint8* iSrcPtr, FBlock* iDstBlock, uint8* iDstPtr ) > iFunc )
{
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS3_ASSERT( iSource != iDestination,                      "Cannot extract a block to itself, use swap instead."   );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iSource->Width()  == iDestination->Width(),   "Blocks sizes don't match"                              );
    ULIS3_ASSERT( iSource->Height() == iDestination->Height(),  "Blocks sizes don't match"                              );
    ULIS3_ASSERT( iFunc,                                        "No func provided"                                      );

    // Format info
    const FFormat& srcFormatInfo( iSource->FormatInfo() );
    const FFormat& dstFormatInfo( iDestination->FormatInfo() );

    // Bake Params and call
    const uint8*    src     = iSource->DataPtr();
    uint32           src_bps = iSource->BytesPerScanLine();
    uint8*          dst     = iDestination->DataPtr();
    uint32           dst_bps = iDestination->BytesPerScanLine();
    const int       max     = iSource->Height();
    const uint32     len     = iSource->Width();
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , InvokeFilterInto
                                   , len
                                   , iSource
                                   , src + ( pLINE * src_bps )
                                   , srcFormatInfo.BPP
                                   , iDestination
                                   , dst + ( pLINE * dst_bps )
                                   , dstFormatInfo.BPP
                                   , iFunc );
    iDestination->Invalidate( iCallCB );
}

ULIS3_NAMESPACE_END

