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
template< typename ... Ts, typename F >
auto ctxCallAdapter( F fptr ) {
    return  [fptr]( FContext* ctx, Ts ... args, const emscripten::val& iWaitList, FEvent* iEvent ) {
        TArray< FEvent > arr;

        const auto len = iWaitList["length"].as< unsigned >();
        arr.Reserve( len );

        emscripten::val memoryView{ emscripten::typed_memory_view( len, arr.Data() ) };
        memoryView.call<void>( "set", iWaitList );

        (ctx->*fptr)( args ..., static_cast< uint32 >( arr.Size() ), arr.Data(), iEvent );
    };
}

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



    /////////
    // eType
    enum_< eType >( "eType" )
        .value( "Type_uint8",   eType::Type_uint8   )
        .value( "Type_uint16",  eType::Type_uint16  )
        .value( "Type_ufloat",  eType::Type_ufloat  );



    /////////
    // eFormat
    enum_< eFormat >( "eFormat" )
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
        .value( "Format_yxYAF",     eFormat::Format_yxYAF   );



    /////////
    // eBorderMode
    enum_< eBorderMode >( "eBorderMode" )
        .value( "Border_Transparent",   eBorderMode::Border_Transparent )
        .value( "Border_Constant",      eBorderMode::Border_Constant    )
        .value( "Border_Extend",        eBorderMode::Border_Extend      )
        .value( "Border_Wrap",          eBorderMode::Border_Wrap        );



    /////////
    // eBlendMode
    enum_< eBlendMode >( "eBlendMode" )
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
        .value( "NumBlendModes",            eBlendMode::NumBlendModes           );



    /////////
    // eAlphaMode
    enum_< eAlphaMode >( "eAlphaMode" )
        .value( "Alpha_Normal",     eAlphaMode::Alpha_Normal    )
        .value( "Alpha_Erase",      eAlphaMode::Alpha_Erase     )
        .value( "Alpha_Top",        eAlphaMode::Alpha_Top       )
        .value( "Alpha_Back",       eAlphaMode::Alpha_Back      )
        .value( "Alpha_Sub",        eAlphaMode::Alpha_Sub       )
        .value( "Alpha_Add",        eAlphaMode::Alpha_Add       )
        .value( "Alpha_Mul",        eAlphaMode::Alpha_Mul       )
        .value( "Alpha_Min",        eAlphaMode::Alpha_Min       )
        .value( "Alpha_Max",        eAlphaMode::Alpha_Max       )
        .value( "NumAlphaModes",    eAlphaMode::NumAlphaModes   );



    /////////
    // eBlendQualifier
    enum_< eBlendQualifier >( "eBlendQualifier" )
        .value( "BlendQualifier_Separable",     eBlendQualifier::BlendQualifier_Separable       )
        .value( "BlendQualifier_NonSeparable",  eBlendQualifier::BlendQualifier_NonSeparable    )
        .value( "BlendQualifier_Misc",          eBlendQualifier::BlendQualifier_Misc            );



    /////////
    // eResamplingMethod
    enum_< eResamplingMethod >( "eResamplingMethod" )
        .value( "Resampling_NearestNeighbour",  eResamplingMethod::Resampling_NearestNeighbour  )
        .value( "Resampling_Bilinear",          eResamplingMethod::Resampling_Bilinear          )
        .value( "Resampling_Bicubic",           eResamplingMethod::Resampling_Bicubic           )
        .value( "Resampling_Area",              eResamplingMethod::Resampling_Area              );



    /////////
    // eMipsLayout
    enum_< eMipsLayout >( "eMipsLayout" )
        .value( "MipsLayout_Standard",  eMipsLayout::MipsLayout_Standard    )
        .value( "MipsLayout_MipsOnly",  eMipsLayout::MipsLayout_MipsOnly    );



    /////////
    // eFileFormat
    enum_< eFileFormat >( "eFileFormat" )
        .value( "FileFormat_png", eFileFormat::FileFormat_png )
        .value( "FileFormat_bmp", eFileFormat::FileFormat_bmp )
        .value( "FileFormat_tga", eFileFormat::FileFormat_tga )
        .value( "FileFormat_jpg", eFileFormat::FileFormat_jpg )
        .value( "FileFormat_hdr", eFileFormat::FileFormat_hdr );



    /////////
    // eGradientType
    enum_< eGradientType >( "eGradientType" )
        .value( "Gradient_Linear",      eGradientType::Gradient_Linear      )
        .value( "Gradient_Radial",      eGradientType::Gradient_Radial      )
        .value( "Gradient_Angular",     eGradientType::Gradient_Angular     )
        .value( "Gradient_Reflected",   eGradientType::Gradient_Reflected   )
        .value( "Gradient_Diamond",     eGradientType::Gradient_Diamond     );



    /////////
    // eMorphologicalElementValue
    enum_< eMorphologicalElementValue >( "eMorphologicalElementValue" )
        .value( "MpE_Zero",     eMorphologicalElementValue::MpE_Zero    )
        .value( "MpE_One",      eMorphologicalElementValue::MpE_One     )
        .value( "MpE_Any",      eMorphologicalElementValue::MpE_Any     );



    /////////
    // eLayerType
    enum_< eLayerType >( "eLayerType" )
        .value( "Layer_Invalid",    eLayerType::Layer_Invalid   )
        .value( "Layer_Root",       eLayerType::Layer_Root      )
        .value( "Layer_Image",      eLayerType::Layer_Image     )
        .value( "Layer_Folder",     eLayerType::Layer_Folder    )
        .value( "Layer_Text",       eLayerType::Layer_Text      )
        .value( "Layer_Vector",     eLayerType::Layer_Vector    )
        .value( "Layer_FX",         eLayerType::Layer_FX        )
        .value( "Layer_Mask",       eLayerType::Layer_Mask      );



    /////////
    // eEventStatus
    enum_< eEventStatus >( "eEventStatus" )
        .value( "EventStatus_Idle",     eEventStatus::EventStatus_Idle      )
        .value( "EventStatus_Queued",   eEventStatus::EventStatus_Queued    )
        .value( "EventStatus_Finished", eEventStatus::EventStatus_Finished  );



    /////////
    // eScheduleTimePolicy
    enum_< eScheduleTimePolicy >( "eScheduleTimePolicy" )
        .value( "ScheduleTime_Sync",    eScheduleTimePolicy::ScheduleTime_Sync  )
        .value( "ScheduleTime_Async",   eScheduleTimePolicy::ScheduleTime_Async );



    /////////
    // eScheduleRunPolicy
    enum_< eScheduleRunPolicy >( "eScheduleRunPolicy" )
        .value( "ScheduleRun_Mono",     eScheduleRunPolicy::ScheduleRun_Mono    )
        .value( "ScheduleRun_Multi",    eScheduleRunPolicy::ScheduleRun_Multi   );



    /////////
    // eScheduleModePolicy
    enum_< eScheduleModePolicy >( "eScheduleModePolicy" )
        .value( "ScheduleMode_Scanlines",   eScheduleModePolicy::ScheduleMode_Scanlines )
        .value( "ScheduleMode_Chunks",      eScheduleModePolicy::ScheduleMode_Chunks    );



    /////////
    // eScheduleParameterPolicy
    enum_< eScheduleParameterPolicy >( "eScheduleParameterPolicy" )
        .value( "ScheduleParameter_Count",  eScheduleParameterPolicy::ScheduleParameter_Count   )
        .value( "ScheduleParameter_Length", eScheduleParameterPolicy::ScheduleParameter_Length  );



    /////////
    // ePerformanceIntent
    enum_< ePerformanceIntent >( "ePerformanceIntent" )
        .value( "PerformanceIntent_MEM", ePerformanceIntent::PerformanceIntent_MEM )
        .value( "PerformanceIntent_SSE", ePerformanceIntent::PerformanceIntent_SSE )
        .value( "PerformanceIntent_AVX", ePerformanceIntent::PerformanceIntent_AVX )
        .value( "PerformanceIntent_Max", ePerformanceIntent::PerformanceIntent_Max );



    /////////
    // FFormatMetrics
    class_< FFormatMetrics >( "FFormatMetrics" )
        .constructor< eFormat >()
        .function( "ReinterpretedType", &FFormatMetrics::ReinterpretedType )
        // Error !
        //.property( "IDT", &FFormatMetrics::IDT )
        // Workaround1:
        .function(
              "RedirectedIndex"
            , optional_override(
                []( const FFormatMetrics& self, int index ) {
                    return  self.IDT[ index ];
                }
            )
        )
        // Workaround2:
        .function(
              "IDT"
            , optional_override(
                []( const FFormatMetrics& self, int index ) {
                    return  self.IDT;
                }
            )
            , allow_raw_pointers()
        )
        // Workaround to get readonly static properties ( const cast )
        .property( "FMT", (const ULIS::eFormat ULIS::FFormatMetrics::*)&FFormatMetrics::FMT )
        .property( "TP", (const ULIS::eType ULIS::FFormatMetrics::*)&FFormatMetrics::TP )
        .property( "CM", (const ULIS::eColorModel ULIS::FFormatMetrics::*)&FFormatMetrics::CM )
        .property( "BPC", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::BPC )
        .property( "NCC", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::NCC )
        .property( "HEA", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::HEA )
        .property( "RSC", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::RSC )
        .property( "SPP", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::SPP )
        .property( "BPP", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::BPP )
        .property( "AID", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::AID )
        .property( "REV", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::REV )
        .property( "SWA", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::SWA )
        .property( "PRE", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::PRE )
        .property( "LIN", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::LIN )
        .property( "PRO", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::PRO )
        .property( "PLA", (const ULIS::uint8 ULIS::FFormatMetrics::*)&FFormatMetrics::PLA );



    /////////
    // FColorSpace
    class_< FColorSpace >( "FColorSpace" )
        .constructor<>();



    /////////
    // FOnInvalidBlock
    class_< FOnInvalidBlock >( "FOnInvalidBlock" )
        .constructor<>()
        .constructor< FOnInvalidBlock::tFptr, void* >( allow_raw_pointers() )
        .function( "ExecuteIfBound", &FOnInvalidBlock::ExecuteIfBound, allow_raw_pointers() )
        .function( "Execute", &FOnInvalidBlock::Execute, allow_raw_pointers() );



    /////////
    // FOnCleanupData
    class_< FOnCleanupData >( "FOnCleanupData" )
        .constructor<>()
        .constructor< FOnCleanupData::tFptr, void* >( allow_raw_pointers() )
        .function( "ExecuteIfBound", &FOnCleanupData::ExecuteIfBound, allow_raw_pointers() )
        .function( "Execute", &FOnCleanupData::Execute, allow_raw_pointers() )
        .class_function( "OnCleanupFreeMemory", optional_override( [](){ return  FOnCleanupData( &OnCleanup_FreeMemory ); } ) );



    /////////
    // FOnEventComplete
    class_< FOnEventComplete >( "FOnEventComplete" )
        .constructor<>()
        // Disable first construction, cannot overload same number of params.
        //.constructor< FOnEventComplete::tFptr >()
        // Lambda wrapper to authorize creating an FOnEventComplete from a
        // js function, wrapped into a secondary lambda.
        // Don't care about async since wasm binding is monothread.
        .constructor(
            optional_override(
                []( emscripten::val func ) {
                    return  FOnEventComplete(
                        [ func ]( const FRectI& rect ) {
                            func( rect );
                        }
                    );
                }
            )
        )
        .function( "ExecuteIfBound", &FOnEventComplete::ExecuteIfBound )
        .function( "Execute", &FOnEventComplete::Execute );



    /////////
    // FEvent
    class_< FEvent >( "FEvent" )
        .constructor<>()
        .constructor< const FOnEventComplete& >()
        .function( "Status", &FEvent::Status )
        .function( "Wait", &FEvent::Wait );
    register_vector< FEvent >( "FEventVector" );



    /////////
    // FFilePathRegistry
    class_< FFilePathRegistry >( "FFilePathRegistry" )
        .constructor<>()
        .function( "LookupPaths", &FFilePathRegistry::LookupPaths )
        .function( "Filters", &FFilePathRegistry::Filters )
        .function( "Records", &FFilePathRegistry::Records )
        .function( "AddLookupPath", &FFilePathRegistry::AddLookupPath )
        .function( "AddLookupPaths", &FFilePathRegistry::AddLookupPaths )
        .function( "AddFilter", &FFilePathRegistry::AddFilter )
        .function( "AddFilters", &FFilePathRegistry::AddFilters )
        .function( "Parse", &FFilePathRegistry::Parse )
        .function( "FilePathForExactMatchingName", &FFilePathRegistry::FilePathForExactMatchingName )
        .function( "FilePathForClosestMatchingName", &FFilePathRegistry::FilePathForClosestMatchingName );



    /////////
    // FThreadPool
    class_< FThreadPool >( "FThreadPool" )
        .constructor< uint32 >()
        .function( "WaitForCompletion", &FThreadPool::WaitForCompletion )
        .function( "SetNumWorkers", &FThreadPool::SetNumWorkers )
        .function( "GetNumWorkers", &FThreadPool::GetNumWorkers )
        .class_function( "MaxWorkers", &FThreadPool::MaxWorkers );



    /////////
    // FCommandQueue
    class_< FCommandQueue >( "FCommandQueue" )
        .constructor< FThreadPool& >()
        .function( "Flush", &FCommandQueue::Flush )
        .function( "Finish", &FCommandQueue::Finish )
        .function( "Fence", &FCommandQueue::Fence );



    /////////
    // FSchedulePolicy
    class_< FSchedulePolicy >( "FSchedulePolicy" )
        .constructor<
              eScheduleTimePolicy
            , eScheduleRunPolicy
            , eScheduleModePolicy
            , eScheduleParameterPolicy
            , ULIS::int64
        >
        ()
        .function( "TimePolicy", &FSchedulePolicy::TimePolicy )
        .function( "RunPolicy", &FSchedulePolicy::RunPolicy )
        .function( "ModePolicy", &FSchedulePolicy::ModePolicy )
        .function( "ParameterPolicy", &FSchedulePolicy::ParameterPolicy )
        .function( "Value", &FSchedulePolicy::Value )
        // Workaround to get readonly static properties ( const cast )
        .class_property( "AsyncCacheEfficient", (const FSchedulePolicy*)&FSchedulePolicy::AsyncCacheEfficient )
        .class_property( "CacheEfficient",      (const FSchedulePolicy*)&FSchedulePolicy::CacheEfficient      )
        .class_property( "AsyncMonoChunk",      (const FSchedulePolicy*)&FSchedulePolicy::AsyncMonoChunk      )
        .class_property( "MonoChunk",           (const FSchedulePolicy*)&FSchedulePolicy::MonoChunk           )
        .class_property( "AsyncMultiScanlines", (const FSchedulePolicy*)&FSchedulePolicy::AsyncMultiScanlines )
        .class_property( "AsyncMonoScanlines",  (const FSchedulePolicy*)&FSchedulePolicy::AsyncMonoScanlines  )
        .class_property( "MultiScanlines",      (const FSchedulePolicy*)&FSchedulePolicy::MultiScanlines      )
        .class_property( "MonoScanlines",       (const FSchedulePolicy*)&FSchedulePolicy::MonoScanlines       );



    /////////
    // FHardwareMetrics
    class_< FHardwareMetrics >( "FHardwareMetrics" )
        .constructor<>()
        .function( "IsHardwareAMD",             &FHardwareMetrics::IsHardwareAMD            )
        .function( "IsHardwareIntel",           &FHardwareMetrics::IsHardwareIntel          )
        .function( "IsOSx64",                   &FHardwareMetrics::IsOSx64                  )
        .function( "HasOSAVX",                  &FHardwareMetrics::HasOSAVX                 )
        .function( "HasOSAVX512",               &FHardwareMetrics::HasOSAVX512              )
        .function( "HasHardwarex64",            &FHardwareMetrics::HasHardwarex64           )
        .function( "HasHardwareMMX",            &FHardwareMetrics::HasHardwareMMX           )
        .function( "HasHardwareABM",            &FHardwareMetrics::HasHardwareABM           )
        .function( "HasHardwareRDRAND",         &FHardwareMetrics::HasHardwareRDRAND        )
        .function( "HasHardwareBMI1",           &FHardwareMetrics::HasHardwareBMI1          )
        .function( "HasHardwareBMI2",           &FHardwareMetrics::HasHardwareBMI2          )
        .function( "HasHardwareADX",            &FHardwareMetrics::HasHardwareADX           )
        .function( "HasHardwarePREFETCHWT1",    &FHardwareMetrics::HasHardwarePREFETCHWT1   )
        .function( "HasHardwareMPX",            &FHardwareMetrics::HasHardwareMPX           )
        .function( "HasHardwareSSE",            &FHardwareMetrics::HasHardwareSSE           )
        .function( "HasHardwareSSE2",           &FHardwareMetrics::HasHardwareSSE2          )
        .function( "HasHardwareSSE3",           &FHardwareMetrics::HasHardwareSSE3          )
        .function( "HasHardwareSSSE3",          &FHardwareMetrics::HasHardwareSSSE3         )
        .function( "HasHardwareSSE41",          &FHardwareMetrics::HasHardwareSSE41         )
        .function( "HasHardwareSSE42",          &FHardwareMetrics::HasHardwareSSE42         )
        .function( "HasHardwareSSE4a",          &FHardwareMetrics::HasHardwareSSE4a         )
        .function( "HasHardwareAES",            &FHardwareMetrics::HasHardwareAES           )
        .function( "HasHardwareSHA",            &FHardwareMetrics::HasHardwareSHA           )
        .function( "HasHardwareAVX",            &FHardwareMetrics::HasHardwareAVX           )
        .function( "HasHardwareXOP",            &FHardwareMetrics::HasHardwareXOP           )
        .function( "HasHardwareFMA3",           &FHardwareMetrics::HasHardwareFMA3          )
        .function( "HasHardwareFMA4",           &FHardwareMetrics::HasHardwareFMA4          )
        .function( "HasHardwareAVX2",           &FHardwareMetrics::HasHardwareAVX2          )
        .function( "HasHardwareAVX512_F",       &FHardwareMetrics::HasHardwareAVX512_F      )
        .function( "HasHardwareAVX512_PF",      &FHardwareMetrics::HasHardwareAVX512_PF     )
        .function( "HasHardwareAVX512_ER",      &FHardwareMetrics::HasHardwareAVX512_ER     )
        .function( "HasHardwareAVX512_CD",      &FHardwareMetrics::HasHardwareAVX512_CD     )
        .function( "HasHardwareAVX512_VL",      &FHardwareMetrics::HasHardwareAVX512_VL     )
        .function( "HasHardwareAVX512_BW",      &FHardwareMetrics::HasHardwareAVX512_BW     )
        .function( "HasHardwareAVX512_DQ",      &FHardwareMetrics::HasHardwareAVX512_DQ     )
        .function( "HasHardwareAVX512_IFMA",    &FHardwareMetrics::HasHardwareAVX512_IFMA   )
        .function( "HasHardwareAVX512_VBMI",    &FHardwareMetrics::HasHardwareAVX512_VBMI   )
        .function( "MaxWorkers",                &FHardwareMetrics::MaxWorkers               )
        .function( "L1CacheSize",               &FHardwareMetrics::L1CacheSize              )
        .function( "L1CacheLineSize",           &FHardwareMetrics::L1CacheLineSize          )
        .function( "Field",                     &FHardwareMetrics::Field                    );



    /////////
    // Vec Utils
    #define WULIS_DEFINE_VEC2_SWIZZLE_FUNCTION( CLASS, E0, E1 )         .function( ULIS_STRINGIFY( E0 ## E1 ), & CLASS :: E0 ## E1 )
    #define WULIS_DEFINE_VEC3_SWIZZLE_FUNCTION( CLASS, E0, E1, E2 )     .function( ULIS_STRINGIFY( E0 ## E1 ## E2 ), & CLASS :: E0 ## E1 ## E2 )
    #define WULIS_DEFINE_VEC4_SWIZZLE_FUNCTION( CLASS, E0, E1, E2, E3 ) .function( ULIS_STRINGIFY( E0 ## E1 ## E2 ## E3 ), & CLASS :: E0 ## E1 ## E2 ## E3 )
    #define WULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS ) ULIS_FOR_ALL_VEC2_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS, WULIS_DEFINE_VEC2_SWIZZLE_FUNCTION )
    #define WULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS ) ULIS_FOR_ALL_VEC2_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS, WULIS_DEFINE_VEC2_SWIZZLE_FUNCTION )
    #define WULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS ) ULIS_FOR_ALL_VEC2_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS, WULIS_DEFINE_VEC2_SWIZZLE_FUNCTION )
    #define WULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS ) ULIS_FOR_ALL_VEC3_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS, WULIS_DEFINE_VEC3_SWIZZLE_FUNCTION )
    #define WULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS ) ULIS_FOR_ALL_VEC3_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS, WULIS_DEFINE_VEC3_SWIZZLE_FUNCTION )
    #define WULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS ) ULIS_FOR_ALL_VEC3_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS, WULIS_DEFINE_VEC3_SWIZZLE_FUNCTION )
    #define WULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS ) ULIS_FOR_ALL_VEC4_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS, WULIS_DEFINE_VEC4_SWIZZLE_FUNCTION )
    #define WULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS ) ULIS_FOR_ALL_VEC4_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS, WULIS_DEFINE_VEC4_SWIZZLE_FUNCTION )
    #define WULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS ) ULIS_FOR_ALL_VEC4_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS, WULIS_DEFINE_VEC4_SWIZZLE_FUNCTION )
    #define WULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC2( CLASS )    \
    WULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS )        \
    WULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS )        \
    WULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC2( CLASS )
    #define WULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC3( CLASS )    \
    WULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS )        \
    WULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS )        \
    WULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC3( CLASS )
    #define WULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC4( CLASS )    \
    WULIS_DEFINE_VEC2_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS )        \
    WULIS_DEFINE_VEC3_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS )        \
    WULIS_DEFINE_VEC4_SWIZZLE_FUNCTIONS_IN_VEC4( CLASS )



    /////////
    // FVec2I
    class_< FVec2I >( "FVec2I" )
        .constructor<>()
        .constructor< int >()
        .constructor< int, int >()
        // Workaround: cannot overload constructor by type.
        //.constructor< const FVec2I& >()
        //.constructor< const FVec3I& >()
        //.constructor< const FVec4I& >()
        //.constructor< const FVec2F& >()
        //.constructor< const FVec3F& >()
        //.constructor< const FVec4F& >()
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther ){ return  FVec2I( iOther ); } ) )
        .class_function( "FromVec3I", optional_override( []( const FVec3I& iOther ){ return  FVec2I( iOther ); } ) )
        .class_function( "FromVec4I", optional_override( []( const FVec4I& iOther ){ return  FVec2I( iOther ); } ) )
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther ){ return  FVec2I( iOther ); } ) )
        .class_function( "FromVec3F", optional_override( []( const FVec3F& iOther ){ return  FVec2I( iOther ); } ) )
        .class_function( "FromVec4F", optional_override( []( const FVec4F& iOther ){ return  FVec2I( iOther ); } ) )
        .function( "Distance", &FVec2I::Distance )
        .function( "DistanceSquared", &FVec2I::DistanceSquared )
        .function( "ManhattanDistance", &FVec2I::ManhattanDistance )
        .function( "DotProduct", &FVec2I::DotProduct )
        .function( "Normalize", &FVec2I::Normalize )
        .function( "Normalized", &FVec2I::Normalized )
        .function( "DecimalPart", &FVec2I::DecimalPart )
        // Workaround: cannot use operator overloads in JS.
        // Use named operators instead. Get rid of unary operators except negate.
        .function( "AddI", optional_override( []( const FVec2I& iA, int iB  ){ return  iA + iB; } ) )
        .function( "SubI", optional_override( []( const FVec2I& iA, int iB  ){ return  iA - iB; } ) )
        .function( "MulI", optional_override( []( const FVec2I& iA, int iB  ){ return  iA * iB; } ) )
        .function( "DivI", optional_override( []( const FVec2I& iA, int iB  ){ return  iA / iB; } ) )
        .function( "Add", optional_override( []( const FVec2I& iA, const FVec2I& iB  ){ return  iA + iB; } ) )
        .function( "Sub", optional_override( []( const FVec2I& iA, const FVec2I& iB  ){ return  iA - iB; } ) )
        .function( "Mul", optional_override( []( const FVec2I& iA, const FVec2I& iB  ){ return  iA * iB; } ) )
        .function( "Div", optional_override( []( const FVec2I& iA, const FVec2I& iB  ){ return  iA / iB; } ) )
        .function( "Eq", optional_override( []( const FVec2I& iA, const FVec2I& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const FVec2I& iA, const FVec2I& iB  ){ return  iA != iB; } ) )
        .function( "Neg", optional_override( []( const FVec2I& iA ){ return  -iA; } ) )
        WULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC2( FVec2I )
        .property( "x", &FVec2I::x )
        .property( "y", &FVec2I::y );



    /////////
    // FVec3I
    class_< FVec3I >( "FVec3I" )
        .constructor<>()
        .constructor< int >()
        .constructor< int, int, int >()
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther ){ return  FVec3I( iOther ); } ) )
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther, int iVal ){ return  FVec3I( iOther, iVal ); } ) )
        .class_function( "FromVec3I", optional_override( []( const FVec3I& iOther ){ return  FVec3I( iOther ); } ) )
        .class_function( "FromVec4I", optional_override( []( const FVec4I& iOther ){ return  FVec3I( iOther ); } ) )
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther ){ return  FVec3I( iOther ); } ) )
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther, float iVal ){ return  FVec3I( iOther, iVal ); } ) )
        .class_function( "FromVec3F", optional_override( []( const FVec3F& iOther ){ return  FVec3I( iOther ); } ) )
        .class_function( "FromVec4F", optional_override( []( const FVec4F& iOther ){ return  FVec3I( iOther ); } ) )
        .function( "Distance", &FVec3I::Distance )
        .function( "DistanceSquared", &FVec3I::DistanceSquared )
        .function( "ManhattanDistance", &FVec3I::ManhattanDistance )
        .function( "DotProduct", &FVec3I::DotProduct )
        .function( "Normalize", &FVec3I::Normalize )
        .function( "Normalized", &FVec3I::Normalized )
        .function( "AddI", optional_override( []( const FVec3I& iA, int iB  ){ return  iA + iB; } ) )
        .function( "SubI", optional_override( []( const FVec3I& iA, int iB  ){ return  iA - iB; } ) )
        .function( "MulI", optional_override( []( const FVec3I& iA, int iB  ){ return  iA * iB; } ) )
        .function( "DivI", optional_override( []( const FVec3I& iA, int iB  ){ return  iA / iB; } ) )
        .function( "Add", optional_override( []( const FVec3I& iA, const FVec3I& iB  ){ return  iA + iB; } ) )
        .function( "Sub", optional_override( []( const FVec3I& iA, const FVec3I& iB  ){ return  iA - iB; } ) )
        .function( "Mul", optional_override( []( const FVec3I& iA, const FVec3I& iB  ){ return  iA * iB; } ) )
        .function( "Div", optional_override( []( const FVec3I& iA, const FVec3I& iB  ){ return  iA / iB; } ) )
        .function( "Eq", optional_override( []( const FVec3I& iA, const FVec3I& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const FVec3I& iA, const FVec3I& iB  ){ return  iA != iB; } ) )
        .function( "Neg", optional_override( []( const FVec3I& iA ){ return  -iA; } ) )
        WULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC3( FVec3I )
        .property( "x", &FVec3I::x )
        .property( "y", &FVec3I::y )
        .property( "z", &FVec3I::z );



    /////////
    // FVec4I
    class_< FVec4I >( "FVec4I" )
        .constructor<>()
        .constructor< int >()
        .constructor< int, int, int, int >()
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther ){ return  FVec4I( iOther ); } ) )
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther, int iA, int iB ){ return  FVec4I( iOther, iA, iB ); } ) )
        .class_function( "FromVec3I", optional_override( []( const FVec3I& iOther ){ return  FVec4I( iOther ); } ) )
        .class_function( "FromVec3I", optional_override( []( const FVec3I& iOther, int iA ){ return  FVec4I( iOther, iA ); } ) )
        .class_function( "FromVec4I", optional_override( []( const FVec4I& iOther ){ return  FVec4I( iOther ); } ) )
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther ){ return  FVec4I( iOther ); } ) )
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther, float iA ){ return  FVec4I( iOther, iA  ); } ) )
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther, float iA, float iB ){ return  FVec4I( iOther, iA, iB ); } ) )
        .class_function( "FromVec3F", optional_override( []( const FVec3F& iOther ){ return  FVec4I( iOther ); } ) )
        .class_function( "FromVec3F", optional_override( []( const FVec3F& iOther, float iA ){ return  FVec4I( iOther, iA ); } ) )
        .class_function( "FromVec4F", optional_override( []( const FVec4F& iOther ){ return  FVec4I( iOther ); } ) )
        .function( "Distance", &FVec4I::Distance )
        .function( "DistanceSquared", &FVec4I::DistanceSquared )
        .function( "ManhattanDistance", &FVec4I::ManhattanDistance )
        .function( "DotProduct", &FVec4I::DotProduct )
        .function( "Normalize", &FVec4I::Normalize )
        .function( "Normalized", &FVec4I::Normalized )
        .function( "AddI", optional_override( []( const FVec4I& iA, int iB  ){ return  iA + iB; } ) )
        .function( "SubI", optional_override( []( const FVec4I& iA, int iB  ){ return  iA - iB; } ) )
        .function( "MulI", optional_override( []( const FVec4I& iA, int iB  ){ return  iA * iB; } ) )
        .function( "DivI", optional_override( []( const FVec4I& iA, int iB  ){ return  iA / iB; } ) )
        .function( "Add", optional_override( []( const FVec4I& iA, const FVec4I& iB  ){ return  iA + iB; } ) )
        .function( "Sub", optional_override( []( const FVec4I& iA, const FVec4I& iB  ){ return  iA - iB; } ) )
        .function( "Mul", optional_override( []( const FVec4I& iA, const FVec4I& iB  ){ return  iA * iB; } ) )
        .function( "Div", optional_override( []( const FVec4I& iA, const FVec4I& iB  ){ return  iA / iB; } ) )
        .function( "Eq", optional_override( []( const FVec4I& iA, const FVec4I& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const FVec4I& iA, const FVec4I& iB  ){ return  iA != iB; } ) )
        .function( "Neg", optional_override( []( const FVec4I& iA ){ return  -iA; } ) )
        WULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC4( FVec4I )
        .property( "x", &FVec4I::x )
        .property( "y", &FVec4I::y )
        .property( "z", &FVec4I::z )
        .property( "w", &FVec4I::w );



    /////////
    // FVec2F
    class_< FVec2F >( "FVec2F" )
        .constructor<>()
        .constructor< float >()
        .constructor< float, float >()
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther ){ return  FVec2F( iOther ); } ) )
        .class_function( "FromVec3F", optional_override( []( const FVec3F& iOther ){ return  FVec2F( iOther ); } ) )
        .class_function( "FromVec4F", optional_override( []( const FVec4F& iOther ){ return  FVec2F( iOther ); } ) )
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther ){ return  FVec2F( iOther ); } ) )
        .class_function( "FromVec3I", optional_override( []( const FVec3I& iOther ){ return  FVec2F( iOther ); } ) )
        .class_function( "FromVec4I", optional_override( []( const FVec4I& iOther ){ return  FVec2F( iOther ); } ) )
        .function( "Distance", &FVec2F::Distance )
        .function( "DistanceSquared", &FVec2F::DistanceSquared )
        .function( "ManhattanDistance", &FVec2F::ManhattanDistance )
        .function( "DotProduct", &FVec2F::DotProduct )
        .function( "Normalize", &FVec2F::Normalize )
        .function( "Normalized", &FVec2F::Normalized )
        .function( "DecimalPart", &FVec2F::DecimalPart )
        .function( "AddF", optional_override( []( const FVec2F& iA, float iB  ){ return  iA + iB; } ) )
        .function( "SubF", optional_override( []( const FVec2F& iA, float iB  ){ return  iA - iB; } ) )
        .function( "MulF", optional_override( []( const FVec2F& iA, float iB  ){ return  iA * iB; } ) )
        .function( "DivF", optional_override( []( const FVec2F& iA, float iB  ){ return  iA / iB; } ) )
        .function( "Add", optional_override( []( const FVec2F& iA, const FVec2F& iB  ){ return  iA + iB; } ) )
        .function( "Sub", optional_override( []( const FVec2F& iA, const FVec2F& iB  ){ return  iA - iB; } ) )
        .function( "Mul", optional_override( []( const FVec2F& iA, const FVec2F& iB  ){ return  iA * iB; } ) )
        .function( "Div", optional_override( []( const FVec2F& iA, const FVec2F& iB  ){ return  iA / iB; } ) )
        .function( "Eq", optional_override( []( const FVec2F& iA, const FVec2F& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const FVec2F& iA, const FVec2F& iB  ){ return  iA != iB; } ) )
        .function( "Neg", optional_override( []( const FVec2F& iA ){ return  -iA; } ) )
        WULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC2( FVec2F )
        .property( "x", &FVec2F::x )
        .property( "y", &FVec2F::y );



    /////////
    // FVec3F
    class_< FVec3F >( "FVec3F" )
        .constructor<>()
        .constructor< float >()
        .constructor< float, float, float >()
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther ){ return  FVec3F( iOther ); } ) )
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther, float iVal ){ return  FVec3F( iOther, iVal ); } ) )
        .class_function( "FromVec3F", optional_override( []( const FVec3F& iOther ){ return  FVec3F( iOther ); } ) )
        .class_function( "FromVec4F", optional_override( []( const FVec4F& iOther ){ return  FVec3F( iOther ); } ) )
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther ){ return  FVec3F( iOther ); } ) )
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther, int iVal ){ return  FVec3F( iOther, iVal ); } ) )
        .class_function( "FromVec3I", optional_override( []( const FVec3I& iOther ){ return  FVec3F( iOther ); } ) )
        .class_function( "FromVec4I", optional_override( []( const FVec4I& iOther ){ return  FVec3F( iOther ); } ) )
        .function( "Distance", &FVec3F::Distance )
        .function( "DistanceSquared", &FVec3F::DistanceSquared )
        .function( "ManhattanDistance", &FVec3F::ManhattanDistance )
        .function( "DotProduct", &FVec3F::DotProduct )
        .function( "Normalize", &FVec3F::Normalize )
        .function( "Normalized", &FVec3F::Normalized )
        .function( "AddF", optional_override( []( const FVec3F& iA, float iB  ){ return  iA + iB; } ) )
        .function( "SubF", optional_override( []( const FVec3F& iA, float iB  ){ return  iA - iB; } ) )
        .function( "MulF", optional_override( []( const FVec3F& iA, float iB  ){ return  iA * iB; } ) )
        .function( "DivF", optional_override( []( const FVec3F& iA, float iB  ){ return  iA / iB; } ) )
        .function( "Add", optional_override( []( const FVec3F& iA, const FVec3F& iB  ){ return  iA + iB; } ) )
        .function( "Sub", optional_override( []( const FVec3F& iA, const FVec3F& iB  ){ return  iA - iB; } ) )
        .function( "Mul", optional_override( []( const FVec3F& iA, const FVec3F& iB  ){ return  iA * iB; } ) )
        .function( "Div", optional_override( []( const FVec3F& iA, const FVec3F& iB  ){ return  iA / iB; } ) )
        .function( "Eq", optional_override( []( const FVec3F& iA, const FVec3F& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const FVec3F& iA, const FVec3F& iB  ){ return  iA != iB; } ) )
        .function( "Neg", optional_override( []( const FVec3F& iA ){ return  -iA; } ) )
        WULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC3( FVec3F )
        .property( "x", &FVec3F::x )
        .property( "y", &FVec3F::y )
        .property( "z", &FVec3F::z );



    /////////
    // FVec4F
    class_< FVec4F >( "FVec4F" )
        .constructor<>()
        .constructor< float >()
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther ){ return  FVec4F( iOther ); } ) )
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther, float iA ){ return  FVec4F( iOther, iA  ); } ) )
        .class_function( "FromVec2F", optional_override( []( const FVec2F& iOther, float iA, float iB ){ return  FVec4F( iOther, iA, iB ); } ) )
        .class_function( "FromVec3F", optional_override( []( const FVec3F& iOther ){ return  FVec4F( iOther ); } ) )
        .class_function( "FromVec3F", optional_override( []( const FVec3F& iOther, float iA ){ return  FVec4F( iOther, iA ); } ) )
        .class_function( "FromVec4F", optional_override( []( const FVec4F& iOther ){ return  FVec4F( iOther ); } ) )
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther ){ return  FVec4F( iOther ); } ) )
        .class_function( "FromVec2I", optional_override( []( const FVec2I& iOther, int iA, int iB ){ return  FVec4F( iOther, iA, iB ); } ) )
        .class_function( "FromVec3I", optional_override( []( const FVec3I& iOther ){ return  FVec4F( iOther ); } ) )
        .class_function( "FromVec3I", optional_override( []( const FVec3I& iOther, int iA ){ return  FVec4F( iOther, iA ); } ) )
        .class_function( "FromVec4I", optional_override( []( const FVec4I& iOther ){ return  FVec4F( iOther ); } ) )
        .function( "Distance", &FVec4F::Distance )
        .function( "DistanceSquared", &FVec4F::DistanceSquared )
        .function( "ManhattanDistance", &FVec4F::ManhattanDistance )
        .function( "DotProduct", &FVec4F::DotProduct )
        .function( "Normalize", &FVec4F::Normalize )
        .function( "Normalized", &FVec4F::Normalized )
        .function( "AddF", optional_override( []( const FVec4F& iA, float iB  ){ return  iA + iB; } ) )
        .function( "SubF", optional_override( []( const FVec4F& iA, float iB  ){ return  iA - iB; } ) )
        .function( "MulF", optional_override( []( const FVec4F& iA, float iB  ){ return  iA * iB; } ) )
        .function( "DivF", optional_override( []( const FVec4F& iA, float iB  ){ return  iA / iB; } ) )
        .function( "Add", optional_override( []( const FVec4F& iA, const FVec4F& iB  ){ return  iA + iB; } ) )
        .function( "Sub", optional_override( []( const FVec4F& iA, const FVec4F& iB  ){ return  iA - iB; } ) )
        .function( "Mul", optional_override( []( const FVec4F& iA, const FVec4F& iB  ){ return  iA * iB; } ) )
        .function( "Div", optional_override( []( const FVec4F& iA, const FVec4F& iB  ){ return  iA / iB; } ) )
        .function( "Eq", optional_override( []( const FVec4F& iA, const FVec4F& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const FVec4F& iA, const FVec4F& iB  ){ return  iA != iB; } ) )
        .function( "Neg", optional_override( []( const FVec4F& iA ){ return  -iA; } ) )
        WULIS_DEFINE_ALL_SWIZZLE_FUNCTIONS_VEC4( FVec4F )
        .property( "x", &FVec4F::x )
        .property( "y", &FVec4F::y )
        .property( "z", &FVec4F::z )
        .property( "w", &FVec4F::w );
}

