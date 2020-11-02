// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Copy.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Copy entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN

ULIS_API void Copy( FOldThreadPool*           iOldThreadPool
                   , bool                   iBlocking
                   , uint32                 iPerfIntent
                   , const FHostDeviceInfo& iHostDeviceInfo
                   , bool                   iCallCB
                   , const FBlock*          iSource
                   , FBlock*                iDestination
                   , const FRectI&           iArea
                   , const FVec2I&          iPos );

ULIS_API FBlock* XCopy( FOldThreadPool*           iOldThreadPool
                       , bool                   iBlocking
                       , uint32                 iPerfIntent
                       , const FHostDeviceInfo& iHostDeviceInfo
                       , bool                   iCallCB
                       , const FBlock*          iSource
                       , const FRectI&           iArea );

ULIS_API void CopyRaw( const FBlock* iSrc, FBlock* iDst, bool iCallCB );
ULIS_API FBlock* XCopyRaw( const FBlock* iSrc, bool iCallCB );

ULIS_NAMESPACE_END

