// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Test.cpp
* @author       Clement Berthaud
* @brief        Test application for ULIS3.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
using namespace ::ULIS;

int
main(int argc,char *argv[]) 
{
    //_sleep(5000);

    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_MEM );
    FHardwareMetrics hw;
    FSchedulePolicy policy_cache_efficient( ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_mono_chunk( ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );

    //Data
    FBlock canvas( 1024, 1024, fmt );

    /*int repeat = 500;
    TArray< FEvent > events( repeat );
    ctx.Fill( canvas, canvas.Rect(), FColor::RGBA8(0,0,0), policy_mono_chunk, 0, nullptr, &events[0] );
    for( int i = 1; i < repeat; ++i ) {
        ctx.Fill(canvas,canvas.Rect(), FColor::RGBA8(0,0,0), policy_mono_chunk,1,&events[i-1],&events[i]);
        ctx.Flush();
    }*/
    int i = 0;
    while( 1 )
    {
        ctx.Fill(canvas,canvas.Rect(),FColor::RGBA8(0,0,0),policy_mono_chunk);
        ctx.Finish();
        std::cout << i << std::endl;
        i++;
    }

    //ctx.Wait(events[499]);
    //_sleep( 5000 );
    //ctx.Fence();

    return  0;
}
