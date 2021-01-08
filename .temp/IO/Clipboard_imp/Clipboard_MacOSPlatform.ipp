// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Clipboard_MacOSPlatform.ipp
* @author       Clement Berthaud
* @brief        This file provides the definition for the clipboard IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "IO/Clipboard.h"

ULIS_NAMESPACE_BEGIN
FBlock* XLoadFromClipboard( FOldThreadPool*            iOldThreadPool
                          , bool                    iBlocking
                          , uint32                  iPerfIntent
                          , const FHardwareMetrics&  iHostDeviceInfo
                          , bool                    iCallCB
                          , eFormat                 iDesiredFormat )
{
    return  nullptr;
}

void SaveToClipboard( FOldThreadPool*              iOldThreadPool
                    , bool                      iBlocking
                    , uint32                    iPerfIntent
                    , const FHardwareMetrics&    iHostDeviceInfo
                    , bool                      iCallCB
                    , const FBlock*             iSource )
{
    return  void();
}

bool ClipboardHasImageData_MacOSPlatform() {
    return  false;
}

ULIS_NAMESPACE_END

