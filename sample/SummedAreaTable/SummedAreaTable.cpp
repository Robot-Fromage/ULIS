// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/**
*
*   ULIS
*__________________
*
* @file         SummedAreaTable.cpp
* @author       Clement Berthaud
* @brief        SummedAreaTable application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
using namespace ::ul3;

int
main() {
    FThreadPool* threadPool = XCreateThreadPool();
    uint32 perfIntent1  = 0;
    uint32 perfIntent2  = ULIS_PERF_MT;
    uint32 perfIntent3  = ULIS_PERF_SSE42 | ULIS_PERF_AVX2;
    uint32 perfIntent4  = ULIS_PERF_MT | ULIS_PERF_SSE42 | ULIS_PERF_AVX2;
    FHostDeviceInfo host = FHostDeviceInfo::Detect();

    int size = 9;
    FBlock* block = new FBlock( size, size, ULIS_FORMAT_RGBA8 );
    for( int y = 0; y < size; ++y ) {
        for( int x = 0; x < size; ++x ) {
            FPixel prox = block->Pixel( x, y );
            prox.SetR8( 1 );
            prox.SetG8( 1 );
            prox.SetB8( 1 );
            prox.SetA8( 1 );
            std::cout << "[ 1, 1, 1, 1 ], ";
        }
        std::cout << std::endl;
    }

    std::cout << "\n==\n" << std::endl;
    FBlock* sat = XGetSummedAreaTable( threadPool, ULIS_BLOCKING, perfIntent4, host, ULIS_NOCB, block );
    ULIS_ASSERT( sat, "Error " );
    ULIS_ASSERT( sat->Width() == size, "Error " );
    ULIS_ASSERT( sat->Height() == size, "Error " );

    for( int y = 0; y < size; ++y ) {
        for( int x = 0; x < size; ++x ) {
            FPixel prox = sat->Pixel( x, y );
            float R = prox.RF();
            float G = prox.GF();
            float B = prox.BF();
            float A = prox.AF();
            printf( "[ %.0f, %.0f, %.0f, %.0f ], ", R, G, B, A );
        }
        std::cout << std::endl;
    }

    XDeleteThreadPool( threadPool );
    delete  block;
    delete  sat;

    return  0;
}

