// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Benchmark.cpp
* @author       Clement Berthaud
* @brief        Benchmark application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <chrono>
#include <iostream>
using namespace ::ULIS;

int main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_Max );
    FHardwareMetrics hw;
    FSchedulePolicy policy_cache_efficient( ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    int size = 128;
    uint32 repeat = 500;
    FBlock src( size, size, fmt );
    FBlock dst( size, size, fmt );
    auto startTime = std::chrono::steady_clock::now();

    for( uint32 l = 0; l < repeat; ++l ) {
        ctx.Blend( src, dst, src.Rect() );
        ctx.Finish();
    }

    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = static_cast< double >( std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count() ) / static_cast< double >( repeat );

    std::cout << deltaMs << std::endl;
    return  0;
}
