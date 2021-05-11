// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         wULIS.cpp
* @author       Clement Berthaud
* @brief        Binding of ULIS for JavaScript / WebAssembly.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
using namespace ::ULIS;

#include <emscripten/bind.h>
using namespace emscripten;

/////////
// Context Utils
/*
template< typename ... Ts, typename F >
auto ctxCallAdapter( F fptr ) {
    return  [fptr]( FContext* ctx, Ts ... args, py::list iWaitList, FEvent* iEvent ) {
        TArray< FEvent > arr;
        arr.Reserve( iWaitList.size() );
        for( auto it = iWaitList.begin(); it != iWaitList.end(); ++it )
            arr.PushBack( (*it).cast< FEvent >() );
        (ctx->*fptr)( args ..., static_cast< uint32 >( arr.Size() ), arr.Data(), iEvent );
    };
}
*/

/////////
// wULIS4
EMSCRIPTEN_BINDINGS( wULIS4 ) {
    /////////
    // ulError codes
    constant( "NO_ERROR"                        , ULIS_NO_ERROR                         );
    constant( "WARNING_NO_OP"                   , ULIS_WARNING_NO_OP                    );
    constant( "WARNING_NO_OP_GEOMETRY"          , ULIS_WARNING_NO_OP_GEOMETRY           );
    constant( "WARNING_NO_OP_BAD_FILE_FORMAT"   , ULIS_WARNING_NO_OP_BAD_FILE_FORMAT    );
    constant( "WARNING_NO_OP_BAD_INPUT_DATA"    , ULIS_WARNING_NO_OP_BAD_INPUT_DATA     );
    constant( "ERROR_FORMATS_MISMATCH"          , ULIS_ERROR_FORMATS_MISMATCH           );
    constant( "ERROR_CONCURRENT_DATA"           , ULIS_ERROR_CONCURRENT_DATA            );
    constant( "ERROR_BAD_INPUT_DATA"            , ULIS_ERROR_BAD_INPUT_DATA             );



    /////////
    // AXI
    function( "VersionString",                  optional_override( [](){ return  std::string( VersionString().Data() ); } ) );
    function( "VersionMajorString",             optional_override( [](){ return  std::string( VersionMajorString().Data() ); } ) );
    function( "VersionMinorString",             optional_override( [](){ return  std::string( VersionMinorString().Data() ); } ) );
    function( "VersionPatchString",             optional_override( [](){ return  std::string( VersionPatchString().Data() ); } ) );
    function( "VersionMajor",                   &VersionMajor );
    function( "VersionMinor",                   &VersionMinor );
    function( "VersionPatch",                   &VersionPatch );
    function( "ConfigurationString",            optional_override( [](){ return  std::string( ConfigurationString().Data() ); } ) );
    function( "CompilationTimeStampString",     optional_override( [](){ return  std::string( CompilationTimeStampString().Data() ); } ) );
    function( "CompilerNameString",             optional_override( [](){ return  std::string( CompilerNameString().Data() ); } ) );
    function( "CompilerVersionString",          optional_override( [](){ return  std::string( CompilerVersionString().Data() ); } ) );
    function( "CompilerInformationString",      optional_override( [](){ return  std::string( CompilerInformationString().Data() ); } ) );
    function( "BranchName",                     optional_override( [](){ return  std::string( BranchName().Data() ); } ) );
    function( "CommitHash",                     optional_override( [](){ return  std::string( CommitHash().Data() ); } ) );
    function( "CommitAbbreviatedHash",          optional_override( [](){ return  std::string( CommitAbbreviatedHash().Data() ); } ) );
    function( "CompiledFor64Bit",               &CompiledFor64Bit );
    function( "CompiledWithAVX2",               &CompiledWithAVX2 );
    function( "CompiledWithSSE42",              &CompiledWithSSE42 );
    function( "CompiledWithMT",                 &CompiledWithMT );
    function( "FullLibraryInformationString",   optional_override( [](){ return  std::string( FullLibraryInformationString().Data() ); } ) );



    /////////
    // eColorModel
    enum_< eColorModel >( "eColorModel" )
        .value( "ColorModel_GREY",  eColorModel::ColorModel_GREY    )
        .value( "ColorModel_RGB",   eColorModel::ColorModel_RGB     )
        .value( "ColorModel_HSV",   eColorModel::ColorModel_HSV     )
        .value( "ColorModel_HSL",   eColorModel::ColorModel_HSL     )
        .value( "ColorModel_CMY",   eColorModel::ColorModel_CMY     )
        .value( "ColorModel_CMYK",  eColorModel::ColorModel_CMYK    )
        .value( "ColorModel_YUV",   eColorModel::ColorModel_YUV     )
        .value( "ColorModel_Lab",   eColorModel::ColorModel_Lab     )
        .value( "ColorModel_XYZ",   eColorModel::ColorModel_XYZ     )
        .value( "ColorModel_Yxy",   eColorModel::ColorModel_Yxy     );
}

