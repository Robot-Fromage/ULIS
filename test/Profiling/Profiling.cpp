// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Profiling.cpp
* @author       Clement Berthaud
* @brief        Profiling application for ULIS.
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

    char a;
    std::cin >> a;
    //std::this_thread::sleep_for(std::chrono::seconds(5));

    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_BGRA8;
    FContext ctx( queue, fmt, PerformanceIntent_SSE);
    int size = 128;
    uint32 repeat = 360000;
    uint32 mult = 360000;
    FBlock src( size, size, fmt );
    FBlock dst( size, size, fmt );

    std::queue<std::function<void()>> funcQueue;

    FEvent eventBlend = FEvent::NoOP();

    auto startTime = std::chrono::steady_clock::now();
    for (uint32 l = 0; l < repeat / mult; ++l) {
        FSchedulePolicy policy = FSchedulePolicy::CacheEfficient;
        FEvent* subEventBlend = new FEvent[mult];

        int i = 0;
        
        //ctx.Copy(src, dst, FRectI::Auto, FVec2I(0), policy, 1, &eventBlend, &subEventBlend[i]);
        //ctx.Dummy_OP(1, &subEventBlend[i], &subEventBlend[i + 1]); i++;
        ctx.BlendAA(
            src
            , dst
            , FRectI::Auto
            , FVec2I(0)
            , Blend_Normal
            , Alpha_Normal
            , 1.0f
            , policy
            , 1
            , &eventBlend
            , &subEventBlend[0]
        );


        for( int j = 1; j < mult; j++)
        {
            //ctx.Copy(src, dst, FRectI::Auto, FVec2I(0), policy, 1, &subEventBlend[i], &subEventBlend[i + 1]); i++;
            //ctx.Dummy_OP(1, &subEventBlend[i], &subEventBlend[i + 1]); i++;
            ctx.BlendAA(
                src
                , dst
                , FRectI::Auto
                , FVec2I(0)
                , Blend_Normal
                , Alpha_Normal
                , 1.0f
                , policy
                , 1
                , &subEventBlend[j - 1]
                , &subEventBlend[j]
            );
        }
        eventBlend = subEventBlend[mult - 1];
        delete[] subEventBlend;
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
