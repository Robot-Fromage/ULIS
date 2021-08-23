// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_Profiler.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include <ULIS_CORE>
#include <chrono>
#include <sstream>
#include <iomanip>
#if defined(_MSC_VER)
#include <Windows.h>
#endif

struct FSize { int w, h; };
FSize image_sizes[] ={ { 64, 64 }
                    , { 128, 128 }
                    , { 256, 256 }
                    , { 512, 512 }
                    , { 1024, 1024 }
                    , { 2048, 2048 }
                    , { 4096, 4096 }
                    , { 8192, 8192 }
                    , { 16384, 16384 } };
#define SIZEOF_IMAGE_SIZES sizeof( image_sizes ) / sizeof( FSize )
#define SIZEOF_NUM_THREADS sizeof( num_threads ) / sizeof( int )

#if defined(__clang__)
std::string compiler = "CLANG";
#elif defined(__GNUC__) || defined(__GNUG__)
std::string compiler = "GCC";
#elif defined(_MSC_VER)
std::string compiler = "MSVC";
#endif

//#define ULIS_PROFILER_VERBOSE

int error( const char* iMsg )
{
    std::cout << iMsg << std::endl;
    return -1;
}

std::string bake_tab( int count )
{
    std::string out;
    for( int i = 0; i < count; ++ i )
        out += ' ';
    return  out;
}

void print_short_op( const std::string& iOp, const std::string& iFormat, const std::string& iNumThreads, const std::string& iRepeat, const std::string& iSize, const std::string& iOptimization, const std::string& iExtra )
{
    // 0        9       17                                  53  57      65      73      81              97
    // CLANG    clear   uint32CMYKhasAlphaCMYKAtypeLimits   64  10000   64      mem     LighterColor    0.05ms
    int index0 = 0;
    int index1 = 9;
    int index2 = 17;
    int index3 = 53;
    int index4 = 57;
    int index5 = 65;
    int index6 = 73;
    int index7 = 81;
    int index8 = 97;
    int del1 = index1 - index0;
    int del2 = index2 - index1;
    int del3 = index3 - index2;
    int del4 = index4 - index3;
    int del5 = index5 - index4;
    int del6 = index6 - index5;
    int del7 = index7 - index6;
    int del8 = index8 - index7;
    std::string tab0 = bake_tab( del1 - compiler.size() );
    std::string tab1 = bake_tab( del2 - iOp.size() );
    std::string tab2 = bake_tab( del3 - iFormat.size() );
    std::string tab3 = bake_tab( del4 - iNumThreads.size() );
    std::string tab4 = bake_tab( del5 - iRepeat.size() );
    std::string tab5 = bake_tab( del6 - iSize.size() );
    std::string tab6 = bake_tab( del7 - iOptimization.size() );
    std::string tab7 = bake_tab( del8 - iExtra.size() );
    
    std::cout << compiler << tab0 << iOp << tab1 << iFormat << tab2 << iNumThreads << tab3 << iRepeat << tab4 << iSize << tab5 << iOptimization << tab6 << iExtra << tab7;
}

std::string stringizefloat( float val )
{
    std::string out;

    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << val;
    out = stream.str();
    return  out;
}

bool check_fmt( ::ULIS::uint32 fmt )
{
    if( fmt == 0 )
        return  false;

    bool in = false;
    for( int i = 0; i < ULIS_REG_SIZE; ++i )
        if( ULIS_REG[i] == fmt )
            in = true;

    return  in;
}


bool check_NT( int NT )
{
    return  NT > 0 && NT <= 64;
}


bool check_num( int num )
{
    return  num > 0;
}

bool check_size( int size )
{
    bool in = false;
    for( int i = 0; i < SIZEOF_IMAGE_SIZES; ++i )
        if( image_sizes[i].w == size )
            in = true;

    return  in;
}


bool check_opt( const std::string& opt )
{
    return  ( opt == "sse" || opt == "mem" || opt == "reg" );
}


bool check_mode( int mode )
{
    return  mode >= 0 && mode < (int)::ULIS::eBlendingMode::kNumBlendingModes;
}

int clear( int argc, char *argv[] )
{
    // Expected input:
    // 0 - ignored
    // 1 - clear
    // 2 - fmt
    // 3 - NT
    // 4 - num
    // 5 - size
    // 6 - opt
    // 7 - ignored
    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << "Processing clear, parsing args..." << std::endl;
    #endif // ULIS_PROFILER_VERBOSE
    if( argc != 7 ) { return error( "Bad args, abort." ); }
    ::ULIS::uint32  fmt  = std::stoul( std::string( argv[2] ).c_str() );
    ::ULIS::uint32  NT   = std::atoi( std::string( argv[3] ).c_str() );
    ::ULIS::uint32  num  = std::atoi( std::string( argv[4] ).c_str() );
    ::ULIS::uint32  size = std::atoi( std::string( argv[5] ).c_str() );
    std::string     opt  = std::string( argv[6] );
    #ifdef ULIS_PROFILER_VERBOSE
    std::cout << "OP:   " << "clear"    << std::endl;
        if( check_fmt(    fmt     ) )   { std::cout << "fmt:  " << fmt << " (" << ::ULIS::FSpecContext::BlockSpecFromHash( fmt )._ss << ")" << std::endl; } else { return error( "Bad arg: fmt, abort."   ); }
        if( check_NT(     NT      ) )   { std::cout << "NT:   " << NT         << std::endl; } else { return error( "Bad arg: NT, abort."    ); }
        if( check_num(    num     ) )   { std::cout << "num:  " << num        << std::endl; } else { return error( "Bad arg: num, abort."   ); }
        if( check_size(   size    ) )   { std::cout << "size: " << size       << std::endl; } else { return error( "Bad arg: size, abort."  ); }
        if( check_opt(    opt     ) )   { std::cout << "opt:  " << opt        << std::endl; } else { return error( "Bad arg: opt, abort."   ); }
    #else
        if( !check_fmt(    fmt     ) )  { return error( "Bad arg: fmt, abort."   ); }
        if( !check_NT(     NT      ) )  { return error( "Bad arg: NT, abort."    ); }
        if( !check_num(    num     ) )  { return error( "Bad arg: num, abort."   ); }
        if( !check_size(   size    ) )  { return error( "Bad arg: size, abort."  ); }
        if( !check_opt(    opt     ) )  { return error( "Bad arg: opt, abort."   ); }
        print_short_op( argv[1], ::ULIS::FSpecContext::BlockSpecFromHash( fmt )._ss, argv[3], argv[4], argv[5], argv[6], "" );
    #endif // ULIS_PROFILER_VERBOSE

    ::ULIS::FThreadPool& pool = ::ULIS::FGlobalThreadPool::Get();
    pool.SetNumWorkers( NT );

    ::ULIS::IBlock* block = ::ULIS::FMakeContext::MakeBlock( size, size, fmt );

    ::ULIS::FPerformanceOptions strat;
    if( opt == "mem" ) strat.use_mem_if_available;
    if( opt == "sse" ) strat.use_sse_if_available;

    auto start_time = std::chrono::steady_clock::now();

    for( int l = 0; l < num; ++l )
    {
        float percent = ( l / float( num-1 ) );
        std::cout << percent << "%";
        ::ULIS::FClearFillContext::Clear( block, strat );
        std::cout << "\b\b\b\b\b";
    }

    auto end_time   = std::chrono::steady_clock::now();
    auto delta      = std::chrono::duration_cast< std::chrono::milliseconds>(end_time - start_time ).count();
    float average   = delta / (float)num;

    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << std::endl;
        std::cout << "Result:   " << average << "ms" << std::endl;
        std::cout << std::endl;
    #else
        std::cout << stringizefloat( average ) << "ms" << std::endl;
    #endif // ULIS_PROFILER_VERBOSE

    delete block;

    return 0;
}

int fill( int argc, char *argv[] )
{
    // Expected input:
    // 0 - ignored
    // 1 - fill
    // 2 - fmt
    // 3 - NT
    // 4 - num
    // 5 - size
    // 6 - opt
    // 7 - ignored
    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << "Processing fill, parsing args..." << std::endl;
    #endif // ULIS_PROFILER_VERBOSE
    if( argc != 7 ) { return error( "Bad args, abort." ); }
    ::ULIS::uint32  fmt  = std::stoul( std::string( argv[2] ).c_str() );
    ::ULIS::uint32  NT   = std::atoi( std::string( argv[3] ).c_str() );
    ::ULIS::uint32  num  = std::atoi( std::string( argv[4] ).c_str() );
    ::ULIS::uint32  size = std::atoi( std::string( argv[5] ).c_str() );
    std::string     opt  = std::string( argv[6] );
    #ifdef ULIS_PROFILER_VERBOSE
    std::cout << "OP:   " << "fill"    << std::endl;
        if( check_fmt(    fmt     ) )   { std::cout << "fmt:  " << fmt << " (" << ::ULIS::FSpecContext::BlockSpecFromHash( fmt )._ss << ")" << std::endl; } else { return error( "Bad arg: fmt, abort."   ); }
        if( check_NT(     NT      ) )   { std::cout << "NT:   " << NT         << std::endl; } else { return error( "Bad arg: NT, abort."    ); }
        if( check_num(    num     ) )   { std::cout << "num:  " << num        << std::endl; } else { return error( "Bad arg: num, abort."   ); }
        if( check_size(   size    ) )   { std::cout << "size: " << size       << std::endl; } else { return error( "Bad arg: size, abort."  ); }
        if( check_opt(    opt     ) )   { std::cout << "opt:  " << opt        << std::endl; } else { return error( "Bad arg: opt, abort."   ); }
    #else
        if( !check_fmt(    fmt     ) )  { return error( "Bad arg: fmt, abort."   ); }
        if( !check_NT(     NT      ) )  { return error( "Bad arg: NT, abort."    ); }
        if( !check_num(    num     ) )  { return error( "Bad arg: num, abort."   ); }
        if( !check_size(   size    ) )  { return error( "Bad arg: size, abort."  ); }
        if( !check_opt(    opt     ) )  { return error( "Bad arg: opt, abort."   ); }
        print_short_op( argv[1], ::ULIS::FSpecContext::BlockSpecFromHash( fmt )._ss, argv[3], argv[4], argv[5], argv[6], "" );
    #endif // ULIS_PROFILER_VERBOSE

    ::ULIS::FThreadPool& pool = ::ULIS::FGlobalThreadPool::Get();
    pool.SetNumWorkers( NT );

    ::ULIS::IBlock* block = ::ULIS::FMakeContext::MakeBlock( size, size, fmt );
    ::ULIS::CColor color( 0, 0, 0 );

    ::ULIS::FPerformanceOptions strat;
    if( opt == "mem" ) strat.use_mem_if_available;
    if( opt == "sse" ) strat.use_sse_if_available;

    auto start_time = std::chrono::steady_clock::now();

    for( int l = 0; l < num; ++l )
    {
        float percent = ( l / float( num-1 ) );
        std::cout << percent << "%";
        ::ULIS::FClearFillContext::Fill( block, color, strat );
        std::cout << "\b\b\b\b\b";
    }

    auto end_time   = std::chrono::steady_clock::now();
    auto delta      = std::chrono::duration_cast< std::chrono::milliseconds>(end_time - start_time ).count();
    float average   = delta / (float)num;

    
    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << std::endl;
        std::cout << "Result:   " << average << "ms" << std::endl;
        std::cout << std::endl;
    #else
        std::cout << stringizefloat( average ) << "ms" << std::endl;
    #endif // ULIS_PROFILER_VERBOSE

    delete block;

    return 0;
}

int blend( int argc, char *argv[] )
{
    // Expected input:
    // 0 - ignored
    // 1 - blend
    // 2 - fmt
    // 3 - NT
    // 4 - num
    // 5 - size
    // 6 - opt
    // 7 - mode
    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << "Processing fill, parsing args..." << std::endl;
    #endif // ULIS_PROFILER_VERBOSE
    if( argc != 8 ) { return error( "Bad args, abort." ); }
    ::ULIS::uint32  fmt  = std::stoul( std::string( argv[2] ).c_str() );
    ::ULIS::uint32  NT   = std::atoi( std::string( argv[3] ).c_str() );
    ::ULIS::uint32  num  = std::atoi( std::string( argv[4] ).c_str() );
    ::ULIS::uint32  size = std::atoi( std::string( argv[5] ).c_str() );
    std::string     opt  = std::string( argv[6] );
    ::ULIS::uint32  mode = std::atoi( std::string( argv[7] ).c_str() );
    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << "OP:   " << "fill"    << std::endl;
        if( check_fmt(    fmt     ) )   { std::cout << "fmt:  " << fmt << " (" << ::ULIS::FSpecContext::BlockSpecFromHash( fmt )._ss << ")" << std::endl; } else { return error( "Bad arg: fmt, abort."   ); }
        if( check_NT(     NT      ) )   { std::cout << "NT:   " << NT         << std::endl; } else { return error( "Bad arg: NT, abort."    ); }
        if( check_num(    num     ) )   { std::cout << "num:  " << num        << std::endl; } else { return error( "Bad arg: num, abort."   ); }
        if( check_size(   size    ) )   { std::cout << "size: " << size       << std::endl; } else { return error( "Bad arg: size, abort."  ); }
        if( check_opt(    opt     ) )   { std::cout << "opt:  " << opt        << std::endl; } else { return error( "Bad arg: opt, abort."   ); }
        if( check_mode(   mode    ) )   { std::cout << "mode: " << ::ULIS::kwBlendingMode[mode] << std::endl; } else { return error( "Bad arg: opt, abort."   ); }
    #else
        if( !check_fmt(    fmt     ) ) { return error( "Bad arg: fmt, abort."   ); }
        if( !check_NT(     NT      ) ) { return error( "Bad arg: NT, abort."    ); }
        if( !check_num(    num     ) ) { return error( "Bad arg: num, abort."   ); }
        if( !check_size(   size    ) ) { return error( "Bad arg: size, abort."  ); }
        if( !check_opt(    opt     ) ) { return error( "Bad arg: opt, abort."   ); }
        if( !check_mode(   mode    ) ) { return error( "Bad arg: opt, abort."   ); }
        print_short_op( argv[1], ::ULIS::FSpecContext::BlockSpecFromHash( fmt )._ss, argv[3], argv[4], argv[5], argv[6], ::ULIS::kwBlendingMode[mode] );
    #endif // ULIS_PROFILER_VERBOSE

    ::ULIS::FThreadPool& pool = ::ULIS::FGlobalThreadPool::Get();
    pool.SetNumWorkers( NT );

    ::ULIS::IBlock* blockA = ::ULIS::FMakeContext::MakeBlock( size, size, fmt );
    ::ULIS::IBlock* blockB = ::ULIS::FMakeContext::MakeBlock( size, size, fmt );
    ::ULIS::eBlendingMode bm = (::ULIS::eBlendingMode)mode;

    ::ULIS::FPerformanceOptions strat;
    if( opt == "mem" ) strat.use_mem_if_available;
    if( opt == "sse" ) strat.use_sse_if_available;

    auto start_time = std::chrono::steady_clock::now();

    for( int l = 0; l < num; ++l )
    {
        float percent = ( l / float( num-1 ) );
        std::cout << percent << "%";
        ::ULIS::FBlendingContext::Blend( blockA, blockB, bm, 1.f, 0, 0, strat );
        std::cout << "\b\b\b\b\b";
    }

    auto end_time   = std::chrono::steady_clock::now();
    auto delta      = std::chrono::duration_cast< std::chrono::milliseconds>(end_time - start_time ).count();
    float average   = delta / (float)num;
    
    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << std::endl;
        std::cout << "Result:   " << average << "ms" << std::endl;
        std::cout << std::endl;
    #else
        std::cout << stringizefloat( average ) << "ms" << std::endl;
    #endif // ULIS_PROFILER_VERBOSE

    delete blockA;
    delete blockB;

    return 0;
}

int copy( int argc, char *argv[] )
{
    // Expected input:
    // 0 - ignored
    // 1 - copy
    // 2 - fmt
    // 3 - NT
    // 4 - num
    // 5 - size
    // 6 - opt
    // 7 - ignored
    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << "Processing copy, parsing args..." << std::endl;
    #endif // ULIS_PROFILER_VERBOSE
    if( argc != 7 ) { return error( "Bad args, abort." ); }
    ::ULIS::uint32  fmt  = std::stoul( std::string( argv[2] ).c_str() );
    ::ULIS::uint32  NT   = std::atoi( std::string( argv[3] ).c_str() );
    ::ULIS::uint32  num  = std::atoi( std::string( argv[4] ).c_str() );
    ::ULIS::uint32  size = std::atoi( std::string( argv[5] ).c_str() );
    std::string     opt  = std::string( argv[6] );
    #ifdef ULIS_PROFILER_VERBOSE
    std::cout << "OP:   " << "copy"    << std::endl;
        if( check_fmt(    fmt     ) )   { std::cout << "fmt:  " << fmt << " (" << ::ULIS::FSpecContext::BlockSpecFromHash( fmt )._ss << ")" << std::endl; } else { return error( "Bad arg: fmt, abort."   ); }
        if( check_NT(     NT      ) )   { std::cout << "NT:   " << NT         << std::endl; } else { return error( "Bad arg: NT, abort."    ); }
        if( check_num(    num     ) )   { std::cout << "num:  " << num        << std::endl; } else { return error( "Bad arg: num, abort."   ); }
        if( check_size(   size    ) )   { std::cout << "size: " << size       << std::endl; } else { return error( "Bad arg: size, abort."  ); }
        if( check_opt(    opt     ) )   { std::cout << "opt:  " << opt        << std::endl; } else { return error( "Bad arg: opt, abort."   ); }
    #else
        if( !check_fmt(    fmt     ) )  { return error( "Bad arg: fmt, abort."   ); }
        if( !check_NT(     NT      ) )  { return error( "Bad arg: NT, abort."    ); }
        if( !check_num(    num     ) )  { return error( "Bad arg: num, abort."   ); }
        if( !check_size(   size    ) )  { return error( "Bad arg: size, abort."  ); }
        if( !check_opt(    opt     ) )  { return error( "Bad arg: opt, abort."   ); }
        print_short_op( argv[1], ::ULIS::FSpecContext::BlockSpecFromHash( fmt )._ss, argv[3], argv[4], argv[5], argv[6], "" );
    #endif // ULIS_PROFILER_VERBOSE

    ::ULIS::FThreadPool& pool = ::ULIS::FGlobalThreadPool::Get();
    pool.SetNumWorkers( NT );

    ::ULIS::IBlock* blockA = ::ULIS::FMakeContext::MakeBlock( size, size, fmt );
    ::ULIS::IBlock* blockB = ::ULIS::FMakeContext::MakeBlock( size, size, fmt );

    ::ULIS::FPerformanceOptions strat;
    if( opt == "mem" ) strat.use_mem_if_available;
    if( opt == "sse" ) strat.use_sse_if_available;

    auto start_time = std::chrono::steady_clock::now();

    for( int l = 0; l < num; ++l )
    {
        float percent = ( l / float( num-1 ) );
        std::cout << percent << "%";
        ::ULIS::FMakeContext::CopyBlockInto( blockA, blockB, strat );
        std::cout << "\b\b\b\b\b";
    }

    auto end_time   = std::chrono::steady_clock::now();
    auto delta      = std::chrono::duration_cast< std::chrono::milliseconds>(end_time - start_time ).count();
    float average   = delta / (float)num;

    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << std::endl;
        std::cout << "Result:   " << average << "ms" << std::endl;
        std::cout << std::endl;
    #else
        std::cout << stringizefloat( average ) << "ms" << std::endl;
    #endif // ULIS_PROFILER_VERBOSE

    delete blockA;
    delete blockB;

    return 0;
}

// Call examples:
// ULIS_Profiler.exe clear  0           64  1000    256     mem
// ULIS_Profiler.exe fill   0           32  200     2048    sse
// ULIS_Profiler.exe blend  3726693057  64  500     8192    reg		8
// ULIS_Profiler.exe copy  	3726693057  32  10000   256     reg
int main( int argc, char *argv[] )
{
    // Expected input:
    // 0 - ignored
    // 1 - OP
    // 2 - fmt
    // 3 - NT
    // 4 - num
    // 5 - size
    // 6 - opt
    // 7 - extra

    if( argc <=1 ) { return error( "No args, abort." ); }

    #ifdef ULIS_PROFILER_VERBOSE
        std::cout << "Requested OP: " << argv[1] << std::endl;
    #endif // ULIS_PROFILER_VERBOSE
    std::string op = argv[1];

    std::cout << std::fixed << std::setprecision(2);
         if( op == "clear"  ) { return  clear(  argc, argv ); }
    else if( op == "fill"   ) { return  fill(   argc, argv ); }
    else if( op == "blend"  ) { return  blend(  argc, argv ); }
    else if( op == "copy"   ) { return  copy(   argc, argv ); }
    else { return  error( "Bad Op, abort." ); }

    return 0;
}

