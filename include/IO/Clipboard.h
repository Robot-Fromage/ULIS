// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Clipboard.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the clipboard IO entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS2_NAMESPACE_BEGIN

ULIS2_API FBlock* XLoadFromClipboard( FThreadPool*              iThreadPool
                                    , bool                      iBlocking
                                    , uint32                    iPerfIntent
                                    , const FHostDeviceInfo&    iHostDeviceInfo
                                    , bool                      iCallCB
                                    , tFormat                   iDesiredFormat );

ULIS2_API void SaveToClipboard( FThreadPool*            iThreadPool
                              , bool                    iBlocking
                              , uint32                  iPerfIntent
                              , const FHostDeviceInfo&  iHostDeviceInfo
                              , bool                    iCallCB
                              , const FBlock*           iSource );

ULIS2_API bool ClipboardHasImageData();

ULIS2_NAMESPACE_END

