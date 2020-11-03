// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Filter.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Filter entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Filter.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"

ULIS2_NAMESPACE_BEGIN

void InvokeFilter( const size_t iLen, const FBlock* iBlock, const tByte* iPtr, const size_t iSrcBPP, std::function< void( const FBlock* iBlock, const tByte* iPtr ) > iFunc ) {
    for( size_t i = 0; i < iLen; ++i ) {
        iFunc( iBlock, iPtr );
        iPtr += iSrcBPP;
    }
}

void InvokeFilterInPlace( const size_t iLen, FBlock* iBlock, tByte* iPtr, const size_t iSrcBPP, std::function< void( FBlock* iBlock, tByte* iPtr ) > iFunc ) {
    for( size_t i = 0; i < iLen; ++i ) {
        iFunc( iBlock, iPtr );
        iPtr += iSrcBPP;
    }
}

void InvokeFilterInto( const size_t iLen, const FBlock* iSrcBlock, const tByte* iSrcPtr, const size_t iSrcBPP, FBlock* iDstBlock, tByte* iDstPtr, const size_t iDstBPP, std::function< void( const FBlock* iSrcBlock, const tByte* iSrcPtr, FBlock* iDstBlock, tByte* iDstPtr ) > iFunc ) {
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
           , std::function< void( const FBlock* iBlock, const tByte* iPtr ) > iFunc )
{
    // Assertions
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS2_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS2_ASSERT( iFunc,                                        "No func provided"                                      );

    // Format info
    const FFormatInfo& srcFormatInfo( iSource->FormatInfo() );

    // Bake Params and call
    const tByte*    src     = iSource->DataPtr();
    tSize           src_bps = iSource->BytesPerScanLine();
    const int       max     = iSource->Height();
    const tSize     len     = iSource->Width();
    ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
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
                  , std::function< void( FBlock* iBlock, tByte* iPtr ) > iFunc )
{
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS2_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS2_ASSERT( iFunc,                                        "No func provided"                                      );

    // Format info
    const FFormatInfo& srcFormatInfo( iSource->FormatInfo() );

    // Bake Params and call
    tByte*      src     = iSource->DataPtr();
    tSize       src_bps = iSource->BytesPerScanLine();
    const int   max     = iSource->Height();
    const tSize len     = iSource->Width();
    ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
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
               , std::function< void( const FBlock* iSrcBlock, const tByte* iSrcPtr, FBlock* iDstBlock, tByte* iDstPtr ) > iFunc )
{
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS2_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS2_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS2_ASSERT( iSource != iDestination,                      "Cannot extract a block to itself, use swap instead."   );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS2_ASSERT( iSource->Width()  == iDestination->Width(),   "Blocks sizes don't match"                              );
    ULIS2_ASSERT( iSource->Height() == iDestination->Height(),  "Blocks sizes don't match"                              );
    ULIS2_ASSERT( iFunc,                                        "No func provided"                                      );

    // Format info
    const FFormatInfo& srcFormatInfo( iSource->FormatInfo() );
    const FFormatInfo& dstFormatInfo( iDestination->FormatInfo() );

    // Bake Params and call
    const tByte*    src     = iSource->DataPtr();
    tSize           src_bps = iSource->BytesPerScanLine();
    tByte*          dst     = iDestination->DataPtr();
    tSize           dst_bps = iDestination->BytesPerScanLine();
    const int       max     = iSource->Height();
    const tSize     len     = iSource->Width();
    ULIS2_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
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

ULIS2_NAMESPACE_END

