// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private.h
* @author       Clement Berthaud && Eric Scholl
* @brief        This provides definition for the ThreadPool_Private class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

#ifdef ULIS_COMPILED_WITH_THREAD_SUPPORT
#include "System/ThreadPool/ThreadPool_Private_Multi.h"
#else
#include "System/ThreadPool/ThreadPool_Private_Mono.h"
#endif

