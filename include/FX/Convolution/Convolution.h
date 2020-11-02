// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Convolution.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Convolution entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Convolution
ULIS_API void Convolution( FOldThreadPool*            iOldThreadPool
                          , bool                    iBlocking
                          , uint32                  iPerfIntent
                          , const FHardwareMetrics&  iHostDeviceInfo
                          , bool                    iCallCB
                          , FBlock*                 iDestination
                          , const FKernel&          iKernel
                          , const FRectI&            iArea
                          , eExtrapolationMethod    iEdgeMode );

ULIS_NAMESPACE_END

