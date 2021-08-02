## Usage
ULIS can be used in a variety of ways, ranging from very easy to quite difficult to grasp when performance matters and asynchronous operations and multithreading are introduced.  
Here is a sample of asynchronous multithreaded operations when performance matters:
```cpp
#include <ULIS>
using namespace ::ULIS;

void OnEventCompleteCallDirty( const FRectI& iGeometry, void* iUserData )
{
    reinterpret_cast< FBlock* >( iUserData )->Dirty( iGeometry );
}

int
main( int argc, char *argv[] ) {
    // Common
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    FHardwareMetrics hw;
    FSchedulePolicy policy_cache_efficient( ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policy_mono_chunk( ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );

    // Data
    FBlock blockA( 1024, 1024, fmt );
    FBlock blockB( 1024, 1024, fmt );
    FBlock blockC( 1024, 1024, fmt );
    FBlock canvas( 1024, 1024, fmt );

    // Operations
    {
        TArray< FEvent > events( 12 );
        TArray< FColor > colors( 4 );
        colors[0] = FColor::RGB( 255, 0, 0 );
        colors[1] = FColor::RGB( 0, 255, 0 );
        colors[2] = FColor::GreyAF( 0.5f );
        colors[3] = FColor::GreyAF( 0.f );
        ctx.Clear( blockA, blockA.Rect(), policy_mono_chunk, 0, nullptr, &events[0] );
        ctx.Clear( blockB, blockB.Rect(), policy_mono_chunk, 0, nullptr, &events[1] );
        ctx.Clear( blockC, blockC.Rect(), policy_mono_chunk, 0, nullptr, &events[2] );
        ctx.Fill( blockA, blockA.Rect(), colors[0], policy_mono_chunk, 1, &events[0], &events[3] );
        ctx.Fill( blockB, blockB.Rect(), colors[1], policy_mono_chunk, 1, &events[1], &events[4] );
        ctx.Fill( blockC, blockC.Rect(), colors[2], policy_mono_chunk, 1, &events[2], &events[5] );
        ctx.Blend( blockC, canvas, blockC.Rect(), FVec2I( 0, 0 ), Blend_Normal, Alpha_Normal, 1.f, policy_cache_efficient, 3, &events[3], &events[6] );
        ctx.Blend( blockA, canvas, blockA.Rect(), FVec2I( 0, 0 ), Blend_Normal, Alpha_Normal, 0.5f, policy_cache_efficient, 1, &events[6], &events[7] );
        ctx.Blend( blockB, canvas, blockB.Rect(), FVec2I( 0, 0 ), Blend_Normal, Alpha_Normal, 0.1f, policy_cache_efficient, 1, &events[7], &events[8] );
        ctx.BlendColor( colors[3], canvas, canvas.Rect(), Blend_BayerDither8x8, Alpha_Normal, 0.5f, policy_cache_efficient, 1, &events[8], &events[9] );
        ctx.BlendAA( blockB, canvas, FRectI( 0, 0, 512, 512 ), FVec2F( 64.5f, 64.5f ), Blend_Normal, Alpha_Normal, 1.f, policy_cache_efficient, 1, &events[9], &events[10] );
        ctx.BlendTiled( blockA, canvas, FRectI( 16, 16, 32, 32 ), FRectI( 512, 512, 128, 128 ), FVec2I(), Blend_Normal, Alpha_Normal, 1.f, policy_cache_efficient, 1, &events[10 ], &events[11] );
        ctx.Finish();
    }

    return  0;
}
```

Be careful of a few things when using async systems:  
Policies
![](documentation/manual/resource/image/async/policy.png)

Stalling
![](documentation/manual/resource/image/async/stalling.png)
