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
#include <sstream>
#include <iomanip>
#include <iostream>
#include <codecvt>

using namespace ::ul3;
constexpr int criticalError = 0xB00BA420;

int error( const char* iMsg ) {
    std::cout << iMsg << std::endl;
    return criticalError;
}

int clear( int argc, char *argv[] ) {
    // Expected input:
    // 0 - ignored  // 2 - Format   // 4 - Repeat   // 6 - Optimizations
    // 1 - clear    // 3 - Threads  // 5 - Size     // 7 - Extra
    if( argc != 7 ) { return error( "Bad args, abort." ); }
    tFormat format  = std::stoul( std::string( argv[2] ).c_str() );
    uint32  threads = std::atoi( std::string( argv[3] ).c_str() );
    uint32  repeat  = std::atoi( std::string( argv[4] ).c_str() );
    uint32  size    = std::atoi( std::string( argv[5] ).c_str() );
    std::string opt = std::string( argv[6] );
    uint32 optBit = 0;
    if( opt == "sse" ) optBit = ULIS3_PERF_SSE42;
    if( opt == "avx" ) optBit = ULIS3_PERF_AVX2;
    FThreadPool* pool = XCreateThreadPool( threads );
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = ULIS3_PERF_MT | optBit;
    FBlock* block = new FBlock( size, size, format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        Clear( pool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, block, block->Rect() );
    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count();
    delete block;
    XDeleteThreadPool( pool );
    return static_cast< int >( deltaMs );
}

int fill( int argc, char *argv[] ) {
    // Expected input:
    // 0 - ignored  // 2 - Format   // 4 - Repeat   // 6 - Optimizations
    // 1 - clear    // 3 - Threads  // 5 - Size     // 7 - Extra
    if( argc != 7 ) { return error( "Bad args, abort." ); }
    tFormat format  = std::stoul( std::string( argv[2] ).c_str() );
    uint32  threads = std::atoi( std::string( argv[3] ).c_str() );
    uint32  repeat  = std::atoi( std::string( argv[4] ).c_str() );
    uint32  size    = std::atoi( std::string( argv[5] ).c_str() );
    std::string opt = std::string( argv[6] );
    uint32 optBit = 0;
    if( opt == "sse" ) optBit = ULIS3_PERF_SSE42;
    if( opt == "avx" ) optBit = ULIS3_PERF_AVX2;
    FThreadPool* pool = XCreateThreadPool( threads );
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = ULIS3_PERF_MT | optBit;
    FBlock* block = new FBlock( size, size, format );
    FPixelValue source( format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        Fill( pool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, block, source, block->Rect() );
    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count();
    delete block;
    XDeleteThreadPool( pool );
    return static_cast< int >( deltaMs );
}

int copy( int argc, char *argv[] ) {
    // Expected input:
    // 0 - ignored  // 2 - Format   // 4 - Repeat   // 6 - Optimizations
    // 1 - clear    // 3 - Threads  // 5 - Size     // 7 - Extra
    if( argc != 7 ) { return error( "Bad args, abort." ); }
    tFormat format  = std::stoul( std::string( argv[2] ).c_str() );
    uint32  threads = std::atoi( std::string( argv[3] ).c_str() );
    uint32  repeat  = std::atoi( std::string( argv[4] ).c_str() );
    uint32  size    = std::atoi( std::string( argv[5] ).c_str() );
    std::string opt = std::string( argv[6] );
    uint32 optBit = 0;
    if( opt == "sse" ) optBit = ULIS3_PERF_SSE42;
    if( opt == "avx" ) optBit = ULIS3_PERF_AVX2;
    FThreadPool* pool = XCreateThreadPool( threads );
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = ULIS3_PERF_MT | optBit;
    FBlock* src = new FBlock( size, size, format );
    FBlock* dst = new FBlock( size, size, format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        Copy( pool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, src, dst, src->Rect(), FVec2I() );
    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count();
    delete src;
    delete dst;
    XDeleteThreadPool( pool );
    return static_cast< int >( deltaMs );
}

int blend( int argc, char *argv[] ) {
    // Expected input:
    // 0 - ignored  // 2 - Format   // 4 - Repeat   // 6 - Optimizations
    // 1 - clear    // 3 - Threads  // 5 - Size     // 7 - Extra
    if( argc != 10 ) { return error( "Bad args, abort." ); }
    tFormat format  = std::stoul( std::string( argv[2] ).c_str() );
    uint32  threads = std::atoi( std::string( argv[3] ).c_str() );
    uint32  repeat  = std::atoi( std::string( argv[4] ).c_str() );
    uint32  size    = std::atoi( std::string( argv[5] ).c_str() );
    std::string opt = std::string( argv[6] );
    eBlendingMode   blendingMode    = static_cast< eBlendingMode >( std::atoi( std::string( argv[7] ).c_str() ) );
    eAlphaMode      alphaMode       = static_cast< eAlphaMode >( std::atoi( std::string( argv[8] ).c_str() ) );
    bool            subpixelFlag    = static_cast< bool >( std::atoi( std::string( argv[9] ).c_str() ) );
    uint32 optBit = 0;
    if( opt == "sse" ) optBit = ULIS3_PERF_SSE42;
    if( opt == "avx" ) optBit = ULIS3_PERF_AVX2;
    FThreadPool* pool = XCreateThreadPool( threads );
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = ULIS3_PERF_MT | optBit;
    FBlock* src = new FBlock( size, size, format );
    FBlock* dst = new FBlock( size, size, format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        Blend( pool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, src, dst, src->Rect(), FVec2F(), subpixelFlag, blendingMode, alphaMode, 0.5f );
    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count();
    delete src;
    delete dst;
    XDeleteThreadPool( pool );
    return static_cast< int >( deltaMs );
}


int conv( int argc, char *argv[] ) {
    // Expected input:
    // 0 - ignored  // 2 - Format   // 4 - Repeat   // 6 - Optimizations
    // 1 - clear    // 3 - Threads  // 5 - Size     // 7 - Extra
    if( argc != 8 ) { return error( "Bad args, abort." ); }
    tFormat format  = std::stoul( std::string( argv[2] ).c_str() );
    uint32  threads = std::atoi( std::string( argv[3] ).c_str() );
    uint32  repeat  = std::atoi( std::string( argv[4] ).c_str() );
    uint32  size    = std::atoi( std::string( argv[5] ).c_str() );
    std::string opt = std::string( argv[6] );
    tFormat dstFormat = std::stoul( std::string( argv[7] ).c_str() );
    uint32 optBit = 0;
    if( opt == "sse" ) optBit = ULIS3_PERF_SSE42;
    if( opt == "avx" ) optBit = ULIS3_PERF_AVX2;
    FThreadPool* pool = XCreateThreadPool( threads );
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = ULIS3_PERF_MT | optBit;
    FBlock* src = new FBlock( size, size, format );
    FBlock* dst = new FBlock( size, size, dstFormat );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        Conv( pool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, src, dst );
    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count();
    delete src;
    delete dst;
    XDeleteThreadPool( pool );
    return static_cast< int >( deltaMs );
}

int clearRaw( int argc, char *argv[] ) {
    // Expected input:
    // 0 - ignored  // 2 - Format   // 4 - Repeat   // 6 - Optimizations
    // 1 - clear    // 3 - Threads  // 5 - Size     // 7 - Extra
    if( argc != 6 ) { return error( "Bad args, abort." ); }
    tFormat format  = std::stoul( std::string( argv[2] ).c_str() );
    uint32  threads = std::atoi( std::string( argv[3] ).c_str() );
    uint32  repeat  = std::atoi( std::string( argv[4] ).c_str() );
    uint32  size    = std::atoi( std::string( argv[5] ).c_str() );
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    FBlock* block = new FBlock( size, size, format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        ClearRaw( block, ULIS3_NOCB );
    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count();
    delete block;
    return static_cast< int >( deltaMs );
}

int copyRaw( int argc, char *argv[] ) {
    // Expected input:
    // 0 - ignored  // 2 - Format   // 4 - Repeat   // 6 - Optimizations
    // 1 - clear    // 3 - Threads  // 5 - Size     // 7 - Extra
    if( argc != 6 ) { return error( "Bad args, abort." ); }
    tFormat format  = std::stoul( std::string( argv[2] ).c_str() );
    uint32  threads = std::atoi( std::string( argv[3] ).c_str() );
    uint32  repeat  = std::atoi( std::string( argv[4] ).c_str() );
    uint32  size    = std::atoi( std::string( argv[5] ).c_str() );
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    FBlock* src = new FBlock( size, size, format );
    FBlock* dst = new FBlock( size, size, format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        CopyRaw( src, dst, ULIS3_NOCB );
    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count();
    delete src;
    delete dst;
    return static_cast< int >( deltaMs );
}

int transform( int argc, char *argv[] ) {
    // Expected input:
    // 0 - ignored  // 2 - Format   // 4 - Repeat   // 6 - Optimizations
    // 1 - clear    // 3 - Threads  // 5 - Size     // 7 - Extra
    if( argc != 17 ) { return error( "Bad args, abort." ); }
    tFormat format  = std::stoul( std::string( argv[2] ).c_str() );
    uint32  threads = std::atoi( std::string( argv[3] ).c_str() );
    uint32  repeat  = std::atoi( std::string( argv[4] ).c_str() );
    uint32  size    = std::atoi( std::string( argv[5] ).c_str() );
    std::string opt = std::string( argv[6] );
    eResamplingMethod method = static_cast< eResamplingMethod >( std::atoi( argv[7] ) );
    float m00 = static_cast< float >( std::atof( argv[8] ) );
    float m10 = static_cast< float >( std::atof( argv[9] ) );
    float m20 = static_cast< float >( std::atof( argv[10] ) );
    float m01 = static_cast< float >( std::atof( argv[11] ) );
    float m11 = static_cast< float >( std::atof( argv[12] ) );
    float m21 = static_cast< float >( std::atof( argv[13] ) );
    float m02 = static_cast< float >( std::atof( argv[14] ) );
    float m12 = static_cast< float >( std::atof( argv[15] ) );
    float m22 = static_cast< float >( std::atof( argv[16] ) );
    FTransform2D mat( FTransform2D::MakeFromMatrix( m00, m10, m20, m01, m11, m21, m02, m12, m22 ) );
    uint32 optBit = 0;
    if( opt == "sse" ) optBit = ULIS3_PERF_SSE42;
    if( opt == "avx" ) optBit = ULIS3_PERF_AVX2;
    FThreadPool* pool = XCreateThreadPool( threads );
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = ULIS3_PERF_MT | optBit;
    FBlock* src = new FBlock( size, size, format );
    FRect dstmetrics = TransformAffineMetrics( src->Rect(), mat, method );
    FBlock* dst = new FBlock( dstmetrics.w, dstmetrics.h, format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        TransformAffine( pool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, src, dst, src->Rect(), mat, method );
    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count();
    delete src;
    delete dst;
    XDeleteThreadPool( pool );
    return static_cast< int >( deltaMs );
}

int text( int argc, char *argv[] ) {
    // Expected input:
    // 0 - ignored  // 2 - Format   // 4 - Repeat   // 6 - Optimizations
    // 1 - clear    // 3 - Threads  // 5 - Size     // 7 - Extra
    if( argc != 12 ) { return error( "Bad args, abort." ); }
    tFormat format  = std::stoul( std::string( argv[2] ).c_str() );
    uint32  threads = std::atoi( std::string( argv[3] ).c_str() );
    uint32  repeat  = std::atoi( std::string( argv[4] ).c_str() );
    uint32  size    = std::atoi( std::string( argv[5] ).c_str() );
    std::string opt = std::string( argv[6] );

    std::string txt = std::string( argv[7] );
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type, wchar_t> converter;
    std::wstring wtxt = converter.from_bytes(txt);
    std::string fam = std::string( argv[8] );
    std::string style = std::string( argv[9] );
    uint32 fontSize = std::atoi( std::string( argv[10] ).c_str() );
    bool antialiasingFlag = static_cast< bool >( std::atoi( std::string( argv[11] ).c_str() ) );
    uint32 optBit = 0;
    if( opt == "sse" ) optBit = ULIS3_PERF_SSE42;
    if( opt == "avx" ) optBit = ULIS3_PERF_AVX2;
    FThreadPool* pool = XCreateThreadPool( threads );
    FHostDeviceInfo host = FHostDeviceInfo::Detect();
    uint32 perfIntent = ULIS3_PERF_MT | optBit;
    FFontEngine fontEngine;
    FFontRegistry fontRegistry( fontEngine );
    FFont font( fontRegistry, fam, style );
    FRect textmetrics = TextMetrics( wtxt, font, fontSize, FTransform2D() );
    FPixelValue color( format );
    FBlock* dst = new FBlock( textmetrics.w, textmetrics.h, format );
    auto startTime = std::chrono::steady_clock::now();
    for( uint32 l = 0; l < repeat; ++l )
        RenderText( pool, ULIS3_BLOCKING, perfIntent, host, ULIS3_NOCB, dst, wtxt, font, fontSize, color, FTransform2D(), antialiasingFlag );
    auto endTime = std::chrono::steady_clock::now();
    auto deltaMs = std::chrono::duration_cast< std::chrono::milliseconds>( endTime - startTime ).count();
    delete dst;
    XDeleteThreadPool( pool );
    return static_cast< int >( deltaMs );
}

// Call examples:
// Benchmark.exe <OP>       <FORMAT>    <THREADS>   <REPEAT>    <SIZE>  <OPT>   <EXTRA>
// Benchmark.exe clear      99451       12          1000        1024    sse
// Benchmark.exe fill       99451       12          1000        1024    sse
// Benchmark.exe copy       99451       12          1000        1024    sse
// Benchmark.exe blend      99451       12          1000        1024    sse     <BM>    <AM>    <AA>
// Benchmark.exe conv       99451       12          1000        1024    sse     <TO>
// Benchmark.exe transform  99451       12          1000        1024    sse     <INTERP>    <m00 ... m22> (9 cells)
// Benchmark.exe text       99451       12          1000        1024    sse     <TEXT>  <FFAM>  <FSTYLE>    <SIZE>  <AA>
int main( int argc, char *argv[] ) {
    // 0    - ignored
    // 1    - OP
    // 2    - Format
    // 3    - Threads
    // 4    - Repeat
    // 5    - Size
    // 6    - Optimizations
    // 7+   - Extra ...

    if( argc <=1 )
        return error( "Bad args, abort." );

    std::string op = argv[1];

    int exit_code = criticalError;
         if( op == "clear"      )   exit_code = clear(      argc, argv );
    else if( op == "fill"       )   exit_code = fill(       argc, argv );
    else if( op == "copy"       )   exit_code = copy(       argc, argv );
    else if( op == "blend"      )   exit_code = blend(      argc, argv );
    else if( op == "conv"       )   exit_code = conv(       argc, argv );
    else if( op == "clearRaw"   )   exit_code = clearRaw(   argc, argv );
    else if( op == "copyRaw"    )   exit_code = copyRaw(    argc, argv );
    else if( op == "transform"  )   exit_code = transform(  argc, argv );
    else if( op == "text"       )   exit_code = text(       argc, argv );
    else return error( "Bad Op, abort." );

    return  exit_code;
}

