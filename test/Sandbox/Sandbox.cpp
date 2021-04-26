// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Sandbox.cpp
* @author       Clement Berthaud
* @brief        Sandbox program without ULIS for quick tests.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
using namespace ::ULIS;

using pyBuffer_Event = TArray< FEvent >;

/////////
// Context Utils
template< typename T >
auto ctxTypeAdapter( T v ) {
    return  v;
}

template<>
auto ctxTypeAdapter< std::wstring >( std::wstring v ) {
    return  FWString( v.c_str() );
}

template< typename ... Ts, typename F >
auto ctxCallAdapter( F fptr ) {
    return  [fptr]( Ts ... args, pyBuffer_Event& iWaitList, FEvent* iEvent ) {
        fptr( ctxTypeAdapter( args ) ..., iWaitList.Size(), iWaitList.Data(), iEvent );
    };
}

// Just a dummy command
int test_command(
      const FBlock* iSource
    , FBlock* iDestination
    , const FWString& iStr
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    auto dummy = 0;
    return  0;
}

// Invocation of command
void test_adapter() {
    pyBuffer_Event arr( 5 );
    ctxCallAdapter< const FBlock*, FBlock*, const std::wstring& >( &test_command )( nullptr, nullptr, L"test", arr, nullptr );
}

int
main() {

    test_adapter();

    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_MEM );

    FBlock canvas( 800, 600, fmt );
    FBlock disk( 40, 40, fmt );

    ctx.Fill( canvas, FColor::Red, canvas.Rect(), FSchedulePolicy::CacheEfficient );
    ctx.Clear( disk, disk.Rect(), FSchedulePolicy::CacheEfficient );
    ctx.Finish();

    ctx.DrawCircleBresenham( disk, disk.Rect().Size() / 2, disk.Width() / 2 - 2, FColor::Blue, true, disk.Rect(), FSchedulePolicy::MonoChunk );
    ctx.Finish();

    ctx.Blend( disk, canvas, disk.Rect(), FVec2F( canvas.Rect().Size() / 2 - disk.Rect().Size() / 2 ) + FVec2F( 0.01f ), Blend_Normal, Alpha_Normal, 1.f, FSchedulePolicy::MultiScanlines );
    ctx.Finish();

    ctx.BlendAA( disk, canvas, disk.Rect(), FVec2F( canvas.Rect().Size() / 2 - disk.Rect().Size() / 2 ) + FVec2F( 10.5f ), Blend_Normal, Alpha_Normal, 1.f, FSchedulePolicy::MonoScanlines );
    ctx.Finish();

    ctx.SaveBlockToDisk( canvas, "C:/Users/PRAXINOS/Documents/work/Output/out.png", FileFormat_png, 100, FSchedulePolicy::MonoChunk );
    ctx.Finish();

    return  0;
}

