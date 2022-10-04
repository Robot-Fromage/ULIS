// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private.cpp
* @author       Clement Berthaud && Eric Scholl
* @brief        This provides implementation for the ThreadPool_Private class.
* @license      Please refer to LICENSE.md
*/
#include "System/ThreadPool/ThreadPool_Private.h"

#ifdef ULIS_COMPILED_WITH_THREAD_SUPPORT
#include "System/ThreadPool/ThreadPool_Private_Multi.inl"
#else
#include "System/ThreadPool/ThreadPool_Private_Mono.inl"
#endif

