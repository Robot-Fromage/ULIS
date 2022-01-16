// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Device_Generic.inl
* @author       Clement Berthaud
* @brief        This file provides the definition for the FHardwareMetrics tools.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "System/CPUInfo/CPUInfo.h"
#include "System/CPUInfo/CPUInfoHelpers.h"

#pragma message( "GENERIC DEVICE ACTIVATED" )

ULIS_NAMESPACE_BEGIN
namespace detail {
void
cpuid( int32_t out[4], int32_t x )
{
    memset( out, 0, 4 * sizeof( int ) );
}

bool
detect_OS_x64() {
    return  true;
}

bool
detect_OS_AVX() {
    return  false;
}

bool
detect_OS_AVX512() {
    return  false;
}

std::string
get_vendor_string() {
    return  "Unknown";
}

void cache_info( uint8 iLevel, uint32 *oCacheSize, uint32* oLineSize ) {
    *oCacheSize = 0;
    *oLineSize = 0;
}

uint32 num_workers() {
    return  1;
}

} // namespace detail

ULIS_NAMESPACE_END

