// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Disk.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the disk IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN

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

ULIS_API FBlock* XLoadFromFile( FOldThreadPool*           iOldThreadPool
                               , bool                   iBlocking
                               , uint32                 iPerfIntent
                               , const FHostDeviceInfo& iHostDeviceInfo
                               , bool                   iCallCB
                               , const std::string&     iPath
                               , eFormat                iDesiredFormat );

ULIS_API void SaveToFile( FOldThreadPool*             iOldThreadPool
                         , bool                     iBlocking
                         , uint32                   iPerfIntent
                         , const FHostDeviceInfo&   iHostDeviceInfo
                         , bool                     iCallCB
                         , const FBlock*            iSource
                         , const std::string&       iPath
                         , eImageFormat             iImageFormat
                         , int                      iQuality );

ULIS_NAMESPACE_END

