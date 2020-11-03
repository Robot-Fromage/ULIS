// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
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
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = ULIS3_PERF_SSE42;
    int size = 128;
    uint32 format = ULIS3_FORMAT_BGRA8;
    uint32 repeat = 500;
    FBlock* src = new FBlock( size, size, format );
    FBlock* dst = new FBlock( size, size, format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        Blend( pool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, src, dst, src->Rect(), FVec2F(), ULIS3_NOAA, BM_NORMAL, AM_NORMAL, 0.5f );

    //AlphaBlend( pool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, src, dst, src->Rect(), FVec2F(), ULIS3_NOAA, 0.5f );

    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = static_cast< double >( std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count() ) / static_cast< double >( repeat );
    delete src;
    delete dst;
    XDeleteThreadPool( pool );

    std::cout << deltaMs << std::endl;

    return  0;
}
