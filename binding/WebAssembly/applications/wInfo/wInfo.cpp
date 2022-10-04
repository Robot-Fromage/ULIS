// IDDN.FR.001.250001.006.S.P.2019.000.00000
/**
*
*   ULIS
*__________________
*
* @file         wInfo.cpp
* @author       Clement Berthaud
* @brief        Formats application for wasm ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>

int main() {
    ::ULIS::FHardwareMetrics hw;
    const char* out[] = { "NO", "YES" };
    std::cout << "Vendor_AMD        : " << out[ hw.IsHardwareAMD()          ] << std::endl;
    std::cout << "Vendor_Intel      : " << out[ hw.IsHardwareIntel()        ] << std::endl;
    std::cout << "OS_x64            : " << out[ hw.IsOSx64()                ] << std::endl;
    std::cout << "OS_AVX            : " << out[ hw.HasOSAVX()               ] << std::endl;
    std::cout << "OS_AVX512         : " << out[ hw.HasOSAVX512()            ] << std::endl;
    std::cout << "HW_MMX            : " << out[ hw.HasHardwarex64()         ] << std::endl;
    std::cout << "HW_x64            : " << out[ hw.HasHardwareMMX()         ] << std::endl;
    std::cout << "HW_ABM            : " << out[ hw.HasHardwareABM()         ] << std::endl;
    std::cout << "HW_RDRAND         : " << out[ hw.HasHardwareRDRAND()      ] << std::endl;
    std::cout << "HW_BMI1           : " << out[ hw.HasHardwareBMI1()        ] << std::endl;
    std::cout << "HW_BMI2           : " << out[ hw.HasHardwareBMI2()        ] << std::endl;
    std::cout << "HW_ADX            : " << out[ hw.HasHardwareADX()         ] << std::endl;
    std::cout << "HW_PREFETCHWT1    : " << out[ hw.HasHardwarePREFETCHWT1() ] << std::endl;
    std::cout << "HW_MPX            : " << out[ hw.HasHardwareMPX()         ] << std::endl;
    std::cout << "HW_SSE            : " << out[ hw.HasHardwareSSE()         ] << std::endl;
    std::cout << "HW_SSE2           : " << out[ hw.HasHardwareSSE2()        ] << std::endl;
    std::cout << "HW_SSE3           : " << out[ hw.HasHardwareSSE3()        ] << std::endl;
    std::cout << "HW_SSSE3          : " << out[ hw.HasHardwareSSSE3()       ] << std::endl;
    std::cout << "HW_SSE41          : " << out[ hw.HasHardwareSSE41()       ] << std::endl;
    std::cout << "HW_SSE42          : " << out[ hw.HasHardwareSSE42()       ] << std::endl;
    std::cout << "HW_SSE4a          : " << out[ hw.HasHardwareSSE4a()       ] << std::endl;
    std::cout << "HW_AES            : " << out[ hw.HasHardwareAES()         ] << std::endl;
    std::cout << "HW_SHA            : " << out[ hw.HasHardwareSHA()         ] << std::endl;
    std::cout << "HW_AVX            : " << out[ hw.HasHardwareAVX()         ] << std::endl;
    std::cout << "HW_XOP            : " << out[ hw.HasHardwareXOP()         ] << std::endl;
    std::cout << "HW_FMA3           : " << out[ hw.HasHardwareFMA3()        ] << std::endl;
    std::cout << "HW_FMA4           : " << out[ hw.HasHardwareFMA4()        ] << std::endl;
    std::cout << "HW_AVX2           : " << out[ hw.HasHardwareAVX2()        ] << std::endl;
    std::cout << "HW_AVX512_F       : " << out[ hw.HasHardwareAVX512_F()    ] << std::endl;
    std::cout << "HW_AVX512_PF      : " << out[ hw.HasHardwareAVX512_PF()   ] << std::endl;
    std::cout << "HW_AVX512_ER      : " << out[ hw.HasHardwareAVX512_ER()   ] << std::endl;
    std::cout << "HW_AVX512_CD      : " << out[ hw.HasHardwareAVX512_CD()   ] << std::endl;
    std::cout << "HW_AVX512_VL      : " << out[ hw.HasHardwareAVX512_VL()   ] << std::endl;
    std::cout << "HW_AVX512_BW      : " << out[ hw.HasHardwareAVX512_BW()   ] << std::endl;
    std::cout << "HW_AVX512_DQ      : " << out[ hw.HasHardwareAVX512_DQ()   ] << std::endl;
    std::cout << "HW_AVX512_IFMA    : " << out[ hw.HasHardwareAVX512_IFMA() ] << std::endl;
    std::cout << "HW_AVX512_VBMI    : " << out[ hw.HasHardwareAVX512_VBMI() ] << std::endl;
    std::cout << "MAX WORKERS       : " << hw.MaxWorkers()      << std::endl;
    std::cout << "L1 CACHE          : " << hw.L1CacheSize()     << std::endl;
    std::cout << "L1 CACHE LINE     : " << hw.L1CacheLineSize() << std::endl;
    std::cout << hw.Field() << std::endl;
    std::cout << std::endl << ::ULIS::FullLibraryInformationString().Data() << std::endl;
    return  0;
}

