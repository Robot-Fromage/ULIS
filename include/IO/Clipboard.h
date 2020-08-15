// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Clipboard.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the clipboard IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN

ULIS_API FRasterImage2D* XLoadFromClipboard( FThreadPool*              iThreadPool
                                    , bool                      iBlocking
                                    , uint32                    iPerfIntent
                                    , const FHostDeviceInfo&    iHostDeviceInfo
                                    , bool                      iCallCB
                                    , eFormat                   iDesiredFormat );

ULIS_API void SaveToClipboard( FThreadPool*            iThreadPool
                              , bool                    iBlocking
                              , uint32                  iPerfIntent
                              , const FHostDeviceInfo&  iHostDeviceInfo
                              , bool                    iCallCB
                              , const FRasterImage2D*           iSource );

ULIS_API bool ClipboardHasImageData();

ULIS_NAMESPACE_END

