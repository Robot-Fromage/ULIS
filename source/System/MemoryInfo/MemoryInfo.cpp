// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MemoryInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FMemoryInfo tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "System/MemoryInfo/MemoryInfo.h"

#if defined( ULIS_WIN )
#include "System/MemoryInfo/MemoryInfo_Windows.inl"
#elif defined( ULIS_MACOS )
#include "System/MemoryInfo/MemoryInfo_macOS.inl"
#elif defined( ULIS_LINUX )
#include "System/MemoryInfo/MemoryInfo_Linux.inl"
#else
#include "System/MemoryInfo/MemoryInfo_Generic.inl"
#endif

ULIS_NAMESPACE_BEGIN
//static
uint64
FMemoryInfo::TotalVirtualMemoryAvailable()
{
    return  0;
}

//static
uint64
FMemoryInfo::VirtualMemoryCurrentlyUsed()
{
    return  0;
}

//static
uint64
FMemoryInfo::VirtualMemoryCurrentlyUsedByProcess()
{
    return  0;
}

//static
uint64
FMemoryInfo::TotalRAMAvailable()
{
    return  0;
}

//static
uint64
FMemoryInfo::TotalRAMCurrentlyUsed()
{
    return  0;
}

//static
uint64
FMemoryInfo::TotalRAMCurrentlyUsedByProcess()
{
    return  0;
}

//static
float
FMemoryInfo::CPUCurrentlyUsed()
{
    return  0;
}

//static
float
FMemoryInfo::CPUCurrentlyUsedByProcess()
{
    return  0;
}

ULIS_NAMESPACE_END

