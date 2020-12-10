// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Extract.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Swap entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN

ULIS_API void Extract( FOldThreadPool*            iOldThreadPool
                      , bool                    iBlocking
                      , uint32                  iPerfIntent
                      , const FHardwareMetrics&  iHostDeviceInfo
                      , bool                    iCallCB
                      , const FBlock*           iSource
                      , bool                    iSourceRawIndicesFlag
                      , uint8                   iSourceExtractMask
                      , FBlock*                 iDestination
                      , bool                    iDestinationRawIndicesFlag
                      , uint8                   iDestinationExtractMask );

ULIS_API FBlock* XExtract( FOldThreadPool*            iOldThreadPool
                          , bool                    iBlocking
                          , uint32                  iPerfIntent
                          , const FHardwareMetrics&  iHostDeviceInfo
                          , bool                    iCallCB
                          , const FBlock*           iSource
                          , bool                    iSourceRawIndicesFlag
                          , uint8                   iSourceExtractMask
                          , eFormat                 iDestinationFormat
                          , bool                    iDestinationRawIndicesFlag
                          , uint8                   iDestinationExtractMask );

ULIS_NAMESPACE_END

