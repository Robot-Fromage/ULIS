// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Profiling.cpp
* @author       Clement Berthaud
* @brief        Profiling application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <chrono>
#include <iostream>
#include <queue>
#include <thread>

using namespace ::ULIS;

int main( int argc, char *argv[] )
{
    //std::this_thread::sleep_for(std::chrono::seconds(5));

    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_BGRA8;
    FContext ctx( queue, fmt, PerformanceIntent_SSE );
    int size = 128;
    uint32 repeat = 36000;
    FBlock src( size, size, fmt );
    FBlock dst( size, size, fmt );

    std::queue<std::function<void()>> funcQueue;

    char a;
    std::cin >> a;

    FEvent eventBlend = FEvent::NoOP();

    auto startTime = std::chrono::steady_clock::now();
    for (uint32 l = 0; l < repeat; ++l) {
        FSchedulePolicy policy = FSchedulePolicy::AsyncCacheEfficient;
        FEvent subEventBlend [10];
        ctx.Copy(src, dst, FRectI::Auto, FVec2I(0), policy, 1, &eventBlend, &subEventBlend[0]);
        for (int i = 1; i < 10; i++)
        {
            ctx.Copy(src, dst, FRectI::Auto, FVec2I(0), policy, 1, &subEventBlend[i-1], &subEventBlend[i]);
        }
        eventBlend = subEventBlend[9];
        ctx.Flush();
    }

    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << deltaMs << std::endl;

    ctx.Finish();

    endTime = std::chrono::steady_clock::now();
    deltaMs = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << deltaMs << std::endl;
    return  0;
}
