/*
*   kaze
*__________________
* @file         CPUInfo_Private.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FCPUInfo_Private tools.
* @copyright    Copyright 2021 Clement Berthaud.
* @license      Please refer to LICENSE.md
*/
#include "System/CPUInfo/CPUInfo_Private.h"
#include "System/CPUInfo/CPUInfoHelpers.h"

#if defined( ULIS_WIN )
#include "CPUInfo_Windows.inl"
#elif defined( ULIS_MACOS )
#include "CPUInfo_macOS.inl"
#elif defined( ULIS_LINUX )
#include "CPUInfo_Linux.inl"
#else
#include "CPUInfo_Generic.inl"
#endif

ULIS_NAMESPACE_BEGIN
FCPUInfo_Private::FCPUInfo_Private()
    : features_bitfield( 0 )
    , max_workers( static_cast< uint32 >( detail::num_workers() ) )
    , l1_cache_size( 65536 )
    , l1_cache_line_size( 64 )
{
    detail::cache_info( 1, &l1_cache_size, &l1_cache_line_size );

    features_bitfield |= ULIS_W_OS_X64( uint64( detail::detect_OS_x64() ) );
    features_bitfield |= ULIS_W_OS_AVX( uint64( detail::detect_OS_AVX() ) );
    features_bitfield |= ULIS_W_OS_AVX512( uint64( detail::detect_OS_AVX512() ) );

    std::string vendor( detail::get_vendor_string() );
    if( vendor == "GenuineIntel" )
        features_bitfield |= ULIS_R_HW_INTEL( 1 );
    else if( vendor == "AuthenticAMD")
        features_bitfield |= ULIS_R_HW_AMD( 1 );

    int info[4] { 0, 0, 0, 0 };
    detail::cpuid( info, 0 );
    int nIds = info[0];

    detail::cpuid( info, 0x80000000 );
    uint32_t nExIds = info[0];

    if( nIds >= 0x00000001 ) {
        detail::cpuid( info, 0x00000001 );
        features_bitfield |= ULIS_W_HW_MMX(    uint64( ( info[3] & ( uint64( 1 ) << 23 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_SSE(    uint64( ( info[3] & ( uint64( 1 ) << 25 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_SSE2(   uint64( ( info[3] & ( uint64( 1 ) << 26 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_SSE3(   uint64( ( info[2] & ( uint64( 1 ) << 0  ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_SSSE3(  uint64( ( info[2] & ( uint64( 1 ) << 9  ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_SSE41(  uint64( ( info[2] & ( uint64( 1 ) << 19 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_SSE42(  uint64( ( info[2] & ( uint64( 1 ) << 20 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AES(    uint64( ( info[2] & ( uint64( 1 ) << 25 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX(    uint64( ( info[2] & ( uint64( 1 ) << 28 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_FMA3(   uint64( ( info[2] & ( uint64( 1 ) << 12 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_RDRAND( uint64( ( info[2] & ( uint64( 1 ) << 30 ) ) != 0 ) );
    }

    if( nIds >= 0x00000007 ) {
        detail::cpuid( info, 0x00000007 );
        features_bitfield |= ULIS_W_HW_AVX2(           uint64( ( info[1] & ( uint64( 1 ) << 5  ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_BMI1(           uint64( ( info[1] & ( uint64( 1 ) << 3  ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_BMI2(           uint64( ( info[1] & ( uint64( 1 ) << 8  ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_ADX(            uint64( ( info[1] & ( uint64( 1 ) << 19 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_MPX(            uint64( ( info[1] & ( uint64( 1 ) << 14 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_SHA(            uint64( ( info[1] & ( uint64( 1 ) << 29 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_PREFETCHWT1(    uint64( ( info[2] & ( uint64( 1 ) << 0  ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX512_F(       uint64( ( info[1] & ( uint64( 1 ) << 16 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX512_CD(      uint64( ( info[1] & ( uint64( 1 ) << 28 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX512_PF(      uint64( ( info[1] & ( uint64( 1 ) << 26 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX512_ER(      uint64( ( info[1] & ( uint64( 1 ) << 27 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX512_VL(      uint64( ( info[1] & ( uint64( 1 ) << 31 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX512_BW(      uint64( ( info[1] & ( uint64( 1 ) << 30 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX512_DQ(      uint64( ( info[1] & ( uint64( 1 ) << 17 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX512_IFMA(    uint64( ( info[1] & ( uint64( 1 ) << 21 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_AVX512_VBMI(    uint64( ( info[2] & ( uint64( 1 ) << 1  ) ) != 0 ) );
    }

    if( nExIds >= 0x80000001 ) {
        detail::cpuid( info, 0x80000001 );
        features_bitfield |= ULIS_W_HW_X64(    uint64( ( info[3] & ( uint64( 1 ) << 29 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_ABM(    uint64( ( info[2] & ( uint64( 1 ) << 5  ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_SSE4A(  uint64( ( info[2] & ( uint64( 1 ) << 6  ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_FMA4(   uint64( ( info[2] & ( uint64( 1 ) << 16 ) ) != 0 ) );
        features_bitfield |= ULIS_W_HW_XOP(    uint64( ( info[2] & ( uint64( 1 ) << 11 ) ) != 0 ) );
    }
}

ULIS_NAMESPACE_END

