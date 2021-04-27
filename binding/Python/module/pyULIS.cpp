// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         pyULIS.cpp
* @author       Clement Berthaud
* @brief        Binding of ULIS for Python.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma warning(push)
// pragma warning disable is bad, except when i don't give a shit about
// 3rdparty warnings that prevent me from seeing actually interesting mistakes.
#pragma warning(disable : 4530 ) // Exception
#pragma warning(disable : 4244 ) // Conversion
#pragma warning(disable : 4180 ) // Qualifiers ignored
#include <iostream>
#include <ULIS>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
using namespace ::ULIS;
using namespace pybind11::literals;
namespace py = pybind11;

/////////
// Context Utils
template< typename ... Ts, typename F >
auto ctxCallAdapter( F fptr ) {
    return  [fptr]( FContext* ctx, Ts ... args, py::list iWaitList, FEvent* iEvent ) {
        TArray< FEvent > arr;
        arr.Reserve( iWaitList.size() );
        /*for( auto it = iWaitList ) {
            arr.PushBack( FEvent( it ) );
        }*/
        (ctx->*fptr)( args ..., static_cast< uint32 >( arr.Size() ), arr.Data(), iEvent );
    };
}

/////////
// pyULIS4
PYBIND11_MODULE( pyULIS4, m ) {
    m.doc() = "pyULIS4 module, a python binding for ULIS.";



    /////////
    // ulError codes
    m.attr( "NO_ERROR"                      ) = ULIS_NO_ERROR;
    m.attr( "WARNING_NO_OP"                 ) = ULIS_WARNING_NO_OP;
    m.attr( "WARNING_NO_OP_GEOMETRY"        ) = ULIS_WARNING_NO_OP_GEOMETRY;
    m.attr( "WARNING_NO_OP_BAD_FILE_FORMAT" ) = ULIS_WARNING_NO_OP_BAD_FILE_FORMAT;
    m.attr( "WARNING_NO_OP_BAD_INPUT_DATA"  ) = ULIS_WARNING_NO_OP_BAD_INPUT_DATA;
    m.attr( "ERROR_FORMATS_MISMATCH"        ) = ULIS_ERROR_FORMATS_MISMATCH;
    m.attr( "ERROR_CONCURRENT_DATA"         ) = ULIS_ERROR_CONCURRENT_DATA;
    m.attr( "ERROR_BAD_INPUT_DATA"          ) = ULIS_ERROR_BAD_INPUT_DATA;



    /////////
    // AXI
    m.def( "VersionString",                 [](){ return  std::string( VersionString().Data() ); } );
    m.def( "VersionMajorString",            [](){ return  std::string( VersionMajorString().Data() ); } );
    m.def( "VersionMinorString",            [](){ return  std::string( VersionMinorString().Data() ); } );
    m.def( "VersionPatchString",            [](){ return  std::string( VersionPatchString().Data() ); } );
    m.def( "VersionMajor",                  &VersionMajor );
    m.def( "VersionMinor",                  &VersionMinor );
    m.def( "VersionPatch",                  &VersionPatch );
    m.def( "ConfigurationString",           [](){ return  std::string( ConfigurationString().Data() ); } );
    m.def( "CompilationTimeStampString",    [](){ return  std::string( CompilationTimeStampString().Data() ); } );
    m.def( "CompilerNameString",            [](){ return  std::string( CompilerNameString().Data() ); } );
    m.def( "CompilerVersionString",         [](){ return  std::string( CompilerVersionString().Data() ); } );
    m.def( "CompilerInformationString",     [](){ return  std::string( CompilerInformationString().Data() ); } );
    m.def( "BranchName",                    [](){ return  std::string( BranchName().Data() ); } );
    m.def( "CommitHash",                    [](){ return  std::string( CommitHash().Data() ); } );
    m.def( "CommitAbbreviatedHash",         [](){ return  std::string( CommitAbbreviatedHash().Data() ); } );
    m.def( "CompiledFor64Bit",              &CompiledFor64Bit );
    m.def( "CompiledWithAVX2",              &CompiledWithAVX2 );
    m.def( "CompiledWithSSE42",             &CompiledWithSSE42 );
    m.def( "CompiledWithMT",                &CompiledWithMT );
    m.def( "FullLibraryInformationString",  [](){ return  std::string( FullLibraryInformationString().Data() ); } );



    /////////
    // eColorModel
    py::enum_< eColorModel >( m, "eColorModel" )
        .value( "ColorModel_GREY",  eColorModel::ColorModel_GREY    )
        .value( "ColorModel_RGB",   eColorModel::ColorModel_RGB     )
        .value( "ColorModel_HSV",   eColorModel::ColorModel_HSV     )
        .value( "ColorModel_HSL",   eColorModel::ColorModel_HSL     )
        .value( "ColorModel_CMY",   eColorModel::ColorModel_CMY     )
        .value( "ColorModel_CMYK",  eColorModel::ColorModel_CMYK    )
        .value( "ColorModel_YUV",   eColorModel::ColorModel_YUV     )
        .value( "ColorModel_Lab",   eColorModel::ColorModel_Lab     )
        .value( "ColorModel_XYZ",   eColorModel::ColorModel_XYZ     )
        .value( "ColorModel_Yxy",   eColorModel::ColorModel_Yxy     )
        .export_values();



    /////////
    // eType
    py::enum_< eType >( m, "eType" )
        .value( "Type_uint8",   eType::Type_uint8   )
        .value( "Type_uint16",  eType::Type_uint16  )
        .value( "Type_ufloat",  eType::Type_ufloat  )
        .export_values();



    /////////
    // eFormat
    py::enum_< eFormat >( m, "eFormat", py::arithmetic() )
        .value( "Format_G8",        eFormat::Format_G8      )
        .value( "Format_GA8",       eFormat::Format_GA8     )
        .value( "Format_AG8",       eFormat::Format_AG8     )
        .value( "Format_G16",       eFormat::Format_G16     )
        .value( "Format_GA16",      eFormat::Format_GA16    )
        .value( "Format_AG16",      eFormat::Format_AG16    )
        .value( "Format_GF",        eFormat::Format_GF      )
        .value( "Format_GAF",       eFormat::Format_GAF     )
        .value( "Format_AGF",       eFormat::Format_AGF     )
        .value( "Format_RGB8",      eFormat::Format_RGB8    )
        .value( "Format_BGR8",      eFormat::Format_BGR8    )
        .value( "Format_RGBA8",     eFormat::Format_RGBA8   )
        .value( "Format_ABGR8",     eFormat::Format_ABGR8   )
        .value( "Format_ARGB8",     eFormat::Format_ARGB8   )
        .value( "Format_BGRA8",     eFormat::Format_BGRA8   )
        .value( "Format_RGB16",     eFormat::Format_RGB16   )
        .value( "Format_BGR16",     eFormat::Format_BGR16   )
        .value( "Format_RGBA16",    eFormat::Format_RGBA16  )
        .value( "Format_ABGR16",    eFormat::Format_ABGR16  )
        .value( "Format_ARGB16",    eFormat::Format_ARGB16  )
        .value( "Format_BGRA16",    eFormat::Format_BGRA16  )
        .value( "Format_RGBF",      eFormat::Format_RGBF    )
        .value( "Format_BGRF",      eFormat::Format_BGRF    )
        .value( "Format_RGBAF",     eFormat::Format_RGBAF   )
        .value( "Format_ABGRF",     eFormat::Format_ABGRF   )
        .value( "Format_ARGBF",     eFormat::Format_ARGBF   )
        .value( "Format_BGRAF",     eFormat::Format_BGRAF   )
        .value( "Format_HSV8",      eFormat::Format_HSV8    )
        .value( "Format_VSH8",      eFormat::Format_VSH8    )
        .value( "Format_HSVA8",     eFormat::Format_HSVA8   )
        .value( "Format_AVSH8",     eFormat::Format_AVSH8   )
        .value( "Format_AHSV8",     eFormat::Format_AHSV8   )
        .value( "Format_VSHA8",     eFormat::Format_VSHA8   )
        .value( "Format_HSV16",     eFormat::Format_HSV16   )
        .value( "Format_VSH16",     eFormat::Format_VSH16   )
        .value( "Format_HSVA16",    eFormat::Format_HSVA16  )
        .value( "Format_AVSH16",    eFormat::Format_AVSH16  )
        .value( "Format_AHSV16",    eFormat::Format_AHSV16  )
        .value( "Format_VSHA16",    eFormat::Format_VSHA16  )
        .value( "Format_HSVF",      eFormat::Format_HSVF    )
        .value( "Format_VSHF",      eFormat::Format_VSHF    )
        .value( "Format_HSVAF",     eFormat::Format_HSVAF   )
        .value( "Format_AVSHF",     eFormat::Format_AVSHF   )
        .value( "Format_AHSVF",     eFormat::Format_AHSVF   )
        .value( "Format_VSHAF",     eFormat::Format_VSHAF   )
        .value( "Format_HSL8",      eFormat::Format_HSL8    )
        .value( "Format_LSH8",      eFormat::Format_LSH8    )
        .value( "Format_HSLA8",     eFormat::Format_HSLA8   )
        .value( "Format_ALSH8",     eFormat::Format_ALSH8   )
        .value( "Format_AHSL8",     eFormat::Format_AHSL8   )
        .value( "Format_LSHA8",     eFormat::Format_LSHA8   )
        .value( "Format_HSL16",     eFormat::Format_HSL16   )
        .value( "Format_LSH16",     eFormat::Format_LSH16   )
        .value( "Format_HSLA16",    eFormat::Format_HSLA16  )
        .value( "Format_ALSH16",    eFormat::Format_ALSH16  )
        .value( "Format_AHSL16",    eFormat::Format_AHSL16  )
        .value( "Format_LSHA16",    eFormat::Format_LSHA16  )
        .value( "Format_HSLF",      eFormat::Format_HSLF    )
        .value( "Format_LSHF",      eFormat::Format_LSHF    )
        .value( "Format_HSLAF",     eFormat::Format_HSLAF   )
        .value( "Format_ALSHF",     eFormat::Format_ALSHF   )
        .value( "Format_AHSLF",     eFormat::Format_AHSLF   )
        .value( "Format_LSHAF",     eFormat::Format_LSHAF   )
        .value( "Format_CMY8",      eFormat::Format_CMY8    )
        .value( "Format_YMC8",      eFormat::Format_YMC8    )
        .value( "Format_CMYA8",     eFormat::Format_CMYA8   )
        .value( "Format_AYMC8",     eFormat::Format_AYMC8   )
        .value( "Format_ACMY8",     eFormat::Format_ACMY8   )
        .value( "Format_YMCA8",     eFormat::Format_YMCA8   )
        .value( "Format_CMY16",     eFormat::Format_CMY16   )
        .value( "Format_YMC16",     eFormat::Format_YMC16   )
        .value( "Format_CMYA16",    eFormat::Format_CMYA16  )
        .value( "Format_AYMC16",    eFormat::Format_AYMC16  )
        .value( "Format_ACMY16",    eFormat::Format_ACMY16  )
        .value( "Format_YMCA16",    eFormat::Format_YMCA16  )
        .value( "Format_CMYF",      eFormat::Format_CMYF    )
        .value( "Format_YMCF",      eFormat::Format_YMCF    )
        .value( "Format_CMYAF",     eFormat::Format_CMYAF   )
        .value( "Format_AYMCF",     eFormat::Format_AYMCF   )
        .value( "Format_ACMYF",     eFormat::Format_ACMYF   )
        .value( "Format_YMCAF",     eFormat::Format_YMCAF   )
        .value( "Format_CMYK8",     eFormat::Format_CMYK8   )
        .value( "Format_KCMY8",     eFormat::Format_KCMY8   )
        .value( "Format_KYMC8",     eFormat::Format_KYMC8   )
        .value( "Format_YMCK8",     eFormat::Format_YMCK8   )
        .value( "Format_CMYKA8",    eFormat::Format_CMYKA8  )
        .value( "Format_ACMYK8",    eFormat::Format_ACMYK8  )
        .value( "Format_AKYMC8",    eFormat::Format_AKYMC8  )
        .value( "Format_KYMCA8",    eFormat::Format_KYMCA8  )
        .value( "Format_CMYK16",    eFormat::Format_CMYK16  )
        .value( "Format_KCMY16",    eFormat::Format_KCMY16  )
        .value( "Format_KYMC16",    eFormat::Format_KYMC16  )
        .value( "Format_YMCK16",    eFormat::Format_YMCK16  )
        .value( "Format_CMYKA16",   eFormat::Format_CMYKA16 )
        .value( "Format_ACMYK16",   eFormat::Format_ACMYK16 )
        .value( "Format_AKYMC16",   eFormat::Format_AKYMC16 )
        .value( "Format_KYMCA16",   eFormat::Format_KYMCA16 )
        .value( "Format_CMYKF",     eFormat::Format_CMYKF   )
        .value( "Format_KCMYF",     eFormat::Format_KCMYF   )
        .value( "Format_KYMCF",     eFormat::Format_KYMCF   )
        .value( "Format_YMCKF",     eFormat::Format_YMCKF   )
        .value( "Format_CMYKAF",    eFormat::Format_CMYKAF  )
        .value( "Format_ACMYKF",    eFormat::Format_ACMYKF  )
        .value( "Format_AKYMCF",    eFormat::Format_AKYMCF  )
        .value( "Format_KYMCAF",    eFormat::Format_KYMCAF  )
        .value( "Format_YUV8",      eFormat::Format_YUV8    )
        .value( "Format_VUY8",      eFormat::Format_VUY8    )
        .value( "Format_YUVA8",     eFormat::Format_YUVA8   )
        .value( "Format_AVUY8",     eFormat::Format_AVUY8   )
        .value( "Format_AYUV8",     eFormat::Format_AYUV8   )
        .value( "Format_VUYA8",     eFormat::Format_VUYA8   )
        .value( "Format_YUV16",     eFormat::Format_YUV16   )
        .value( "Format_VUY16",     eFormat::Format_VUY16   )
        .value( "Format_YUVA16",    eFormat::Format_YUVA16  )
        .value( "Format_AVUY16",    eFormat::Format_AVUY16  )
        .value( "Format_AYUV16",    eFormat::Format_AYUV16  )
        .value( "Format_VUYA16",    eFormat::Format_VUYA16  )
        .value( "Format_YUVF",      eFormat::Format_YUVF    )
        .value( "Format_VUYF",      eFormat::Format_VUYF    )
        .value( "Format_YUVAF",     eFormat::Format_YUVAF   )
        .value( "Format_AVUYF",     eFormat::Format_AVUYF   )
        .value( "Format_AYUVF",     eFormat::Format_AYUVF   )
        .value( "Format_VUYAF",     eFormat::Format_VUYAF   )
        .value( "Format_Lab8",      eFormat::Format_Lab8    )
        .value( "Format_baL8",      eFormat::Format_baL8    )
        .value( "Format_LabA8",     eFormat::Format_LabA8   )
        .value( "Format_AbaL8",     eFormat::Format_AbaL8   )
        .value( "Format_ALab8",     eFormat::Format_ALab8   )
        .value( "Format_baLA8",     eFormat::Format_baLA8   )
        .value( "Format_Lab16",     eFormat::Format_Lab16   )
        .value( "Format_baL16",     eFormat::Format_baL16   )
        .value( "Format_LabA16",    eFormat::Format_LabA16  )
        .value( "Format_AbaL16",    eFormat::Format_AbaL16  )
        .value( "Format_ALab16",    eFormat::Format_ALab16  )
        .value( "Format_baLA16",    eFormat::Format_baLA16  )
        .value( "Format_LabF",      eFormat::Format_LabF    )
        .value( "Format_baLF",      eFormat::Format_baLF    )
        .value( "Format_LabAF",     eFormat::Format_LabAF   )
        .value( "Format_AbaLF",     eFormat::Format_AbaLF   )
        .value( "Format_ALabF",     eFormat::Format_ALabF   )
        .value( "Format_baLAF",     eFormat::Format_baLAF   )
        .value( "Format_XYZ8",      eFormat::Format_XYZ8    )
        .value( "Format_ZYX8",      eFormat::Format_ZYX8    )
        .value( "Format_XYZA8",     eFormat::Format_XYZA8   )
        .value( "Format_AZYX8",     eFormat::Format_AZYX8   )
        .value( "Format_AXYZ8",     eFormat::Format_AXYZ8   )
        .value( "Format_ZYXA8",     eFormat::Format_ZYXA8   )
        .value( "Format_XYZ16",     eFormat::Format_XYZ16   )
        .value( "Format_ZYX16",     eFormat::Format_ZYX16   )
        .value( "Format_XYZA16",    eFormat::Format_XYZA16  )
        .value( "Format_AZYX16",    eFormat::Format_AZYX16  )
        .value( "Format_AXYZ16",    eFormat::Format_AXYZ16  )
        .value( "Format_ZYXA16",    eFormat::Format_ZYXA16  )
        .value( "Format_XYZF",      eFormat::Format_XYZF    )
        .value( "Format_ZYXF",      eFormat::Format_ZYXF    )
        .value( "Format_XYZAF",     eFormat::Format_XYZAF   )
        .value( "Format_AZYXF",     eFormat::Format_AZYXF   )
        .value( "Format_AXYZF",     eFormat::Format_AXYZF   )
        .value( "Format_ZYXAF",     eFormat::Format_ZYXAF   )
        .value( "Format_Yxy8",      eFormat::Format_Yxy8    )
        .value( "Format_yxY8",      eFormat::Format_yxY8    )
        .value( "Format_YxyA8",     eFormat::Format_YxyA8   )
        .value( "Format_AyxY8",     eFormat::Format_AyxY8   )
        .value( "Format_AYxy8",     eFormat::Format_AYxy8   )
        .value( "Format_yxYA8",     eFormat::Format_yxYA8   )
        .value( "Format_Yxy16",     eFormat::Format_Yxy16   )
        .value( "Format_yxY16",     eFormat::Format_yxY16   )
        .value( "Format_YxyA16",    eFormat::Format_YxyA16  )
        .value( "Format_AyxY16",    eFormat::Format_AyxY16  )
        .value( "Format_AYxy16",    eFormat::Format_AYxy16  )
        .value( "Format_yxYA16",    eFormat::Format_yxYA16  )
        .value( "Format_YxyF",      eFormat::Format_YxyF    )
        .value( "Format_yxYF",      eFormat::Format_yxYF    )
        .value( "Format_YxyAF",     eFormat::Format_YxyAF   )
        .value( "Format_AyxYF",     eFormat::Format_AyxYF   )
        .value( "Format_AYxyF",     eFormat::Format_AYxyF   )
        .value( "Format_yxYAF",     eFormat::Format_yxYAF   )
        .export_values();



    /////////
    // eBorderMode
    py::enum_< eBorderMode >( m, "eBorderMode" )
        .value( "Border_Transparent",   eBorderMode::Border_Transparent )
        .value( "Border_Constant",      eBorderMode::Border_Constant    )
        .value( "Border_Extend",        eBorderMode::Border_Extend      )
        .value( "Border_Wrap",          eBorderMode::Border_Wrap        )
        .export_values();



    /////////
    // eBlendMode
    py::enum_< eBlendMode >( m, "eBlendMode" )
        .value( "Blend_Normal",             eBlendMode::Blend_Normal            )
        .value( "Blend_Top",                eBlendMode::Blend_Top               )
        .value( "Blend_Back",               eBlendMode::Blend_Back              )
        .value( "Blend_Behind",             eBlendMode::Blend_Behind            )
        .value( "Blend_Dissolve",           eBlendMode::Blend_Dissolve          )
        .value( "Blend_BayerDither8x8",     eBlendMode::Blend_BayerDither8x8    )
        .value( "Blend_Darken",             eBlendMode::Blend_Darken            )
        .value( "Blend_Multiply",           eBlendMode::Blend_Multiply          )
        .value( "Blend_ColorBurn",          eBlendMode::Blend_ColorBurn         )
        .value( "Blend_LinearBurn",         eBlendMode::Blend_LinearBurn        )
        .value( "Blend_DarkerColor",        eBlendMode::Blend_DarkerColor       )
        .value( "Blend_Lighten",            eBlendMode::Blend_Lighten           )
        .value( "Blend_Screen",             eBlendMode::Blend_Screen            )
        .value( "Blend_ColorDodge",         eBlendMode::Blend_ColorDodge        )
        .value( "Blend_LinearDodge",        eBlendMode::Blend_LinearDodge       )
        .value( "Blend_LighterColor",       eBlendMode::Blend_LighterColor      )
        .value( "Blend_Overlay",            eBlendMode::Blend_Overlay           )
        .value( "Blend_SoftLight",          eBlendMode::Blend_SoftLight         )
        .value( "Blend_HardLight",          eBlendMode::Blend_HardLight         )
        .value( "Blend_VividLight",         eBlendMode::Blend_VividLight        )
        .value( "Blend_LinearLight",        eBlendMode::Blend_LinearLight       )
        .value( "Blend_PinLight",           eBlendMode::Blend_PinLight          )
        .value( "Blend_HardMix",            eBlendMode::Blend_HardMix           )
        .value( "Blend_Phoenix",            eBlendMode::Blend_Phoenix           )
        .value( "Blend_Reflect",            eBlendMode::Blend_Reflect           )
        .value( "Blend_Glow",               eBlendMode::Blend_Glow              )
        .value( "Blend_Difference",         eBlendMode::Blend_Difference        )
        .value( "Blend_Exclusion",          eBlendMode::Blend_Exclusion         )
        .value( "Blend_Add",                eBlendMode::Blend_Add               )
        .value( "Blend_Substract",          eBlendMode::Blend_Substract         )
        .value( "Blend_Divide",             eBlendMode::Blend_Divide            )
        .value( "Blend_Average",            eBlendMode::Blend_Average           )
        .value( "Blend_Negation",           eBlendMode::Blend_Negation          )
        .value( "Blend_Hue",                eBlendMode::Blend_Hue               )
        .value( "Blend_Saturation",         eBlendMode::Blend_Saturation        )
        .value( "Blend_Color",              eBlendMode::Blend_Color             )
        .value( "Blend_Luminosity",         eBlendMode::Blend_Luminosity        )
        .value( "Blend_PartialDerivative",  eBlendMode::Blend_PartialDerivative )
        .value( "Blend_Whiteout",           eBlendMode::Blend_Whiteout          )
        .value( "Blend_AngleCorrected",     eBlendMode::Blend_AngleCorrected    )
        .value( "NumBlendModes",            eBlendMode::NumBlendModes           )
        .export_values();



    /////////
    // eAlphaMode
    py::enum_< eAlphaMode >( m, "eAlphaMode" )
        .value( "Alpha_Normal",     eAlphaMode::Alpha_Normal    )
        .value( "Alpha_Erase",      eAlphaMode::Alpha_Erase     )
        .value( "Alpha_Top",        eAlphaMode::Alpha_Top       )
        .value( "Alpha_Back",       eAlphaMode::Alpha_Back      )
        .value( "Alpha_Sub",        eAlphaMode::Alpha_Sub       )
        .value( "Alpha_Add",        eAlphaMode::Alpha_Add       )
        .value( "Alpha_Mul",        eAlphaMode::Alpha_Mul       )
        .value( "Alpha_Min",        eAlphaMode::Alpha_Min       )
        .value( "Alpha_Max",        eAlphaMode::Alpha_Max       )
        .value( "NumAlphaModes",    eAlphaMode::NumAlphaModes   )
        .export_values();



    /////////
    // eBlendQualifier
    py::enum_< eBlendQualifier >( m, "eBlendQualifier" )
        .value( "BlendQualifier_Separable",     eBlendQualifier::BlendQualifier_Separable       )
        .value( "BlendQualifier_NonSeparable",  eBlendQualifier::BlendQualifier_NonSeparable    )
        .value( "BlendQualifier_Misc",          eBlendQualifier::BlendQualifier_Misc            )
        .export_values();



    /////////
    // eResamplingMethod
    py::enum_< eResamplingMethod >( m, "eResamplingMethod" )
        .value( "Resampling_NearestNeighbour",  eResamplingMethod::Resampling_NearestNeighbour  )
        .value( "Resampling_Bilinear",          eResamplingMethod::Resampling_Bilinear          )
        .value( "Resampling_Bicubic",           eResamplingMethod::Resampling_Bicubic           )
        .value( "Resampling_Area",              eResamplingMethod::Resampling_Area              )
        .export_values();



    /////////
    // eMipsLayout
    py::enum_< eMipsLayout >( m, "eMipsLayout" )
        .value( "MipsLayout_Standard",  eMipsLayout::MipsLayout_Standard    )
        .value( "MipsLayout_MipsOnly",  eMipsLayout::MipsLayout_MipsOnly    )
        .export_values();



    /////////
    // eFileFormat
    py::enum_< eFileFormat >( m, "eFileFormat" )
        .value( "FileFormat_png", eFileFormat::FileFormat_png )
        .value( "FileFormat_bmp", eFileFormat::FileFormat_bmp )
        .value( "FileFormat_tga", eFileFormat::FileFormat_tga )
        .value( "FileFormat_jpg", eFileFormat::FileFormat_jpg )
        .value( "FileFormat_hdr", eFileFormat::FileFormat_hdr )
        .export_values();



    /////////
    // eGradientType
    py::enum_< eGradientType >( m, "eGradientType" )
        .value( "Gradient_Linear",      eGradientType::Gradient_Linear      )
        .value( "Gradient_Radial",      eGradientType::Gradient_Radial      )
        .value( "Gradient_Angular",     eGradientType::Gradient_Angular     )
        .value( "Gradient_Reflected",   eGradientType::Gradient_Reflected   )
        .value( "Gradient_Diamond",     eGradientType::Gradient_Diamond     )
        .export_values();



    /////////
    // eMorphologicalElementValue
    py::enum_< eMorphologicalElementValue >( m, "eMorphologicalElementValue" )
        .value( "MpE_Zero",     eMorphologicalElementValue::MpE_Zero    )
        .value( "MpE_One",      eMorphologicalElementValue::MpE_One     )
        .value( "MpE_Any",      eMorphologicalElementValue::MpE_Any     )
        .export_values();



    /////////
    // eLayerType
    py::enum_< eLayerType >( m, "eLayerType" )
        .value( "Layer_Invalid",    eLayerType::Layer_Invalid   )
        .value( "Layer_Root",       eLayerType::Layer_Root      )
        .value( "Layer_Image",      eLayerType::Layer_Image     )
        .value( "Layer_Folder",     eLayerType::Layer_Folder    )
        .value( "Layer_Text",       eLayerType::Layer_Text      )
        .value( "Layer_Vector",     eLayerType::Layer_Vector    )
        .value( "Layer_FX",         eLayerType::Layer_FX        )
        .value( "Layer_Mask",       eLayerType::Layer_Mask      )
        .export_values();



    /////////
    // eEventStatus
    py::enum_< eEventStatus >( m, "eEventStatus" )
        .value( "EventStatus_Idle",     eEventStatus::EventStatus_Idle      )
        .value( "EventStatus_Queued",   eEventStatus::EventStatus_Queued    )
        .value( "EventStatus_Finished", eEventStatus::EventStatus_Finished  )
        .export_values();



    /////////
    // eScheduleTimePolicy
    py::enum_< eScheduleTimePolicy >( m, "eScheduleTimePolicy" )
        .value( "ScheduleTime_Sync",    eScheduleTimePolicy::ScheduleTime_Sync  )
        .value( "ScheduleTime_Async",   eScheduleTimePolicy::ScheduleTime_Async )
        .export_values();



    /////////
    // eScheduleRunPolicy
    py::enum_< eScheduleRunPolicy >( m, "eScheduleRunPolicy" )
        .value( "ScheduleRun_Mono",     eScheduleRunPolicy::ScheduleRun_Mono    )
        .value( "ScheduleRun_Multi",    eScheduleRunPolicy::ScheduleRun_Multi   )
        .export_values();


    /////////
    // eScheduleModePolicy
    py::enum_< eScheduleModePolicy >( m, "eScheduleModePolicy" )
        .value( "ScheduleMode_Scanlines",   eScheduleModePolicy::ScheduleMode_Scanlines )
        .value( "ScheduleMode_Chunks",      eScheduleModePolicy::ScheduleMode_Chunks    )
        .export_values();



    /////////
    // eScheduleParameterPolicy
    py::enum_< eScheduleParameterPolicy >( m, "eScheduleParameterPolicy" )
        .value( "ScheduleParameter_Count",  eScheduleParameterPolicy::ScheduleParameter_Count   )
        .value( "ScheduleParameter_Length", eScheduleParameterPolicy::ScheduleParameter_Length  )
        .export_values();


    /////////
    // ePerformanceIntent
    py::enum_< ePerformanceIntent >( m, "ePerformanceIntent" )
        .value( "PerformanceIntent_MEM", ePerformanceIntent::PerformanceIntent_MEM )
        .value( "PerformanceIntent_SSE", ePerformanceIntent::PerformanceIntent_SSE )
        .value( "PerformanceIntent_AVX", ePerformanceIntent::PerformanceIntent_AVX )
        .value( "PerformanceIntent_Max", ePerformanceIntent::PerformanceIntent_Max )
        .export_values();



    /////////
    // FFormatMetrics
    py::class_< FFormatMetrics >( m, "FFormatMetrics" )
        .def( py::init< eFormat >(), "format"_a )
        .def( py::init< const FFormatMetrics& >(), "other"_a )
        .def( "ReinterpretedType", &FFormatMetrics::ReinterpretedType )
        .def_readonly( "IDT", &FFormatMetrics::IDT )
        .def_readonly( "FMT", &FFormatMetrics::FMT )
        .def_readonly( "TP", &FFormatMetrics::TP )
        .def_readonly( "CM", &FFormatMetrics::CM )
        .def_readonly( "BPC", &FFormatMetrics::BPC )
        .def_readonly( "NCC", &FFormatMetrics::NCC )
        .def_readonly( "HEA", &FFormatMetrics::HEA )
        .def_readonly( "RSC", &FFormatMetrics::RSC )
        .def_readonly( "SPP", &FFormatMetrics::SPP )
        .def_readonly( "BPP", &FFormatMetrics::BPP )
        .def_readonly( "AID", &FFormatMetrics::AID )
        .def_readonly( "REV", &FFormatMetrics::REV )
        .def_readonly( "SWA", &FFormatMetrics::SWA )
        .def_readonly( "PRE", &FFormatMetrics::PRE )
        .def_readonly( "LIN", &FFormatMetrics::LIN )
        .def_readonly( "PRO", &FFormatMetrics::PRO )
        .def_readonly( "PLA", &FFormatMetrics::PLA )
        .def(
              "RedirectedIndex"
            , []( const FFormatMetrics& self, int index ) {
                return  self.IDT[ index ];
            }
        );



    /////////
    // FColorSpace
    py::class_< FColorSpace >( m, "FColorSpace" )
        .def( py::init<>() );



    /////////
    // FOnInvalidBlock
    py::class_< FOnInvalidBlock >( m, "FOnInvalidBlock" )
        .def( py::init<>() )
        .def( py::init< FOnInvalidBlock::tFptr, void* >(), "fptr"_a, "info"_a = nullptr )
        .def( "ExecuteIfBound", &FOnInvalidBlock::ExecuteIfBound )
        .def( "Execute", &FOnInvalidBlock::Execute );



    /////////
    // FOnCleanupData
    py::class_< FOnCleanupData >( m, "FOnCleanupData" )
        .def( py::init<>() )
        .def( py::init< FOnCleanupData::tFptr, void* >(), "fptr"_a, "info"_a = nullptr )
        .def( "ExecuteIfBound", &FOnCleanupData::ExecuteIfBound )
        .def( "Execute", &FOnCleanupData::Execute )
        .def_static(
              "OnCleanupFreeMemory"
            , []() {
                return  FOnCleanupData( &OnCleanup_FreeMemory );
            }
        );



    /////////
    // FOnEventComplete
    py::class_< FOnEventComplete >( m, "FOnEventComplete" )
        .def( py::init<>() )
        .def( py::init< FOnEventComplete::tFptr >(), "fptr"_a )
        // Lambda wrapper to authorize creating an FOnEventComplete from a
        // python function, wrapped into a secondary lambda.
        // Don't care about GIL ( global interpreter lock ) for now.
        .def(
            py::init<>(
                []( py::function func ) {
                    return  FOnEventComplete(
                        [ func ]( const FRectI& rect ) {
                            func( rect );
                        }
                    );
                }
            )
            , "fptr"_a
        )
        .def( "ExecuteIfBound", &FOnEventComplete::ExecuteIfBound )
        .def( "Execute", &FOnEventComplete::Execute );



    /////////
    // FEvent
    py::class_< FEvent >( m, "FEvent" )
        .def( py::init<>() )
        .def( py::init< const FOnEventComplete& >(), "onComplete"_a = FOnEventComplete() )
        .def( "Status", &FEvent::Status )
        .def( "Wait", &FEvent::Wait );



    /////////
    // FFilePathRegistry
    py::class_< FFilePathRegistry >( m, "FFilePathRegistry" )
        .def( py::init<>() )
        .def( "LookupPaths", &FFilePathRegistry::LookupPaths )
        .def( "Filters", &FFilePathRegistry::Filters )
        .def( "Records", &FFilePathRegistry::Records )
        .def( "AddLookupPath", &FFilePathRegistry::AddLookupPath )
        .def( "AddLookupPaths", &FFilePathRegistry::AddLookupPaths )
        .def( "AddFilter", &FFilePathRegistry::AddFilter )
        .def( "AddFilters", &FFilePathRegistry::AddFilters )
        .def( "Parse", &FFilePathRegistry::Parse )
        .def( "FilePathForExactMatchingName", &FFilePathRegistry::FilePathForExactMatchingName )
        .def( "FilePathForClosestMatchingName", &FFilePathRegistry::FilePathForClosestMatchingName );



    /////////
    // FThreadPool
    py::class_< FThreadPool >( m, "FThreadPool" )
        .def( py::init< uint32 >(), "workers"_a = FThreadPool::MaxWorkers() )
        .def( "WaitForCompletion", &FThreadPool::WaitForCompletion )
        .def( "SetNumWorkers", &FThreadPool::SetNumWorkers )
        .def( "GetNumWorkers", &FThreadPool::GetNumWorkers )
        .def_static( "MaxWorkers", &FThreadPool::MaxWorkers );



    /////////
    // FCommandQueue
    py::class_< FCommandQueue >( m, "FCommandQueue" )
        .def( py::init< FThreadPool& >(), "pool"_a )
        .def( "Flush", &FCommandQueue::Flush )
        .def( "Finish", &FCommandQueue::Finish )
        .def( "Fence", &FCommandQueue::Fence );



    /////////
    // FSchedulePolicy
    py::class_< FSchedulePolicy >( m, "FSchedulePolicy" )
        .def(
            py::init<
                  eScheduleTimePolicy
                , eScheduleRunPolicy
                , eScheduleModePolicy
                , eScheduleParameterPolicy
                , ULIS::int64
            >
            ()
            , "time"_a  = eScheduleTimePolicy::ScheduleTime_Sync
            , "run"_a   = eScheduleRunPolicy::ScheduleRun_Multi
            , "mode"_a  = eScheduleModePolicy::ScheduleMode_Scanlines
            , "param"_a = eScheduleParameterPolicy::ScheduleParameter_Count
            , "value"_a = -1
        )
        .def( "TimePolicy", &FSchedulePolicy::TimePolicy )
        .def( "RunPolicy", &FSchedulePolicy::RunPolicy )
        .def( "ModePolicy", &FSchedulePolicy::ModePolicy )
        .def( "ParameterPolicy", &FSchedulePolicy::ParameterPolicy )
        .def( "Value", &FSchedulePolicy::Value )
        .def_readonly_static( "AsyncCacheEfficient",    &FSchedulePolicy::AsyncCacheEfficient   )
        .def_readonly_static( "CacheEfficient",         &FSchedulePolicy::CacheEfficient        )
        .def_readonly_static( "AsyncMonoChunk",         &FSchedulePolicy::AsyncMonoChunk        )
        .def_readonly_static( "MonoChunk",              &FSchedulePolicy::MonoChunk             )
        .def_readonly_static( "AsyncMultiScanlines",    &FSchedulePolicy::AsyncMultiScanlines   )
        .def_readonly_static( "AsyncMonoScanlines",     &FSchedulePolicy::AsyncMonoScanlines    )
        .def_readonly_static( "MultiScanlines",         &FSchedulePolicy::MultiScanlines        )
        .def_readonly_static( "MonoScanlines",          &FSchedulePolicy::MonoScanlines         );



    /////////
    // FHardwareMetrics
    py::class_< FHardwareMetrics >( m, "FHardwareMetrics" )
        .def( py::init<>() )
        .def( "IsHardwareAMD",          &FHardwareMetrics::IsHardwareAMD            )
        .def( "IsHardwareIntel",        &FHardwareMetrics::IsHardwareIntel          )
        .def( "IsOSx64",                &FHardwareMetrics::IsOSx64                  )
        .def( "HasOSAVX",               &FHardwareMetrics::HasOSAVX                 )
        .def( "HasOSAVX512",            &FHardwareMetrics::HasOSAVX512              )
        .def( "HasHardwarex64",         &FHardwareMetrics::HasHardwarex64           )
        .def( "HasHardwareMMX",         &FHardwareMetrics::HasHardwareMMX           )
        .def( "HasHardwareABM",         &FHardwareMetrics::HasHardwareABM           )
        .def( "HasHardwareRDRAND",      &FHardwareMetrics::HasHardwareRDRAND        )
        .def( "HasHardwareBMI1",        &FHardwareMetrics::HasHardwareBMI1          )
        .def( "HasHardwareBMI2",        &FHardwareMetrics::HasHardwareBMI2          )
        .def( "HasHardwareADX",         &FHardwareMetrics::HasHardwareADX           )
        .def( "HasHardwarePREFETCHWT1", &FHardwareMetrics::HasHardwarePREFETCHWT1   )
        .def( "HasHardwareMPX",         &FHardwareMetrics::HasHardwareMPX           )
        .def( "HasHardwareSSE",         &FHardwareMetrics::HasHardwareSSE           )
        .def( "HasHardwareSSE2",        &FHardwareMetrics::HasHardwareSSE2          )
        .def( "HasHardwareSSE3",        &FHardwareMetrics::HasHardwareSSE3          )
        .def( "HasHardwareSSSE3",       &FHardwareMetrics::HasHardwareSSSE3         )
        .def( "HasHardwareSSE41",       &FHardwareMetrics::HasHardwareSSE41         )
        .def( "HasHardwareSSE42",       &FHardwareMetrics::HasHardwareSSE42         )
        .def( "HasHardwareSSE4a",       &FHardwareMetrics::HasHardwareSSE4a         )
        .def( "HasHardwareAES",         &FHardwareMetrics::HasHardwareAES           )
        .def( "HasHardwareSHA",         &FHardwareMetrics::HasHardwareSHA           )
        .def( "HasHardwareAVX",         &FHardwareMetrics::HasHardwareAVX           )
        .def( "HasHardwareXOP",         &FHardwareMetrics::HasHardwareXOP           )
        .def( "HasHardwareFMA3",        &FHardwareMetrics::HasHardwareFMA3          )
        .def( "HasHardwareFMA4",        &FHardwareMetrics::HasHardwareFMA4          )
        .def( "HasHardwareAVX2",        &FHardwareMetrics::HasHardwareAVX2          )
        .def( "HasHardwareAVX512_F",    &FHardwareMetrics::HasHardwareAVX512_F      )
        .def( "HasHardwareAVX512_PF",   &FHardwareMetrics::HasHardwareAVX512_PF     )
        .def( "HasHardwareAVX512_ER",   &FHardwareMetrics::HasHardwareAVX512_ER     )
        .def( "HasHardwareAVX512_CD",   &FHardwareMetrics::HasHardwareAVX512_CD     )
        .def( "HasHardwareAVX512_VL",   &FHardwareMetrics::HasHardwareAVX512_VL     )
        .def( "HasHardwareAVX512_BW",   &FHardwareMetrics::HasHardwareAVX512_BW     )
        .def( "HasHardwareAVX512_DQ",   &FHardwareMetrics::HasHardwareAVX512_DQ     )
        .def( "HasHardwareAVX512_IFMA", &FHardwareMetrics::HasHardwareAVX512_IFMA   )
        .def( "HasHardwareAVX512_VBMI", &FHardwareMetrics::HasHardwareAVX512_VBMI   )
        .def( "MaxWorkers",             &FHardwareMetrics::MaxWorkers               )
        .def( "L1CacheSize",            &FHardwareMetrics::L1CacheSize              )
        .def( "L1CacheLineSize",        &FHardwareMetrics::L1CacheLineSize          )
        .def( "Field",                  &FHardwareMetrics::Field                    );



    /////////
    // Vec Utils
    #define PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTION( CLASS, E0, E1 )            .def( ULIS_STRINGIFY( E0 ## E1 ), & CLASS :: E0 ## E1 )
    #define PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTION( CLASS, E0, E1, E2 )        .def( ULIS_STRINGIFY( E0 ## E1 ## E2 ), & CLASS :: E0 ## E1 ## E2 )
    #define PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTION( CLASS, E0, E1, E2, E3 )    .def( ULIS_STRINGIFY( E0 ## E1 ## E2 ## E3 ), & CLASS :: E0 ## E1 ## E2 ## E3 )
    #define PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS ) ULIS_FOR_ALL_VEC2_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS, PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTION )
    #define PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS ) ULIS_FOR_ALL_VEC2_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS, PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTION )
    #define PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS ) ULIS_FOR_ALL_VEC2_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS, PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTION )
    #define PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS ) ULIS_FOR_ALL_VEC3_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS, PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTION )
    #define PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS ) ULIS_FOR_ALL_VEC3_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS, PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTION )
    #define PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS ) ULIS_FOR_ALL_VEC3_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS, PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTION )
    #define PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS ) ULIS_FOR_ALL_VEC4_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS, PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTION )
    #define PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS ) ULIS_FOR_ALL_VEC4_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS, PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTION )
    #define PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS ) ULIS_FOR_ALL_VEC4_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS, PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTION )
    #define PYULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC2( CLASS )   \
    PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS )       \
    PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS )       \
    PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS )
    #define PYULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC3( CLASS )   \
    PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS )       \
    PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS )       \
    PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS )
    #define PYULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC4( CLASS )   \
    PYULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS )       \
    PYULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS )       \
    PYULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS )



    /////////
    // FVec2I
    py::class_< FVec2I >( m, "FVec2I" )
        .def( py::init<>() )
        .def( py::init< int >(), "value"_a )
        .def( py::init< int, int >(), "x"_a, "y"_a )
        .def( py::init< const FVec2I& >(), "vec"_a )
        .def( py::init< const FVec3I& >(), "vec"_a )
        .def( py::init< const FVec4I& >(), "vec"_a )
        .def( py::init< const FVec2F& >(), "vec"_a )
        .def( py::init< const FVec3F& >(), "vec"_a )
        .def( py::init< const FVec4F& >(), "vec"_a )
        .def( "Distance", &FVec2I::Distance )
        .def( "DistanceSquared", &FVec2I::DistanceSquared )
        .def( "ManhattanDistance", &FVec2I::ManhattanDistance )
        .def( "DotProduct", &FVec2I::DotProduct )
        .def( "Normalize", &FVec2I::Normalize )
        .def( "Normalized", &FVec2I::Normalized )
        .def( "DecimalPart", &FVec2I::DecimalPart )
        .def( py::self += int() )
        .def( py::self -= int() )
        .def( py::self *= int() )
        .def( py::self /= int() )
        .def( py::self += py::self )
        .def( py::self -= py::self )
        .def( py::self *= py::self )
        .def( py::self /= py::self )
        .def( py::self + int() )
        .def( py::self - int() )
        .def( py::self * int() )
        .def( py::self / int() )
        .def( py::self + py::self )
        .def( py::self - py::self )
        .def( py::self * py::self )
        .def( py::self / py::self )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( -py::self )
        PYULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC2( FVec2I )
        .def_readwrite( "x", &FVec2I::x )
        .def_readwrite( "y", &FVec2I::y );



    /////////
    // FVec3I
    py::class_< FVec3I >( m, "FVec3I" )
        .def( py::init<>() )
        .def( py::init< int >(), "value"_a )
        .def( py::init< int, int, int >(), "x"_a, "y"_a, "z"_a = 0 )
        .def( py::init< const FVec2I&, int >(), "vec"_a, "z"_a = 0 )
        .def( py::init< const FVec3I& >(), "vec"_a )
        .def( py::init< const FVec4I& >(), "vec"_a )
        .def( py::init< const FVec2F&, float >(), "vec"_a, "z"_a = 0.f )
        .def( py::init< const FVec3F& >(), "vec"_a )
        .def( py::init< const FVec4F& >(), "vec"_a )
        .def( "Distance", &FVec3I::Distance )
        .def( "DistanceSquared", &FVec3I::DistanceSquared )
        .def( "ManhattanDistance", &FVec3I::ManhattanDistance )
        .def( "DotProduct", &FVec3I::DotProduct )
        .def( "Normalize", &FVec3I::Normalize )
        .def( "Normalized", &FVec3I::Normalized )
        .def( py::self += int() )
        .def( py::self -= int() )
        .def( py::self *= int() )
        .def( py::self /= int() )
        .def( py::self += py::self )
        .def( py::self -= py::self )
        .def( py::self *= py::self )
        .def( py::self /= py::self )
        .def( py::self + int() )
        .def( py::self - int() )
        .def( py::self * int() )
        .def( py::self / int() )
        .def( py::self + py::self )
        .def( py::self - py::self )
        .def( py::self * py::self )
        .def( py::self / py::self )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( -py::self )
        PYULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC3( FVec3I )
        .def_readwrite( "x", &FVec3I::x )
        .def_readwrite( "y", &FVec3I::y )
        .def_readwrite( "z", &FVec3I::z );



    /////////
    // FVec4I
    py::class_< FVec4I >( m, "FVec4I" )
        .def( py::init<>() )
        .def( py::init< int >(), "value"_a )
        .def( py::init< int, int, int, int >(), "x"_a, "y"_a, "z"_a = 0, "w"_a = 0 )
        .def( py::init< const FVec2I&, int, int >(), "vec"_a, "z"_a = 0, "w"_a = 0 )
        .def( py::init< const FVec3I&, int >(), "vec"_a, "w"_a = 0 )
        .def( py::init< const FVec4I& >(), "vec"_a )
        .def( py::init< const FVec2F&, float, float >(), "vec"_a, "z"_a = 0.f, "w"_a = 0.f )
        .def( py::init< const FVec3F&, float >(), "vec"_a, "w"_a = 0.f )
        .def( py::init< const FVec4F& >(), "vec"_a )
        .def( "Distance", &FVec4I::Distance )
        .def( "DistanceSquared", &FVec4I::DistanceSquared )
        .def( "ManhattanDistance", &FVec4I::ManhattanDistance )
        .def( "DotProduct", &FVec4I::DotProduct )
        .def( "Normalize", &FVec4I::Normalize )
        .def( "Normalized", &FVec4I::Normalized )
        .def( py::self += int() )
        .def( py::self -= int() )
        .def( py::self *= int() )
        .def( py::self /= int() )
        .def( py::self += py::self )
        .def( py::self -= py::self )
        .def( py::self *= py::self )
        .def( py::self /= py::self )
        .def( py::self + int() )
        .def( py::self - int() )
        .def( py::self * int() )
        .def( py::self / int() )
        .def( py::self + py::self )
        .def( py::self - py::self )
        .def( py::self * py::self )
        .def( py::self / py::self )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( -py::self )
        PYULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC4( FVec4I )
        .def_readwrite( "x", &FVec4I::x )
        .def_readwrite( "y", &FVec4I::y )
        .def_readwrite( "z", &FVec4I::z )
        .def_readwrite( "w", &FVec4I::w );



    /////////
    // FVec2F
    py::class_< FVec2F >( m, "FVec2F" )
        .def( py::init<>() )
        .def( py::init< float >(), "value"_a )
        .def( py::init< float, float >(), "x"_a, "y"_a )
        .def( py::init< const FVec2F& >(), "vec"_a )
        .def( py::init< const FVec3F& >(), "vec"_a )
        .def( py::init< const FVec4F& >(), "vec"_a )
        .def( py::init< const FVec2I& >(), "vec"_a )
        .def( py::init< const FVec3I& >(), "vec"_a )
        .def( py::init< const FVec4I& >(), "vec"_a )
        .def( "Distance", &FVec2F::Distance )
        .def( "DistanceSquared", &FVec2F::DistanceSquared )
        .def( "ManhattanDistance", &FVec2F::ManhattanDistance )
        .def( "DotProduct", &FVec2F::DotProduct )
        .def( "Normalize", &FVec2F::Normalize )
        .def( "Normalized", &FVec2F::Normalized )
        .def( "DecimalPart", &FVec2F::DecimalPart )
        .def( py::self += float() )
        .def( py::self -= float() )
        .def( py::self *= float() )
        .def( py::self /= float() )
        .def( py::self += py::self )
        .def( py::self -= py::self )
        .def( py::self *= py::self )
        .def( py::self /= py::self )
        .def( py::self + float() )
        .def( py::self - float() )
        .def( py::self * float() )
        .def( py::self / float() )
        .def( py::self + py::self )
        .def( py::self - py::self )
        .def( py::self * py::self )
        .def( py::self / py::self )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( -py::self )
        PYULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC2( FVec2F )
        .def_readwrite( "x", &FVec2F::x )
        .def_readwrite( "y", &FVec2F::y );



    /////////
    // FVec3F
    py::class_< FVec3F >( m, "FVec3F" )
        .def( py::init<>() )
        .def( py::init< float >(), "value"_a )
        .def( py::init< float, float, float >(), "x"_a, "y"_a, "z"_a = 0.f )
        .def( py::init< const FVec2F&, float >(), "vec"_a, "z"_a = 0.f )
        .def( py::init< const FVec3F& >(), "vec"_a )
        .def( py::init< const FVec4F& >(), "vec"_a )
        .def( py::init< const FVec2I&, int >(), "vec"_a, "z"_a = 0 )
        .def( py::init< const FVec3I& >(), "vec"_a )
        .def( py::init< const FVec4I& >(), "vec"_a )
        .def( "Distance", &FVec3F::Distance )
        .def( "DistanceSquared", &FVec3F::DistanceSquared )
        .def( "ManhattanDistance", &FVec3F::ManhattanDistance )
        .def( "DotProduct", &FVec3F::DotProduct )
        .def( "Normalize", &FVec3F::Normalize )
        .def( "Normalized", &FVec3F::Normalized )
        .def( py::self += float() )
        .def( py::self -= float() )
        .def( py::self *= float() )
        .def( py::self /= float() )
        .def( py::self += py::self )
        .def( py::self -= py::self )
        .def( py::self *= py::self )
        .def( py::self /= py::self )
        .def( py::self + float() )
        .def( py::self - float() )
        .def( py::self * float() )
        .def( py::self / float() )
        .def( py::self + py::self )
        .def( py::self - py::self )
        .def( py::self * py::self )
        .def( py::self / py::self )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( -py::self )
        PYULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC3( FVec3F )
        .def_readwrite( "x", &FVec3F::x )
        .def_readwrite( "y", &FVec3F::y )
        .def_readwrite( "z", &FVec3F::z );



    /////////
    // FVec4F
    py::class_< FVec4F >( m, "FVec4F" )
        .def( py::init<>() )
        .def( py::init< float >(), "value"_a )
        .def( py::init< float, float, float, float >(), "x"_a, "y"_a, "z"_a = 0.f, "w"_a = 0.f )
        .def( py::init< const FVec2F&, float, float >(), "vec"_a, "z"_a = 0.f, "w"_a = 0.f )
        .def( py::init< const FVec3F&, float >(), "vec"_a, "w"_a = 0.f )
        .def( py::init< const FVec4F& >(), "vec"_a )
        .def( py::init< const FVec2I&, int, int >(), "vec"_a, "z"_a = 0, "w"_a = 0 )
        .def( py::init< const FVec3I&, int >(), "vec"_a, "w"_a = 0 )
        .def( py::init< const FVec4I& >(), "vec"_a )
        .def( "Distance", &FVec4F::Distance )
        .def( "DistanceSquared", &FVec4F::DistanceSquared )
        .def( "ManhattanDistance", &FVec4F::ManhattanDistance )
        .def( "DotProduct", &FVec4F::DotProduct )
        .def( "Normalize", &FVec4F::Normalize )
        .def( "Normalized", &FVec4F::Normalized )
        .def( py::self += float() )
        .def( py::self -= float() )
        .def( py::self *= float() )
        .def( py::self /= float() )
        .def( py::self += py::self )
        .def( py::self -= py::self )
        .def( py::self *= py::self )
        .def( py::self /= py::self )
        .def( py::self + float() )
        .def( py::self - float() )
        .def( py::self * float() )
        .def( py::self / float() )
        .def( py::self + py::self )
        .def( py::self - py::self )
        .def( py::self * py::self )
        .def( py::self / py::self )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( -py::self )
        PYULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC4( FVec4F )
        .def_readwrite( "x", &FVec4F::x )
        .def_readwrite( "y", &FVec4F::y )
        .def_readwrite( "z", &FVec4F::z )
        .def_readwrite( "w", &FVec4F::w );



    /////////
    // FVec4F
    py::class_< FMat3F >( m, "FMat3F" )
        .def( py::init<>() )
        .def( py::init< float >(), "value"_a )
        .def(
            py::init<
                  float, float, float
                , float, float, float
                , float, float, float
            >
            ()
            , "m00"_a, "m01"_a, "m02"_a
            , "m10"_a, "m11"_a, "m02"_a
            , "m20"_a, "m21"_a, "m02"_a
        )
        .def( py::init< const FVec3F&, const FVec3F&, const FVec3F& >(), "col0"_a, "col1"_a, "col2"_a )
        .def( py::init< const FMat3F& >(), "other"_a )
        .def( "Data", static_cast< float* ( FMat3F::* )() >( &FMat3F::Data ) )
        .def( "Inverse", &FMat3F::Inverse )
        .def( "Transpose", &FMat3F::Transpose )
        .def( "Determinant", &FMat3F::Determinant )
        .def( "Decompose", &FMat3F::Decompose )
        .def( "ApplyHomography", &FMat3F::ApplyHomography )
        .def( "MakeRotationMatrix", &FMat3F::MakeRotationMatrix )
        .def( "MakeScaleMatrix", &FMat3F::MakeScaleMatrix )
        .def( "MakeSkewMatrix", &FMat3F::MakeSkewMatrix )
        .def( "MakeTranslationMatrix", &FMat3F::MakeTranslationMatrix )
        .def( "MakeHomography", &FMat3F::MakeHomography )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( -py::self )
        .def( py::self += float() )
        .def( py::self -= float() )
        .def( py::self *= float() )
        .def( py::self /= float() )
        .def( py::self += py::self )
        .def( py::self -= py::self )
        .def( py::self *= py::self )
        .def( py::self /= py::self )
        .def( py::self + float() )
        .def( py::self - float() )
        .def( py::self * float() )
        .def( py::self / float() )
        .def( float() + py::self )
        .def( float() - py::self )
        .def( float() * py::self )
        .def( float() / py::self )
        .def( py::self + py::self )
        .def( py::self - py::self )
        .def( py::self * py::self )
        .def( py::self / py::self )
        .def( py::self * FVec3F() )
        .def( FVec3F() * py::self );



    /////////
    // FRectI
    py::class_< FRectI >( m, "FRectI" )
        .def( py::init<>() )
        .def( py::init< int, int, int, int >(), "x"_a, "y"_a, "w"_a, "h"_a )
        .def_static( "FromXYWH", &FRectI::FromXYWH )
        .def_static( "FromMinMax", &FRectI::FromMinMax )
        .def_static( "FromPositionAndSize", &FRectI::FromPositionAndSize )
        .def( "HitTest", &FRectI::HitTest )
        .def( "HitTest", &FRectI::InVerticalRange )
        .def( "HitTest", &FRectI::InHorizontalRange )
        .def( py::self & py::self )
        .def( py::self | py::self )
        .def( py::self - py::self )
        .def( "Exclusion", &FRectI::Exclusion )
        .def( "UnionLeaveEmpty", &FRectI::UnionLeaveEmpty )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( "Area", &FRectI::Area )
        .def( "Sanitize", &FRectI::Sanitize )
        .def( "Sanitized", &FRectI::Sanitized )
        .def( "TransformAffine", &FRectI::TransformAffine )
        .def( "TransformPerspective", &FRectI::TransformPerspective )
        .def( "TransformedAffine", &FRectI::TransformedAffine )
        .def( "TransformedPerspective", &FRectI::TransformedPerspective )
        .def( "Shift", &FRectI::Shift )
        .def( "Position", &FRectI::Position )
        .def( "Size", &FRectI::Size )
        .def_readonly_static( "Auto", &FRectI::Auto )
        .def_readwrite( "x", &FRectI::x )
        .def_readwrite( "y", &FRectI::y )
        .def_readwrite( "h", &FRectI::h )
        .def_readwrite( "w", &FRectI::w );



    /////////
    // FRectF
    py::class_< FRectF >( m, "FRectF" )
        .def( py::init<>() )
        .def( py::init< float, float, float, float >(), "x"_a, "y"_a, "w"_a, "h"_a )
        .def_static( "FromXYWH", &FRectF::FromXYWH )
        .def_static( "FromMinMax", &FRectF::FromMinMax )
        .def_static( "FromPositionAndSize", &FRectF::FromPositionAndSize )
        .def( "HitTest", &FRectF::HitTest )
        .def( "HitTest", &FRectF::InVerticalRange )
        .def( "HitTest", &FRectF::InHorizontalRange )
        .def( py::self & py::self )
        .def( py::self | py::self )
        .def( py::self - py::self )
        .def( "Exclusion", &FRectF::Exclusion )
        .def( "UnionLeaveEmpty", &FRectF::UnionLeaveEmpty )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( "Area", &FRectF::Area )
        .def( "Sanitize", &FRectF::Sanitize )
        .def( "Sanitized", &FRectF::Sanitized )
        .def( "TransformAffine", &FRectF::TransformAffine )
        .def( "TransformPerspective", &FRectF::TransformPerspective )
        .def( "TransformedAffine", &FRectF::TransformedAffine )
        .def( "TransformedPerspective", &FRectF::TransformedPerspective )
        .def( "Shift", &FRectF::Shift )
        .def( "Position", &FRectF::Position )
        .def( "Size", &FRectF::Size )
        .def_readonly_static( "Auto", &FRectF::Auto )
        .def_readwrite( "x", &FRectF::x )
        .def_readwrite( "y", &FRectF::y )
        .def_readwrite( "h", &FRectF::h )
        .def_readwrite( "w", &FRectF::w );



    /////////
    // IHasFormat
    py::class_< IHasFormat >( m, "IHasFormat" )
        .def( "FormatMetrics", &IHasFormat::FormatMetrics )
        .def( "IndexTable", &IHasFormat::IndexTable )
        .def( "Format", &IHasFormat::Format )
        .def( "Type", &IHasFormat::Type )
        .def( "Model", &IHasFormat::Model )
        .def( "BytesPerSample", &IHasFormat::BytesPerSample )
        .def( "NumColorChannels", &IHasFormat::NumColorChannels )
        .def( "HasAlpha", &IHasFormat::HasAlpha )
        .def( "SamplesPerPixel", &IHasFormat::SamplesPerPixel )
        .def( "BytesPerPixel", &IHasFormat::BytesPerPixel )
        .def( "AlphaIndex", &IHasFormat::AlphaIndex )
        .def( "Reversed", &IHasFormat::Reversed )
        .def( "Swapped", &IHasFormat::Swapped )
        .def( "Premultiplied", &IHasFormat::Premultiplied )
        .def( "Linear", &IHasFormat::Linear )
        .def( "DefaultProfileCode", &IHasFormat::DefaultProfileCode )
        .def( "RedirectedIndex", &IHasFormat::RedirectedIndex );



    /////////
    // IHasFormat
    py::class_< IHasColorSpace >( m, "IHasColorSpace" )
        .def( "AssignColorSpace", &IHasColorSpace::AssignColorSpace )
        .def( "ColorSpace", &IHasColorSpace::ColorSpace );



    /////////
    // ISample
    py::class_< ISample, IHasFormat, IHasColorSpace >( m, "ISample" )
        .def( py::self == py::self )
        .def( py::self != py::self )
        .def( "Bits", static_cast< uint8* ( ISample::* )() >( &ISample::Bits ) )
        .def( "Channel8", &ISample::Channel8 )
        .def( "Channel16", &ISample::Channel16 )
        .def( "ChannelF", &ISample::ChannelF )
        .def( "SetChannel8", &ISample::SetChannel8 )
        .def( "SetChannel16", &ISample::SetChannel16 )
        .def( "SetChannelF", &ISample::SetChannelF )
        .def( "Alpha8", &ISample::Alpha8 )
        .def( "Alpha16", &ISample::Alpha16 )
        .def( "AlphaF", &ISample::AlphaF )
        .def( "A8", &ISample::A8 )
        .def( "A16", &ISample::A16 )
        .def( "AF", &ISample::AF )
        .def( "SetAlpha8", &ISample::SetAlpha8 )
        .def( "SetAlpha16", &ISample::SetAlpha16 )
        .def( "SetAlphaF", &ISample::SetAlphaF )
        .def( "SetA8", &ISample::SetA8 )
        .def( "SetA16", &ISample::SetA16 )
        .def( "SetAF", &ISample::SetAF )
        .def( "Grey8", &ISample::Grey8 )
        .def( "Red8", &ISample::Red8 )
        .def( "Green8", &ISample::Green8 )
        .def( "Blue8", &ISample::Blue8 )
        .def( "R8", &ISample::R8 )
        .def( "G8", &ISample::G8 )
        .def( "B8", &ISample::B8 )
        .def( "Hue8", &ISample::Hue8 )
        .def( "Saturation8", &ISample::Saturation8 )
        .def( "Lightness8", &ISample::Lightness8 )
        .def( "Value8", &ISample::Value8 )
        .def( "Cyan8", &ISample::Cyan8 )
        .def( "Magenta8", &ISample::Magenta8 )
        .def( "Yellow8", &ISample::Yellow8 )
        .def( "Key8", &ISample::Key8 )
        .def( "Luma8", &ISample::Luma8 )
        .def( "U8", &ISample::U8 )
        .def( "V8", &ISample::V8 )
        .def( "K8", &ISample::K8 )
        .def( "X8", &ISample::X8 )
        .def( "Y8", &ISample::Y8 )
        .def( "Z8", &ISample::Z8 )
        .def( "L8", &ISample::L8 )
        .def( "a8", &ISample::a8 )
        .def( "b8", &ISample::b8 )
        .def( "x8", &ISample::x8 )
        .def( "y8", &ISample::y8 )
        .def( "Grey16", &ISample::Grey16 )
        .def( "Red16", &ISample::Red16 )
        .def( "Green16", &ISample::Green16 )
        .def( "Blue16", &ISample::Blue16 )
        .def( "R16", &ISample::R16 )
        .def( "G16", &ISample::G16 )
        .def( "B16", &ISample::B16 )
        .def( "Hue16", &ISample::Hue16 )
        .def( "Saturation16", &ISample::Saturation16 )
        .def( "Lightness16", &ISample::Lightness16 )
        .def( "Value16", &ISample::Value16 )
        .def( "Cyan16", &ISample::Cyan16 )
        .def( "Magenta16", &ISample::Magenta16 )
        .def( "Yellow16", &ISample::Yellow16 )
        .def( "Key16", &ISample::Key16 )
        .def( "Luma16", &ISample::Luma16 )
        .def( "U16", &ISample::U16 )
        .def( "V16", &ISample::V16 )
        .def( "K16", &ISample::K16 )
        .def( "X16", &ISample::X16 )
        .def( "Y16", &ISample::Y16 )
        .def( "Z16", &ISample::Z16 )
        .def( "L16", &ISample::L16 )
        .def( "a16", &ISample::a16 )
        .def( "b16", &ISample::b16 )
        .def( "x16", &ISample::x16 )
        .def( "y16", &ISample::y16 )
        .def( "GreyF", &ISample::GreyF )
        .def( "RedF", &ISample::RedF )
        .def( "GreenF", &ISample::GreenF )
        .def( "BlueF", &ISample::BlueF )
        .def( "RF", &ISample::RF )
        .def( "GF", &ISample::GF )
        .def( "BF", &ISample::BF )
        .def( "HueF", &ISample::HueF )
        .def( "SaturationF", &ISample::SaturationF )
        .def( "LightnessF", &ISample::LightnessF )
        .def( "ValueF", &ISample::ValueF )
        .def( "CyanF", &ISample::CyanF )
        .def( "MagentaF", &ISample::MagentaF )
        .def( "YellowF", &ISample::YellowF )
        .def( "KeyF", &ISample::KeyF )
        .def( "LumaF", &ISample::LumaF )
        .def( "UF", &ISample::UF )
        .def( "VF", &ISample::VF )
        .def( "KF", &ISample::KF )
        .def( "XF", &ISample::XF )
        .def( "YF", &ISample::YF )
        .def( "ZF", &ISample::ZF )
        .def( "LF", &ISample::LF )
        .def( "aF", &ISample::aF )
        .def( "bF", &ISample::bF )
        .def( "xF", &ISample::xF )
        .def( "yF", &ISample::yF )
        .def( "SetGrey8", &ISample::SetGrey8 )
        .def( "SetRed8", &ISample::SetRed8 )
        .def( "SetGreen8", &ISample::SetGreen8 )
        .def( "SetBlue8", &ISample::SetBlue8 )
        .def( "SetR8", &ISample::SetR8 )
        .def( "SetG8", &ISample::SetG8 )
        .def( "SetB8", &ISample::SetB8 )
        .def( "SetHue8", &ISample::SetHue8 )
        .def( "SetSaturation8", &ISample::SetSaturation8 )
        .def( "SetLightness8", &ISample::SetLightness8 )
        .def( "SetValue8", &ISample::SetValue8 )
        .def( "SetCyan8", &ISample::SetCyan8 )
        .def( "SetMagenta8", &ISample::SetMagenta8 )
        .def( "SetYellow8", &ISample::SetYellow8 )
        .def( "SetKey8", &ISample::SetKey8 )
        .def( "SetLuma8", &ISample::SetLuma8 )
        .def( "SetU8", &ISample::SetU8 )
        .def( "SetV8", &ISample::SetV8 )
        .def( "SetK8", &ISample::SetK8 )
        .def( "SetX8", &ISample::SetX8 )
        .def( "SetY8", &ISample::SetY8 )
        .def( "SetZ8", &ISample::SetZ8 )
        .def( "SetL8", &ISample::SetL8 )
        .def( "Seta8", &ISample::Seta8 )
        .def( "Setb8", &ISample::Setb8 )
        .def( "Setx8", &ISample::Setx8 )
        .def( "Sety8", &ISample::Sety8 )
        .def( "SetGrey16", &ISample::SetGrey16 )
        .def( "SetRed16", &ISample::SetRed16 )
        .def( "SetGreen16", &ISample::SetGreen16 )
        .def( "SetBlue16", &ISample::SetBlue16 )
        .def( "SetR16", &ISample::SetR16 )
        .def( "SetG16", &ISample::SetG16 )
        .def( "SetB16", &ISample::SetB16 )
        .def( "SetHue16", &ISample::SetHue16 )
        .def( "SetSaturation16", &ISample::SetSaturation16 )
        .def( "SetLightness16", &ISample::SetLightness16 )
        .def( "SetValue16", &ISample::SetValue16 )
        .def( "SetCyan16", &ISample::SetCyan16 )
        .def( "SetMagenta16", &ISample::SetMagenta16 )
        .def( "SetYellow16", &ISample::SetYellow16 )
        .def( "SetKey16", &ISample::SetKey16 )
        .def( "SetLuma16", &ISample::SetLuma16 )
        .def( "SetU16", &ISample::SetU16 )
        .def( "SetV16", &ISample::SetV16 )
        .def( "SetK16", &ISample::SetK16 )
        .def( "SetX16", &ISample::SetX16 )
        .def( "SetY16", &ISample::SetY16 )
        .def( "SetZ16", &ISample::SetZ16 )
        .def( "SetL16", &ISample::SetL16 )
        .def( "Seta16", &ISample::Seta16 )
        .def( "Setb16", &ISample::Setb16 )
        .def( "Setx16", &ISample::Setx16 )
        .def( "Sety16", &ISample::Sety16 )
        .def( "SetGreyF", &ISample::SetGreyF )
        .def( "SetRedF", &ISample::SetRedF )
        .def( "SetGreenF", &ISample::SetGreenF )
        .def( "SetBlueF", &ISample::SetBlueF )
        .def( "SetRF", &ISample::SetRF )
        .def( "SetGF", &ISample::SetGF )
        .def( "SetBF", &ISample::SetBF )
        .def( "SetHueF", &ISample::SetHueF )
        .def( "SetSaturationF", &ISample::SetSaturationF )
        .def( "SetLightnessF", &ISample::SetLightnessF )
        .def( "SetValueF", &ISample::SetValueF )
        .def( "SetCyanF", &ISample::SetCyanF )
        .def( "SetMagentaF", &ISample::SetMagentaF )
        .def( "SetYellowF", &ISample::SetYellowF )
        .def( "SetKeyF", &ISample::SetKeyF )
        .def( "SetLumaF", &ISample::SetLumaF )
        .def( "SetUF", &ISample::SetUF )
        .def( "SetVF", &ISample::SetVF )
        .def( "SetKF", &ISample::SetKF )
        .def( "SetXF", &ISample::SetXF )
        .def( "SetYF", &ISample::SetYF )
        .def( "SetZF", &ISample::SetZF )
        .def( "SetLF", &ISample::SetLF )
        .def( "SetaF", &ISample::SetaF )
        .def( "SetbF", &ISample::SetbF )
        .def( "SetxF", &ISample::SetxF )
        .def( "SetyF", &ISample::SetyF )
        .def( "ToFormat", &ISample::ToFormat )
        .def_static( "ConvertFormat", &ISample::ConvertFormat )
        .def_static( "MixFormat", &ISample::MixFormat )
        .def_static( "MixRGB", &ISample::MixRGB )
        .def_static( "MixLab", &ISample::MixLab )
        .def( "Premultiply", &ISample::Premultiply )
        .def( "Unpremultiply", &ISample::Unpremultiply )
        .def( "Premultiplied", &ISample::Premultiplied )
        .def( "Unpremultiplied", &ISample::Unpremultiplied );



    /////////
    // FColor
    py::class_< FColor, ISample >( m, "FColor" )
        .def( py::init<>() )
        .def( py::init< eFormat, const FColorSpace* >(), "format"_a, "colorspace"_a = nullptr )
        .def( py::init< const uint8*, eFormat, const FColorSpace* >(), "data"_a, "format"_a, "colorspace"_a = nullptr )
        .def( py::init< const FPixel& >(), "pixel_a" )
        .def( py::init< const FColor& >(), "color_a" )
        .def( py::init< const ISample& >(), "sample_a" )
        .def_static( "RGB", &FColor::RGB )
        .def_static( "CMYA16", &FColor::CMYA16 )
        .def_static( "CMYA8", &FColor::CMYA8 )
        .def_static( "CMYAF", &FColor::CMYAF )
        .def_static( "CMYKA16", &FColor::CMYKA16 )
        .def_static( "CMYKA8", &FColor::CMYKA8 )
        .def_static( "CMYKAF", &FColor::CMYKAF )
        .def_static( "GreyA16", &FColor::GreyA16 )
        .def_static( "GreyA8", &FColor::GreyA8 )
        .def_static( "GreyAF", &FColor::GreyAF )
        .def_static( "HSLA16", &FColor::HSLA16 )
        .def_static( "HSLA8", &FColor::HSLA8 )
        .def_static( "HSLAF", &FColor::HSLAF )
        .def_static( "HSVA16", &FColor::HSVA16 )
        .def_static( "HSVA8", &FColor::HSVA8 )
        .def_static( "HSVAF", &FColor::HSVAF )
        .def_static( "LabA16", &FColor::LabA16 )
        .def_static( "LabA8", &FColor::LabA8 )
        .def_static( "LabAF", &FColor::LabAF )
        .def_static( "RGB", &FColor::RGB )
        .def_static( "RGBA16", &FColor::RGBA16 )
        .def_static( "RGBA8", &FColor::RGBA8 )
        .def_static( "RGBAF", &FColor::RGBAF )
        .def_static( "XYZA16", &FColor::XYZA16 )
        .def_static( "XYZA8", &FColor::XYZA8 )
        .def_static( "XYZAF", &FColor::XYZAF )
        .def_static( "YUVA16", &FColor::YUVA16 )
        .def_static( "YUVA8", &FColor::YUVA8 )
        .def_static( "YUVAF", &FColor::YUVAF )
        .def_static( "YxyA16", &FColor::YxyA16 )
        .def_static( "YxyA8", &FColor::YxyA8 )
        .def_static( "YxyAF", &FColor::YxyAF )
        .def_readonly_static( "Black", &FColor::Black )
        .def_readonly_static( "White", &FColor::White )
        .def_readonly_static( "Red", &FColor::Red )
        .def_readonly_static( "Green", &FColor::Green )
        .def_readonly_static( "Blue", &FColor::Blue )
        .def_readonly_static( "Yellow", &FColor::Yellow )
        .def_readonly_static( "Magenta", &FColor::Magenta )
        .def_readonly_static( "Cyan", &FColor::Cyan )
        .def_readonly_static( "Transparent", &FColor::Transparent );



    /////////
    // FPixel
    py::class_< FPixel, ISample >( m, "FPixel" )
        .def( py::init< uint8*, eFormat, const FColorSpace* >(), "data"_a, "format"_a, "colorspace"_a = nullptr )
        .def( py::init< const FPixel& >(), "pixel"_a )
        .def( py::init< const FColor& >(), "color"_a )
        .def( "SetPointer", &FPixel::SetPointer )
        .def( "Next", &FPixel::Next )
        .def( "Prev", &FPixel::Prev );



    /////////
    // FBlock
    py::class_< FBlock, IHasFormat, IHasColorSpace >( m, "FBlock" )
        .def(
            py::init<
              ULIS::uint16
            , ULIS::uint16
            , eFormat
            , const FColorSpace*
            , const FOnInvalidBlock&
            , const FOnCleanupData&
            >
            ()
            , "width"_a
            , "height"_a
            , "format"_a = eFormat::Format_RGBA8
            , "colorspace"_a = nullptr
            , "onInvalid"_a = FOnInvalidBlock()
            , "onCleanup"_a = FOnCleanupData( &OnCleanup_FreeMemory )
        )
        .def(
            py::init<
              uint8*
            , ULIS::uint16
            , ULIS::uint16
            , eFormat
            , const FColorSpace*
            , const FOnInvalidBlock&
            , const FOnCleanupData&
            >
            ()
            , "data"_a = nullptr
            , "width"_a = 0
            , "height"_a = 0
            , "format"_a = eFormat::Format_RGBA8
            , "colorspace"_a = nullptr
            , "onInvalid"_a = FOnInvalidBlock()
            , "onCleanup"_a = FOnCleanupData( &OnCleanup_FreeMemory )
        )
        .def( "Area", &FBlock::Area )
        .def( "Bits", static_cast< uint8* ( FBlock::* )() >( &FBlock::Bits ) )
        .def( "BytesPerScanLine", &FBlock::BytesPerScanLine )
        .def( "BytesTotal", &FBlock::BytesTotal )
        .def( "Color", &FBlock::Color, "x"_a, "y"_a )
        .def( "Dirty", static_cast< void ( FBlock::* )( bool ) const >( &FBlock::Dirty), "call"_a = true )
        .def( "Dirty", static_cast< void ( FBlock::* )( const FRectI&, bool ) const >( &FBlock::Dirty ), "rect"_a, "call"_a = true )
        .def( "Height", &FBlock::Height )
        .def( "IsHollow", &FBlock::IsHollow )
        .def( "OnCleanup", &FBlock::OnCleanup, "callback"_a )
        .def( "OnInvalid", &FBlock::OnInvalid, "callback"_a )
        .def( "Pixel", static_cast< FPixel ( FBlock::* )( uint16, uint16 ) >( &FBlock::Pixel ), "x"_a, "y"_a )
        .def( "PixelBits", static_cast< uint8* ( FBlock::* )( uint16, uint16 ) >( &FBlock::PixelBits ), "x"_a, "y"_a )
        .def( "Rect", &FBlock::Rect )
        .def( "LoadFromData"
            , &FBlock::LoadFromData
            , "data"_a
            , "width"_a
            , "height"_a
            , "format"_a
            , "colorspace"_a = nullptr
            , "onInvalid"_a = FOnInvalidBlock()
            , "onCleanup"_a = FOnCleanupData( &OnCleanup_FreeMemory ) )
        .def( "ReallocInternalData"
            , &FBlock::ReallocInternalData
            , "width"_a
            , "height"_a
            , "format"_a
            , "colorspace"_a = nullptr
            , "onInvalid"_a = FOnInvalidBlock()
            , "onCleanup"_a = FOnCleanupData( &OnCleanup_FreeMemory ) )
        .def( "Rect", &FBlock::Rect )
        .def( "Sample", &FBlock::Sample, "x"_a, "y"_a, "border_mode"_a, "border_color"_a )
        .def( "SampleSubpixel", &FBlock::SampleSubpixel, "x"_a, "y"_a, "border_mode"_a, "border_color"_a, "compensate_black_drifting"_a )
        .def( "ScanlineBits", static_cast< uint8* ( FBlock::* )( uint16 ) >( &FBlock::ScanlineBits ), "row"_a )
        .def( "SetPixel", &FBlock::SetPixel, "x"_a, "y"_a, "sample"_a )
        .def( "Width", &FBlock::Width );



    /////////
    // FKernel
    py::class_< FKernel, FBlock >( m, "FKernel" )
        .def( py::init< const FVec2I&, float >(), "size"_a, "value"_a = 0.f )
        .def( py::init< const FKernel& >(), "other"_a )
        .def( "At", static_cast< float ( FKernel::* )( int, int ) const >( &FKernel::At ) )
        .def( "At", static_cast< float ( FKernel::* )( const FVec2I& ) const >( &FKernel::At ) )
        .def( "SetAt", static_cast< void ( FKernel::* )( int, int, float ) >( &FKernel::SetAt ) )
        .def( "SetAt", static_cast< void ( FKernel::* )( const FVec2I&, float ) >( &FKernel::SetAt ) )
        .def( "Clear", &FKernel::Clear )
        .def( "Fill", &FKernel::Fill )
        .def( "SetZeroes", &FKernel::SetZeroes )
        .def( "SetOnes", &FKernel::SetOnes )
        .def( "Sum", &FKernel::Sum )
        .def( "Add", &FKernel::Add )
        .def( "Mul", &FKernel::Mul )
        .def( "Normalize", &FKernel::Normalize )
        .def( "IsNormalized", &FKernel::IsNormalized )
        .def( "FlipX", &FKernel::FlipX )
        .def( "FlipY", &FKernel::FlipY )
        .def( "Rotate90CW", &FKernel::Rotate90CW )
        .def( "Rotate90CCW", &FKernel::Rotate90CCW )
        .def( "Rotate180", &FKernel::Rotate180 )
        .def( "Normalized", &FKernel::Normalized )
        .def( "FlippedX", &FKernel::FlippedX )
        .def( "FlippedY", &FKernel::FlippedY )
        .def( "Rotated90CW", &FKernel::Rotated90CW )
        .def( "Rotated90CCW", &FKernel::Rotated90CCW )
        .def( "Rotated180", &FKernel::Rotated180 )
        .def( "Size", &FKernel::Size )
        .def( "Pivot", &FKernel::Pivot )
        .def( "SetPivot", &FKernel::SetPivot )
        .def_readonly_static( "Identity",       &FKernel::Identity      )
        .def_readonly_static( "Edge4",          &FKernel::Edge4         )
        .def_readonly_static( "Edge8",          &FKernel::Edge8         )
        .def_readonly_static( "Sharpen",        &FKernel::Sharpen       )
        .def_readonly_static( "BoxBlur",        &FKernel::BoxBlur       )
        .def_readonly_static( "GaussianBlur",   &FKernel::GaussianBlur  )
        .def_readonly_static( "UnsharpMask",    &FKernel::UnsharpMask   );



    /////////
    // FStructuringElement
    py::class_< FStructuringElement, FBlock >( m, "FStructuringElement" )
        .def( py::init< const FVec2I&, eMorphologicalElementValue >(), "size"_a, "value"_a = 0.f )
        .def( py::init< const FStructuringElement& >(), "other"_a )
        .def( "At", static_cast< eMorphologicalElementValue ( FStructuringElement::* )( int, int ) const >( &FStructuringElement::At ) )
        .def( "At", static_cast< eMorphologicalElementValue ( FStructuringElement::* )( const FVec2I& ) const >( &FStructuringElement::At ) )
        .def( "SetAt", static_cast< void ( FStructuringElement::* )( int, int, eMorphologicalElementValue ) >( &FStructuringElement::SetAt ) )
        .def( "SetAt", static_cast< void ( FStructuringElement::* )( const FVec2I&, eMorphologicalElementValue ) >( &FStructuringElement::SetAt ) )
        .def( "Clear", &FStructuringElement::Clear )
        .def( "Fill", &FStructuringElement::Fill )
        .def( "SetZeroes", &FStructuringElement::SetZeroes )
        .def( "SetOnes", &FStructuringElement::SetOnes )
        .def( "FlipX", &FStructuringElement::FlipX )
        .def( "FlipY", &FStructuringElement::FlipY )
        .def( "Rotate90CW", &FStructuringElement::Rotate90CW )
        .def( "Rotate90CCW", &FStructuringElement::Rotate90CCW )
        .def( "Rotate180", &FStructuringElement::Rotate180 )
        .def( "FlippedX", &FStructuringElement::FlippedX )
        .def( "FlippedY", &FStructuringElement::FlippedY )
        .def( "Rotated90CW", &FStructuringElement::Rotated90CW )
        .def( "Rotated90CCW", &FStructuringElement::Rotated90CCW )
        .def( "Rotated180", &FStructuringElement::Rotated180 )
        .def( "Size", &FStructuringElement::Size )
        .def( "Pivot", &FStructuringElement::Pivot )
        .def( "SetPivot", &FStructuringElement::SetPivot );



    /////////
    // FFontStyleEntry
    py::class_< FFontStyleEntry >( m, "FFontStyleEntry" )
        .def( py::init< const std::string&, const std::string&, const std::string& >(), "family"_a, "style"_a, "path"_a )
        .def( "Family", &FFontStyleEntry::Family )
        .def( "Style", &FFontStyleEntry::Style )
        .def( "Path", &FFontStyleEntry::Path );



    /////////
    // FFontFamilyEntry
    py::class_< FFontFamilyEntry >( m, "FFontFamilyEntry" )
        .def( py::init< const std::string& >(), "name"_a )
        .def( "AddFontStyleKey", &FFontFamilyEntry::AddFontStyleKey )
        .def( "StyleCount", &FFontFamilyEntry::StyleCount )
        .def( "Styles", &FFontFamilyEntry::Styles )
        .def( "Family", &FFontFamilyEntry::Family )
        .def( "FuzzyFindFontStyleKey", &FFontFamilyEntry::FuzzyFindFontStyleKey );



    /////////
    // FFontEngine
    py::class_< FFontEngine >( m, "FFontEngine" )
        .def( py::init<>() )
        .def( "LibraryHandle", &FFontEngine::LibraryHandle )
        .def( "AddLookupPath", &FFontEngine::AddLookupPath )
        .def( "AddLookupPaths", &FFontEngine::AddLookupPaths )
        .def( "Refresh", &FFontEngine::Refresh )
        .def( "FamilyCount", &FFontEngine::FamilyCount )
        .def( "Records", &FFontEngine::Records )
        .def( "LookupPaths", &FFontEngine::LookupPaths )
        .def( "FuzzyFindFontFamily", &FFontEngine::FuzzyFindFontFamily )
        .def( "FuzzyFindFontStyle", &FFontEngine::FuzzyFindFontStyle )
        .def( "FuzzyFindFontPath", &FFontEngine::FuzzyFindFontPath );



    /////////
    // FFont
    py::class_< FFont >( m, "FFont" )
        .def( py::init< const FFontEngine&, const std::string&, const std::string& >(), "engine"_a, "family"_a, "style"_a )
        .def( py::init< const FFontEngine&, const FFontStyleEntry* >(), "engine"_a, "entry"_a )
        .def( py::init< const FFontEngine&, const std::string& >(), "engine"_a, "path"_a )
        .def( py::init< const FFont& >(), "other"_a )
        .def( "FontHandle", &FFont::FontHandle )
        .def( "FontEngine", &FFont::FontEngine )
        .def( "Family", &FFont::Family )
        .def( "Style", &FFont::Style );



    /////////
    // FContext
    py::class_< FContext >( m, "FContext" )
        .def( py::init< FCommandQueue&, eFormat, ePerformanceIntent >(), "queue"_a, "format"_a, "intent"_a = ePerformanceIntent::PerformanceIntent_Max )
        .def( "Flush", &FContext::Flush )
        .def( "Finish", &FContext::Finish )
        .def( "Fence", &FContext::Fence )
        .def( "Wait", &FContext::Wait )
        .def( "Format", &FContext::Format )
        .def( "FontEngine", static_cast< FFontEngine& ( FContext::* )() >( &FContext::FontEngine ) )
        .def( "FinishEventNo_OP", &FContext::FinishEventNo_OP )
        .def( "Dummy_OP", &FContext::Dummy_OP )
        .def( "AccumulateSample", ctxCallAdapter< const FBlock&, FColor*, const FRectI&, const FSchedulePolicy& >( &FContext::AccumulateSample )
            , "block"_a, "color"_a, "rect"_a = FRectI::Auto, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "AlphaBlend", ctxCallAdapter< const FBlock&, FBlock&, const FRectI&, const FVec2I&, ufloat, const FSchedulePolicy& >( &FContext::AlphaBlend )
            , "src"_a, "dst"_a, "rect"_a = FRectI::Auto, "pos"_a = FVec2I( 0 ), "opacity"_a = 1.f, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "AlphaBlendAA", ctxCallAdapter< const FBlock&, FBlock&, const FRectI&, const FVec2F&, ufloat, const FSchedulePolicy& >( &FContext::AlphaBlendAA )
            , "src"_a, "dst"_a, "rect"_a = FRectI::Auto, "pos"_a = FVec2I( 0 ), "opacity"_a = 1.f, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "AnalyzeSmallestVisibleRect", ctxCallAdapter< const FBlock&, FRectI*, const FRectI&, const FSchedulePolicy& >( &FContext::AnalyzeSmallestVisibleRect )
            , "block"_a, "result"_a, "rect"_a = FRectI::Auto, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def_static( "BezierDisplacementFieldMetrics", &FContext::BezierDisplacementFieldMetrics )
        .def_static( "BezierDisplacementMaskMetrics", &FContext::BezierDisplacementMaskMetrics )
        .def( "Blend", ctxCallAdapter< const FBlock&, FBlock&, const FRectI&, const FVec2I&, eBlendMode, eAlphaMode, ufloat, const FSchedulePolicy& >( &FContext::Blend )
            , "src"_a, "dst"_a, "rect"_a = FRectI::Auto, "pos"_a = FVec2I( 0 ), "blendMode"_a = eBlendMode::Blend_Normal, "alphaMode"_a = eAlphaMode::Alpha_Normal, "opacity"_a = 1.f, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "BlendAA", ctxCallAdapter< const FBlock&, FBlock&, const FRectI&, const FVec2F&, eBlendMode, eAlphaMode, ufloat, const FSchedulePolicy& >( &FContext::BlendAA )
            , "src"_a, "dst"_a, "rect"_a = FRectI::Auto, "pos"_a = FVec2F( 0 ), "blendMode"_a = eBlendMode::Blend_Normal, "alphaMode"_a = eAlphaMode::Alpha_Normal, "opacity"_a = 1.f, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "BlendColor", ctxCallAdapter< const ISample&, FBlock&, const FRectI&, eBlendMode, eAlphaMode, ufloat, const FSchedulePolicy& >( &FContext::BlendColor )
            , "color"_a, "dst"_a, "rect"_a = FRectI::Auto, "blendMode"_a = eBlendMode::Blend_Normal, "alphaMode"_a = eAlphaMode::Alpha_Normal, "opacity"_a = 1.f, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "BlendTiled", ctxCallAdapter< const FBlock&, FBlock&, const FRectI&, const FRectI&, const FVec2I&, eBlendMode, eAlphaMode, ufloat, const FSchedulePolicy& >( &FContext::BlendTiled )
            , "src"_a, "dst"_a, "srcRect"_a = FRectI::Auto, "srcRect"_a = FRectI::Auto, "pos"_a = FVec2I( 0 ), "blendMode"_a = eBlendMode::Blend_Normal, "alphaMode"_a = eAlphaMode::Alpha_Normal, "opacity"_a = 1.f, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "BrownianNoise", ctxCallAdapter< FBlock&, float, float, float, ULIS::uint8, int, const FRectI&, const FSchedulePolicy& >( &FContext::BrownianNoise )
            , "block"_a, "frequency"_a = 0.22f, "frequencyMult"_a = 1.8f, "amplitudeMult"_a = 0.35f, "numLayers"_a = 5, "seed"_a = -1, "rect"_a = FRectI::Auto, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "BuildSummedAreaTable", ctxCallAdapter< const FBlock&, FBlock&, const FSchedulePolicy& >( &FContext::BuildSummedAreaTable )
            , "src"_a, "dst"_a, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "Clear", ctxCallAdapter< FBlock&, const FRectI&, const FSchedulePolicy& >( &FContext::Clear )
            , "block"_a, "rect"_a = FRectI::Auto, "policy"_a = FSchedulePolicy::CacheEfficient, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "Clouds", ctxCallAdapter< FBlock&, int, const FRectI&, const FSchedulePolicy& >( &FContext::Clouds )
            , "block"_a, "seed"_a = -1, "rect"_a = FRectI::Auto, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "ConvertFormat", ctxCallAdapter< const FBlock&, FBlock&, const FRectI&, const FVec2I&, const FSchedulePolicy& >( &FContext::ConvertFormat )
            , "src"_a, "dst"_a, "rect"_a = FRectI::Auto, "pos"_a = FVec2I( 0 ), "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "Convolve", ctxCallAdapter< const FBlock&, FBlock&, const FKernel&, const FRectI&, const FVec2I&, eResamplingMethod, eBorderMode, const ISample&, const FSchedulePolicy& >( &FContext::Convolve )
            , "src"_a, "dst"_a, "kernel"_a, "rect"_a = FRectI::Auto, "pos"_a = FVec2I( 0 ), "resamplingMethod"_a = eResamplingMethod::Resampling_Bilinear, "borderMode"_a = eBorderMode::Border_Transparent, "borderValue"_a = FColor::Transparent, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "ConvolvePremult", ctxCallAdapter< const FBlock&, FBlock&, const FKernel&, const FRectI&, const FVec2I&, eResamplingMethod, eBorderMode, const ISample&, const FSchedulePolicy& >( &FContext::ConvolvePremult )
            , "src"_a, "dst"_a, "kernel"_a, "rect"_a = FRectI::Auto, "pos"_a = FVec2I( 0 ), "resamplingMethod"_a = eResamplingMethod::Resampling_Bilinear, "borderMode"_a = eBorderMode::Border_Transparent, "borderValue"_a = FColor::Transparent, "policy"_a = FSchedulePolicy::MultiScanlines, "waitList"_a = py::list(), "event"_a = nullptr )
        .def( "Copy", ctxCallAdapter< const FBlock&, FBlock&, const FRectI&, const FVec2I&, const FSchedulePolicy& >( &FContext::Copy )
            , "src"_a, "dst"_a, "rect"_a = FRectI::Auto, "pos"_a = FVec2I( 0 ), "policy"_a = FSchedulePolicy::CacheEfficient, "waitList"_a = py::list(), "event"_a = nullptr )
        ;
}
#pragma warning(pop)

