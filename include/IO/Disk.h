// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Disk.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the disk IO entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS2_NAMESPACE_BEGIN

enum eImageFormat {
      IM_PNG
    , IM_BMP
    , IM_TGA
    , IM_JPG
    , IM_HDR
};

static const char* kwImageFormat[] = {
      "png"
    , "bmp"
    , "tga"
    , "jpg"
    , "hdr"
};

ULIS2_API FBlock* XLoadFromFile( FThreadPool*           iThreadPool
                               , bool                   iBlocking
                               , uint32                 iPerfIntent
                               , const FHostDeviceInfo& iHostDeviceInfo
                               , bool                   iCallCB
                               , const std::string&     iPath
                               , tFormat                iDesiredFormat );

ULIS2_API void SaveToFile( FThreadPool*             iThreadPool
                         , bool                     iBlocking
                         , uint32                   iPerfIntent
                         , const FHostDeviceInfo&   iHostDeviceInfo
                         , bool                     iCallCB
                         , const FBlock*            iSource
                         , const std::string&       iPath
                         , eImageFormat             iImageFormat
                         , int                      iQuality );

ULIS2_NAMESPACE_END

