// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Filter.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Filter entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <functional>

ULIS_NAMESPACE_BEGIN

typedef void (*fpFilterOPFunc)( const FBlock* iBlock, const uint8* iPtr );
typedef void (*fpFilterOPInPlaceFunc)( FBlock* iBlock, uint8* iPtr );
typedef void (*fpFilterOPInto)( const FBlock* iSrcBlock, const uint8* iSrcPtr, FBlock* iDstBlock, uint8* iDstPtr );

ULIS_API void Filter( FOldThreadPool*             iOldThreadPool
                     , bool                     iBlocking
                     , uint32                   iPerfIntent
                     , const FHardwareMetrics&   iHostDeviceInfo
                     , bool                     iCallCB
                     , const FBlock*            iSource
                     , std::function< void( const FBlock* iBlock, const uint8* iPtr ) > iFunc );

ULIS_API void FilterInPlace( FOldThreadPool*              iOldThreadPool
                            , bool                      iBlocking
                            , uint32                    iPerfIntent
                            , const FHardwareMetrics&    iHostDeviceInfo
                            , bool                      iCallCB
                            , FBlock*                   iSource
                            , std::function< void( FBlock* iBlock, uint8* iPtr ) > iFunc );

ULIS_API void FilterInto( FOldThreadPool*             iOldThreadPool
                         , bool                     iBlocking
                         , uint32                   iPerfIntent
                         , const FHardwareMetrics&   iHostDeviceInfo
                         , bool                     iCallCB
                         , const FBlock*            iSource
                         , FBlock*                  iDestination
                         , std::function< void( const FBlock* iSrcBlock, const uint8* iSrcPtr, FBlock* iDstBlock, uint8* iDstPtr ) > iFunc );
ULIS_NAMESPACE_END

