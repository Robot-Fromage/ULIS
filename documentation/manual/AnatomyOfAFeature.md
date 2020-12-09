# Anatomy Of A Feature
An overview of the various elements involved in setting up an Image Processing feature for ULIS.

---

## Introduction
We will take the Clear feature as an exemple to dissect the inner mechanisms used to setup a feature to work in the async pipeline with its various requirements. 
We will start from the top-level public API and slowly dive into the private implementation parts.

## API Entry-Point Definition In FContext
The first step is to implement what we call an entry-point in the FContext class, that is the primary symbol exposed to the user. It is a public method of the FContext class, and the method signature follows a simple pattern. 
Before implementing an entry-point we should ask ourselves a few questions:
    - How many blocks do i need to complete this process ?
    - Does the process operate in-place ?
    - Am-i allowed to process only a subset of the geometry or should i only process whole blocks ?
    - What other arguments do i need to complete the task ?

For a Clear operation, we need only one block, that will be processed in-place, and we can process only a sub-rectangle of the block geometry, so we should also pass a rectangle as an argument. We don't need any other arguments in order to complete the task ( Clear is one of the simplest functions available ). 
The method definition would look like this:

> "include/Context/Context.h"
```d
    void
    Clear(
          FBlock& iBlock
        , const FRectI& iRect = FRectI( 0, 0, INT_MAX, INT_MAX )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
        , FEvent* iEvent = nullptr
    );
```

The signature shows that we expect a block passed by reference. We privilege reference over pointer here because it is "safer" in the sense that it makes no sense to be able to pass a nullptr to the operation. The reference is non-const which indicates the input block can be modified. 
The signature also shows that we can pass an optional rectangle geometry to indicate what portion of the block should be cleared. If the argument is missing, it will take the largest possible geometry by default, even if it is bigger than the block geometry; these geometry will have to be sanitized in the method implementation. 
Then we see a bunch of arguments related to asynchronous mechanisms, that are common to ALL entry-points in FContext:
    - FSchedulePolicy, this is an object that allows us to hint at the asynchronous engine how we would prefere the task to be scheduled, that is if we would rather have the task scheduling Scanlines-Jobs or Chunks-Jobs, and other options that allow us to have finer-grained control over the sizes of the scheduled Jobs, in order to optimize cache efficiency in certain situations. Keep in mind these are just hints and the async engine can chose to override these options, because some taks explicitely disallow Scanlines or Chunks because of implementation constraints, but this is unknown to the user. There are no such constraints for a Clear operation however as we will see. This argument is optional and if missing will default to a policy that is still decent performance-wise, but explicit intel can usually improve it.
    - The next two arguments common to all entry-points that use the async pipeline are an uint32 number and a pointer to the first element of a contiguous array of events, that define how many events we will be waiting for in the event wait list before completing the task. It is basically an array of events with its size. We use this old-school way of passing array for flexibility and interoperability with application frameworks that may use various types for storing arrays, such as std::vector or a custom Array class. ULIS also provides a custom Array class, but we chose not to enforce its use here.
    - The last argument is an optional Event that will be associated to this operation. Here it makes sense to expect a pointer because we can pass a nullptr to indicate no event processing is needed for this task, we can authorize this if it has no race constraints with other operations.

---

## Runtime-Cached Entry Function Pointer in FContextualDispatchTable

Now for a given feature entry-point in FContext, we associate a symetrical cached function pointer in FContextualDispatchTable. This function pointer is initialized at runtime, during construction of a FContextualDispatchTable object and will be used by the implementation of the Clear method in FContext:
> "source/Context/ContextualDispatchTable.h"
```d
struct FContext::FContextualDispatchTable
{
public:
    FContextualDispatchTable( const FHardwareMetrics& iHardwareMetrics, eFormat iFormat );

private:
    const fpCommandScheduler mScheduleClear;
};
```

> "source/Context/ContextualDispatchTable.cpp"
```d
FContext::FContextualDispatchTable::FContextualDispatchTable( const FHardwareMetrics& iHardwareMetrics, eFormat iFormat )
        : mScheduleClear( TDispatcher< FDispatchedClearInvocationSchedulerSelector >::Query( iHardwareMetrics, iFormat ) )
{}
```

As we can see, the FContextualDispatchTable now has a "mScheduleClear" member of type "fpCommandScheduler", which is the common type for all features cached entry. 
The "mScheduleClear" member is const so it has to be initialized in the constructor initializer list, and it is initialized from an unusual looking statement:

```cpp
    TDispatcher< FDispatchedClearInvocationSchedulerSelector >::Query( iHardwareMetrics, iFormat )
```
This statement has a lot happening inside, and we'll go into it, but for now just know "TDispatcher" is a template mechanism functor to query and retrieve the dispatched implementation according to runtime arguments such as the hardware metrics or the format; and "FDispatchedClearInvocationSchedulerSelector" is an automatically generated structure during preprocessing that has all the information needed for the "TDispatcher" to work.

---

## API Entry-Point Implementation In FContext

Let's go back to FContext and take a look at the actual implementation of our Clear entry point:

> "source/Context/Commands/Context.Clear.cpp"
```d
void
FContext::Clear(
          FBlock& iBlock
        , const FRectI& iRect
        , const FSchedulePolicy& iPolicy
        , uint32 iNumWait
        , const FEvent* iWaitList
        , FEvent* iEvent
)
{
    // Sanitize geometry
    const FRectI src_rect = iBlock.Rect();
    const FRectI src_roi = iRect.Sanitized() & src_rect;

    // Check no-op
    if( src_roi.Area() <= 0 )
        return  FinishEventNoOP( iEvent );

    // Bake and push command
    mCommandQueue.Push(
        new FCommand(
              mContextualDispatchTable->mScheduleClear
            , new FSimpleBufferCommandArgs(
                  iBlock
                , src_roi
            )
            , iPolicy
            , src_roi == src_rect
            , iNumWait
            , iWaitList
            , iEvent
        )
    );
}
```

First we retrieve the same familiar function signature, now let us break down the various steps involved here:
    - A geometry sanitize phase.
    - An event safety phase.
    - A command building phase.

The first "geometry sanitize" phase is pretty much straight forward, we fetch the block geometry and intersect it with the input geometry, that way if the input geometry is bigger than the block, it will be clamped, and if there is no intersection at all, the resulting intersection will be an invalid rectangle that we can check.
The event safety phase checks if the obtained rectangle is valid, and if it is not, it will return because no work is needed ( a no-op ), while notifying the input iEvent that it doesn't need to wait for anything, basically completing the task immediately.
Finally we build the command, and this is a more serious part of the implementation, because many things happen at the same time:
    - We build an FCommand object, that is expecting:
        + A fpCommandScheduler function pointer, which we retrieve from the contextual dispatch table from the member "mScheduleClear"
        + A new ICommandArgs object, which we pass from a newly created FSimpleBufferCommandArgs ( all command args class should derive from ICommandArgs ), the FSimpleBufferCommandArgs is a common command args object that fits perfectly for clear because it expects the same parameters we devised earlier ( here we give him the block and the sanitized geometry ).
        + We basically forward the policy, the event wait list, and the event.
        + And there's another expression leaning against iPolicy ( "src_roi == src_rect" ) which evaluates to a bool, and that states wether the scheduling actually allows Chunks Jobs if requested. The logic behind that being that only if the whole block is to be cleared, we can expect its data to be totally contiguous and slice it into chunks to feed the threads. Otherwise, Scanlines Jobs will be preferred.
    - We push the newly created FCommand object into the Command Queue at the same time.

What happens inside an FCommand or what happends inside the FCommandQueue is not relevant for the purpose of implementing a new feature, so let's focus on that part. What is interesting to notice here is that we used the mScheduleClear member from the contextual dispatch table, and that we mention a FSimpleBufferCommandArgs object, so let us have a look at that.

---

## Command Arguments

FSimpleBufferCommandArgs, along with FSimpleBufferJobArgs, are commonly used command arguments for operations that work in-place and don't require additional arguments.

> "source/Scheduling/SimpleBufferArgs.h"
```cpp
class FSimpleBufferCommandArgs
    : public ICommandArgs
{
public:
    virtual ~FSimpleBufferCommandArgs() override {}
    FSimpleBufferCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
    )
        : ICommandArgs()
        , block( iBlock )
        , rect( iRect )
    {}

    FBlock& block;
    const FRectI rect;
};
```

Some features will require you to implement a custom Args object for this command, but most will be simple enough to only require FSimpleBufferCommandArgs, or its counterpart FDualBufferCommandArgs for operations that don't work in-place.

---

## Feature Implementation

Now everything in the top-level API is setup, but we still haven't implemented anything in terms of actual feature work yet, and we still need to understand about "FDispatchedClearInvocationSchedulerSelector". So let's go to "source/Clear/" and see what we have here. We only see two files, Clear.h and Clear.cpp, which are pretty short, and will contain everything we need to implement the actual feature. 

The "Clear.h" file is 34 lines short and all we need to look at is this:
> "source/Clear/Clear.h"
```cpp
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleClearMT_AVX );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleClearMT_SSE );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleClearMT_MEM );
ULIS_DECLARE_DISPATCHER( FDispatchedClearInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP(
      FDispatchedClearInvocationSchedulerSelector
    , ScheduleClearMT_AVX
    , ScheduleClearMT_SSE
    , ScheduleClearMT_MEM
)
```

Lower-level implementations rely on recurring macros to avoid redundant work, and while macros are often frowned upon for messing with the reading flow, they work here in a language extension paradigm, allowing to automatically generate complex structures automatically during preprocessing, you just need to be familiar with their interface. 
First, whe have three "ULIS_DECLARE_COMMAND_SCHEDULER", declaring three low-level entries for ScheduleClear. The "mScheduleClear" member in FContextualDispatchTable will point to one of these versions ( AVX, SSE or MEM ). Their purpose is to be called immediately when building an FCommand object, passed as an fpCommandScheduler function pointer, and their role is to build a bunch of jobs according to the policy and the chucks constraints of the invocations. 

Now we can finally have a look a the Dispatcher mechanism, and at "FDispatchedClearInvocationSchedulerSelector". With a single macro "ULIS_DEFINE_DISPATCHER_GENERIC_GROUP", we define a SchedulerSelector structure, that TDispatcher will be able to read and query informations from. After defining its name, we pass in the command schedulers we defined earlier in order, the AVX, SSE, and MEM version. This allows us to implement various levels of optimization for a given feature, and the dispatcher will query the most appropriate one based on the hardware metrics at runtime. Alternatively, when building on a platform that doesn't suppor SIMD, these optimizations branches will be thrown away and default to the MEM version. MEM indicates no SIMD optimisations are used. 



Now for the actual bulk of the work, let's switch to Clear.cpp. 
Let's first have a look at the end of the file: 
> "source/Clear/Clear.cpp"
```cpp
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleClearMT_AVX, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeClearMT_AVX )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleClearMT_SSE, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeClearMT_SSE )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleClearMT_MEM, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeClearMT_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedClearInvocationSchedulerSelector )
```

This is symmetrical from the header file, we just provide automatic implementation for the various ScheduleClear methods, indicate which kind of Arguments are expected, here FSimpleBufferCommandArgs as we've seen earlier, as well as FSimpleBufferJobArgs which will be expected in the invocations. Finally, we indicate which invocations these will actually schedule within jobs. 
Then we have the option to specify specializations of the the dispatcher or Scheduler Selector for a given format ( for an example with specializations for RGBA8, see Blend ), but here we don't need any. 

Now for the most interesting piece where the work actually occurs, let's have a look at the Invocations:
> "source/Clear/Clear.cpp"
```cpp
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- AVX
#ifdef ULIS_COMPILETIME_AVX_SUPPORT
static
void
InvokeClearMT_AVX(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    __m256i* ULIS_RESTRICT dst = reinterpret_cast< __m256i* >( jargs->dst );
    int64 index = 0;
    for( index = 0; index < jargs->size - 32; index += 32 ) {
        _mm256_storeu_si256( dst, _mm256_setzero_si256() );
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 256 bit with avx and perform a memset instead
    memset( dst, 0, jargs->size - index );
}
#endif // ULIS_COMPILETIME_AVX_SUPPORT

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- SSE
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
static
void
InvokeClearMT_SSE(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    __m128i* ULIS_RESTRICT dst = reinterpret_cast< __m128i* >( jargs->dst );
    int64 index = 0;
    for( index = 0; index < jargs->size - 16; index += 16 ) {
        _mm_storeu_si128( dst, _mm_setzero_si128() );
        ++dst;
    }
    // Remaining unaligned scanline end: avoid concurrent write on 128 bit with SSE and perform a memset instead
    memset( dst, 0, jargs->size - index );
}
#endif // __SE4_2__

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
static
void
InvokeClearMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    // Full scanline width instead of many BPP clears
    memset( jargs->dst, 0, jargs->size );
}
```

Only then can we do actual work on the image buffer, which we retrieve from the jargs and cargs inputs. jargs are Job Arguments, which have arguments specific to a job, such as which chunk of the buffer to process. cargs are Command Arguments that contain information common to all jobs, it is unused here due to the simplicity of the command but can be usefull at times if Jobs share a common source to read from.

## Conclusion
Finally, the implementation consists of approximately 80 lines of actual code and that's all you need for a Clear feature that will work on every block, for all formats, all colors models, all type depths, any number of channels, working asynchronously and in a multithreaded environment, with a simple API. Here is an exemple of use:
```cpp
// Clear Example
#include <ULIS>

int main() {
    // Common
    FThreadPool pool;
    FCommandQueue queue( pool );
    tFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );
    FHardwareMetrics hw;
    FSchedulePolicy cacheEfficientPolicy( ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.CacheLineSize_L1 );

    // Data
    FBlock block( 1024, 1024, fmt );

    // Operation
    FEvent evt_clear;
    ctx.Clear( block, block.Rect(), cacheEfficientPolicy, 0, nullptr, &evt_clear );
    ctx.Finish();

    return  0;
}
```