// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MemoryInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FMemoryInfo tools.
* @license      Please refer to LICENSE.md
*/
#include "System/MemoryInfo/MemoryInfo.h"
#include "String/String.h"

// Most of the code displayed in the inl files is from here:
// https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
#if defined( ULIS_WIN )
#include "System/MemoryInfo/MemoryInfo_Windows.inl"
#elif defined( ULIS_MACOS )
#include "System/MemoryInfo/MemoryInfo_macOS.inl"
#elif defined( ULIS_LINUX )
#include "System/MemoryInfo/MemoryInfo_Linux.inl"
#else
#include "System/MemoryInfo/MemoryInfo_Generic.inl"
#endif

#if ( defined( ULIS_GCC ) || defined( ULIS_MINGW ) ) && __GNUC__ < 8
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

ULIS_NAMESPACE_BEGIN
//static
uint64
FMemoryInfo::TotalVirtualMemory()
{
    return  detail::TotalVirtualMemory();
}

//static
uint64
FMemoryInfo::VirtualMemoryCurrentlyUsed()
{
    return  detail::VirtualMemoryCurrentlyUsed();
}

//static
uint64
FMemoryInfo::VirtualMemoryCurrentlyUsedByProcess()
{
    return  detail::VirtualMemoryCurrentlyUsedByProcess();
}

//static
uint64
FMemoryInfo::TotalRAM()
{
    return  detail::TotalRAM();
}

//static
uint64
FMemoryInfo::TotalRAMCurrentlyUsed()
{
    return  detail::TotalRAMCurrentlyUsed();
}

//static
uint64
FMemoryInfo::TotalRAMCurrentlyUsedByProcess()
{
    return  detail::TotalRAMCurrentlyUsedByProcess();
}

//static
udouble
FMemoryInfo::CPUCurrentlyUsed()
{
    return  detail::CPUCurrentlyUsed();
}

//static
udouble
FMemoryInfo::CPUCurrentlyUsedByProcess()
{
    return  detail::CPUCurrentlyUsedByProcess();
}

//static
uint64
FMemoryInfo::DiskCapacity( const FString& iDisk )
{
    std::error_code ec;
    const std::filesystem::space_info si = std::filesystem::space( iDisk.Data(), ec );
    return  si.capacity;
}

//static
uint64
FMemoryInfo::DiskSpaceAvailable( const FString& iDisk )
{
    std::error_code ec;
    const std::filesystem::space_info si = std::filesystem::space( iDisk.Data(), ec );
    return  si.available;
}

//static
uint64
FMemoryInfo::DiskSpaceFree( const FString& iDisk )
{
    std::error_code ec;
    const std::filesystem::space_info si = std::filesystem::space( iDisk.Data(), ec );
    return  si.free;
}

ULIS_NAMESPACE_END

