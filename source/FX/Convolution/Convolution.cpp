// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Convolution.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Convolution entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "FX/Convolution/Convolution.h"
#include "FX/Convolution/KernelSSE.h"
#include "Base/HostDeviceInfo.h"
#include "Conv/Conv.h"
#include "Data/Block.h"
#include "Data/Pixel.h"
#include "Data/Kernel.h"
#include "Maths/Geometry.h"
#include "Thread/ThreadPool.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Convolution
void
Convolution( FThreadPool*            iThreadPool
           , bool                    iBlocking
           , uint32                  iPerfIntent
           , const FHostDeviceInfo&  iHostDeviceInfo
           , bool                    iCallCB
           , FBlock*                 iDestination
           , const FKernel&          iKernel
           , const FRect&            iArea
           , eExtrapolationMethod    iEdgeMode )
{
    // Assertions
    ULIS3_ASSERT( iDestination,             "Bad source."                                           );
    ULIS3_ASSERT( iThreadPool,              "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    // Fit region of interest
    FRect roi = iArea & iDestination->Rect();

    // Check no-op
    if( roi.Area() <= 0 )
        return;

    // Invalid
    iDestination->Invalidate( roi, iCallCB );
}

ULIS3_NAMESPACE_END

