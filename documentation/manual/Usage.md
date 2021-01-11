## Usage
ULIS can be used in a variety of ways, ranging from very easy to quite difficult to grasp when performance matters and asynchronous operations and multithreading are introduced.  
Here is a sample of asynchronous multithreaded operations when performance matters:
```cpp
//////////////////////////////////////////////////////
// Advanced Out Of Order Multithreaded Scheduling
#include <ULIS3>

int main() {
    using namespace ::ul3;

    FThreadPool pool( FThreadPool::MaxWorkers() - 1 );
    FCommandQueue queue( thread_pool );
    tFormat documentFormat = Format_RGBA8;
    FContext ctx( documentFormat, command_queue, Ctx_PrefetchDispatch, SelectPerf_SSE );

    FHostDeviceInfo info = FHostDeviceInfo::Detect();

    FSchedulerPolicy cacheEfficientPolicy( Run_Multi, Schedule_Chunks, ItemSize_ChunkLength, info.CacheLineSize_L1 );
    FSchedulerPolicy scanLinePolicy( Run_Multi, Schedule_Scanlines )
    FSchedulerPolicy monoPolicy( Run_Mono );

    FVec2UI16 size( 1024, 1024 );
    FBlock drawing( documentFormat, size );
    FBlock overlay( documentFormat, size );
    FGPUTexture2D texture( documentFormat, size );

    FTaskEvent evt_fill_drawing = ctx.Fill( drawing, FColor::Hex( 0xF00 ), cacheEfficientPolicy );
    FTaskEvent evt_fill_overlay = ctx.Fill( overlay, FColor::Hex( 0xBA2 ), cacheEfficientPolicy, &evt_fill_drawing );

    FTaskEvent evt_blend = ctx.AlphaBlendAA( overlay, drawing, overlay.Rect(), FVec2F( 0, 0 ), Blend_Normal, 1.f, scanLinePolicy );
    ctx.Fence();

    ctx.UploadToGPU( drawing, texture, monoPolicy, &evt_blend );
    ctx.Flush();

    return  0;
}
```

Be careful of a few things when using async systems:  
Policies
![](documentation/manual/resource/image/async/policy.png)

Stalling
![](documentation/manual/resource/image/async/stalling.png)
