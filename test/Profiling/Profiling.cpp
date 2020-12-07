// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Benchmark.cpp
* @author       Clement Berthaud
* @brief        Benchmark application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
#include <chrono>
#include <iostream>

using namespace ::ul3;

int main( int argc, char *argv[] ) {


    FThreadPool* pool = XCreateThreadPool();
    FHardwareMetrics host = FHardwareMetrics::Detect();
    uint32 perfIntent = ULIS_PERF_SSE42;
    int size = 128;
    uint32 format = ULIS_FORMAT_BGRA8;
    uint32 repeat = 500;
    FBlock* src = new FBlock( size, size, format );
    FBlock* dst = new FBlock( size, size, format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        Blend( pool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, src, dst, src->Rect(), FVec2F(), ULIS_NOAA, BM_NORMAL, AM_NORMAL, 0.5f );

    //AlphaBlend( pool, ULIS_BLOCKING, perfIntent, host, ULIS_NOCB, src, dst, src->Rect(), FVec2F(), ULIS_NOAA, 0.5f );

    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = static_cast< double >( std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count() ) / static_cast< double >( repeat );
    delete src;
    delete dst;
    XDeleteThreadPool( pool );

    std::cout << deltaMs << std::endl;

    return  0;
}
