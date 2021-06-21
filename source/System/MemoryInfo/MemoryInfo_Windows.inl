// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MemoryInfo_Windows.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FMemoryInfo tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/MemoryInfo/MemoryInfo.h"

#include <windows.h>
#include <psapi.h>
#include <TCHAR.h>
#include <pdh.h>

#pragma comment(lib, "pdh.lib")

ULIS_NAMESPACE_BEGIN
namespace details {

static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

uint64 TotalVirtualMemory() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof( MEMORYSTATUSEX );
    GlobalMemoryStatusEx( &memInfo );
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
    return  static_cast< uint64 >( totalVirtualMem );
}

uint64 VirtualMemoryCurrentlyUsed() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof( MEMORYSTATUSEX );
    GlobalMemoryStatusEx( &memInfo );
    DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
    return  static_cast< uint64 >( virtualMemUsed );
}

uint64 VirtualMemoryCurrentlyUsedByProcess() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo( GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof( pmc ) );
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    return  static_cast< uint64 >( virtualMemUsedByMe );
}

uint64 TotalRAM() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof( MEMORYSTATUSEX );
    GlobalMemoryStatusEx( &memInfo );
    DWORDLONG totalPhysMem = memInfo.ullTotalPhys;
    return  static_cast< uint64 >( totalPhysMem );
}

uint64 TotalRAMCurrentlyUsed() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof( MEMORYSTATUSEX );
    GlobalMemoryStatusEx( &memInfo );
    DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    return  static_cast< uint64 >( physMemUsed );
}

uint64 TotalRAMCurrentlyUsedByProcess() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo( GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof( pmc ) );
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
    return  static_cast< uint64 >( physMemUsedByMe );
}

int init_CPUCurrentlyUsed() {
    PdhOpenQuery(NULL, NULL, &cpuQuery);
    // You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
    PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
    return  0;
}

double CPUCurrentlyUsed(){
    static const int dummy = init_CPUCurrentlyUsed();
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;
}

int init_CPUCurrentlyUsedByProcess(){
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
    return  0;
}

double CPUCurrentlyUsedByProcess(){
    static const int dummy = init_CPUCurrentlyUsedByProcess();

    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = static_cast< double >( ( sys.QuadPart - lastSysCPU.QuadPart ) + ( user.QuadPart - lastUserCPU.QuadPart ) );
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent;
}

} // namespace details

ULIS_NAMESPACE_END

