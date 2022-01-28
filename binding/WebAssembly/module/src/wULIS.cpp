// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         wULIS.cpp
* @author       Clement Berthaud
* @brief        Binding of ULIS for JavaScript / WebAssembly.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
using namespace ::ULIS;

#include <emscripten/bind.h>
using namespace emscripten;

/////////
// Context Utils
#define WULIS_FUNK( i ) decltype( i ), i
template< typename F, F fptr, typename ... Ts >
auto ctxCallAdapter() {
    return  optional_override(
        []( FContext& self, Ts ... args ) {
            (self.*fptr)( args ..., FSchedulePolicy(), 0, nullptr, nullptr );
        }
    );
}

/////////
// Wrapper Utils
// Workaround to simulate multiple inheritance, despite JS allowing only single inheritance chains
//class IHasFormat_Wrapper : public IHasFormat {};
//class IHasFormatAndColorspace_Wrapper : public IHasFormat_Wrapper, public IHasColorSpace {};
//class ISampleWrapper : public IHasFormatAndColorspace_Wrapper {};
//class FColorWrapper : public ISampleWrapper {};

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
        .value( "EventStatus_Idle",      eEventStatus::EventStatus_Idle      )
        .value( "EventStatus_Queued",    eEventStatus::EventStatus_Queued    )
        .value( "EventStatus_Finished",  eEventStatus::EventStatus_Finished  );



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



    /////////
    // FMat3F
    class_< FMat3F >( "FMat3F" )
        .constructor<>()
        .constructor< float >()
        .constructor<
              float, float, float
            , float, float, float
            , float, float, float
        >
        ()
        .constructor< const FVec3F&, const FVec3F&, const FVec3F& >()
        .function( "Data", select_overload< float*() >( &FMat3F::Data ), allow_raw_pointers() )
        .function( "Inverse", &FMat3F::Inverse )
        .function( "Transpose", &FMat3F::Transpose )
        .function( "Determinant", &FMat3F::Determinant )
        .function( "Decompose", &FMat3F::Decompose, allow_raw_pointers() ) // Warning, output by ptr* o, find a workaround.
        .function( "ApplyHomography", &FMat3F::ApplyHomography )
        .function( "MakeRotationMatrix", &FMat3F::MakeRotationMatrix )
        .function( "MakeScaleMatrix", &FMat3F::MakeScaleMatrix )
        .function( "MakeSkewMatrix", &FMat3F::MakeSkewMatrix )
        .function( "MakeTranslationMatrix", &FMat3F::MakeTranslationMatrix )
        .function( "MakeHomography", &FMat3F::MakeHomography, allow_raw_pointers() ) // Warning, input points array by ptr* i, find a workaround
        .function( "AddF", optional_override( []( const FMat3F& iA, float iB  ){ return  iA + iB; } ) )
        .function( "SubF", optional_override( []( const FMat3F& iA, float iB  ){ return  iA - iB; } ) )
        .function( "MulF", optional_override( []( const FMat3F& iA, float iB  ){ return  iA * iB; } ) )
        .function( "DivF", optional_override( []( const FMat3F& iA, float iB  ){ return  iA / iB; } ) )
        .function( "Add", optional_override( []( const FMat3F& iA, const FMat3F& iB  ){ return  iA + iB; } ) )
        .function( "Sub", optional_override( []( const FMat3F& iA, const FMat3F& iB  ){ return  iA - iB; } ) )
        .function( "Mul", optional_override( []( const FMat3F& iA, const FMat3F& iB  ){ return  iA * iB; } ) )
        .function( "Div", optional_override( []( const FMat3F& iA, const FMat3F& iB  ){ return  iA / iB; } ) )
        .function( "MulV", optional_override( []( const FMat3F& iA, const FVec3F& iB  ){ return  iA * iB; } ) )
        .function( "Eq", optional_override( []( const FMat3F& iA, const FMat3F& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const FMat3F& iA, const FMat3F& iB  ){ return  iA != iB; } ) )
        .function( "Neg", optional_override( []( const FMat3F& iA ){ return  -iA; } ) );



    /////////
    // FRectI
    class_< FRectI >( "FRectI" )
        .constructor<>()
        .constructor< int, int, int, int >()
        .class_function( "FromXYWH", &FRectI::FromXYWH )
        .class_function( "FromMinMax", &FRectI::FromMinMax )
        .class_function( "FromPositionAndSize", &FRectI::FromPositionAndSize )
        .function( "HitTest", &FRectI::HitTest )
        .function( "HitTest", &FRectI::InVerticalRange )
        .function( "HitTest", &FRectI::InHorizontalRange )
        .function( "Exclusion", &FRectI::Exclusion, allow_raw_pointers() )
        .function( "UnionLeaveEmpty", &FRectI::UnionLeaveEmpty )
        .function( "And", optional_override( []( const FRectI& iA, const FRectI& iB  ){ return  iA & iB; } ) )
        .function( "Or", optional_override( []( const FRectI& iA, const FRectI& iB  ){ return  iA | iB; } ) )
        .function( "Sub", optional_override( []( const FRectI& iA, const FRectI& iB  ){ return  iA - iB; } ) )
        .function( "Eq", optional_override( []( const FRectI& iA, const FRectI& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const FRectI& iA, const FRectI& iB  ){ return  iA != iB; } ) )
        .function( "Area", &FRectI::Area )
        .function( "Sanitize", &FRectI::Sanitize )
        .function( "Sanitized", &FRectI::Sanitized )
        .function( "TransformAffine", &FRectI::TransformAffine )
        .function( "TransformPerspective", &FRectI::TransformPerspective )
        .function( "TransformedAffine", &FRectI::TransformedAffine )
        .function( "TransformedPerspective", &FRectI::TransformedPerspective )
        .function( "Shift", &FRectI::Shift )
        .function( "Position", &FRectI::Position )
        .function( "Size", &FRectI::Size )
        .class_property( "Auto", (const FRectI*)&FRectI::Auto )
        .property( "x", &FRectI::x )
        .property( "y", &FRectI::y )
        .property( "h", &FRectI::h )
        .property( "w", &FRectI::w );



    /////////
    // FRectF
    class_< FRectF >( "FRectF" )
        .constructor<>()
        .constructor< float, float, float, float >()
        .class_function( "FromXYWH", &FRectF::FromXYWH )
        .class_function( "FromMinMax", &FRectF::FromMinMax )
        .class_function( "FromPositionAndSize", &FRectF::FromPositionAndSize )
        .function( "HitTest", &FRectF::HitTest )
        .function( "HitTest", &FRectF::InVerticalRange )
        .function( "HitTest", &FRectF::InHorizontalRange )
        .function( "Exclusion", &FRectF::Exclusion, allow_raw_pointers() )
        .function( "UnionLeaveEmpty", &FRectF::UnionLeaveEmpty )
        .function( "And", optional_override( []( const FRectF& iA, const FRectF& iB  ){ return  iA & iB; } ) )
        .function( "Or", optional_override( []( const FRectF& iA, const FRectF& iB  ){ return  iA | iB; } ) )
        .function( "Sub", optional_override( []( const FRectF& iA, const FRectF& iB  ){ return  iA - iB; } ) )
        .function( "Eq", optional_override( []( const FRectF& iA, const FRectF& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const FRectF& iA, const FRectF& iB  ){ return  iA != iB; } ) )
        .function( "Area", &FRectF::Area )
        .function( "Sanitize", &FRectF::Sanitize )
        .function( "Sanitized", &FRectF::Sanitized )
        .function( "TransformAffine", &FRectF::TransformAffine )
        .function( "TransformPerspective", &FRectF::TransformPerspective )
        .function( "TransformedAffine", &FRectF::TransformedAffine )
        .function( "TransformedPerspective", &FRectF::TransformedPerspective )
        .function( "Shift", &FRectF::Shift )
        .function( "Position", &FRectF::Position )
        .function( "Size", &FRectF::Size )
        .class_property( "Auto", (const FRectF*)&FRectF::Auto )
        .property( "x", &FRectF::x )
        .property( "y", &FRectF::y )
        .property( "h", &FRectF::h )
        .property( "w", &FRectF::w );



    /////////
    // IHasFormat
    class_< IHasFormat >( "IHasFormat" )
        .function( "FormatMetrics", &IHasFormat::FormatMetrics )
        .function( "IndexTable", &IHasFormat::IndexTable, allow_raw_pointers() )
        .function( "Format", &IHasFormat::Format )
        .function( "Type", &IHasFormat::Type )
        .function( "Model", &IHasFormat::Model )
        .function( "BytesPerSample", &IHasFormat::BytesPerSample )
        .function( "NumColorChannels", &IHasFormat::NumColorChannels )
        .function( "HasAlpha", &IHasFormat::HasAlpha )
        .function( "SamplesPerPixel", &IHasFormat::SamplesPerPixel )
        .function( "BytesPerPixel", &IHasFormat::BytesPerPixel )
        .function( "AlphaIndex", &IHasFormat::AlphaIndex )
        .function( "Reversed", &IHasFormat::Reversed )
        .function( "Swapped", &IHasFormat::Swapped )
        .function( "Premultiplied", &IHasFormat::Premultiplied )
        .function( "Linear", &IHasFormat::Linear )
        .function( "DefaultProfileCode", &IHasFormat::DefaultProfileCode )
        .function( "RedirectedIndex", &IHasFormat::RedirectedIndex );



    /////////
    // IHasColorSpace
    class_< IHasColorSpace >( "IHasColorSpace" )
        .function( "AssignColorSpace", &IHasColorSpace::AssignColorSpace, allow_raw_pointers() )
        .function( "ColorSpace", &IHasColorSpace::ColorSpace, allow_raw_pointers() );



    /////////
    // __DEPRECATED__ SEE BELOW
    // IHasFormat_Wrapper
    // IHasFormatAndColorspace_Wrapper
    // Workaround to simulate multiple inheritance, despite JS allowing only single inheritance chains
    //class_< IHasFormat_Wrapper >( "IHasFormat_Wrapper" );
    //class_< IHasFormatAndColorspace_Wrapper, base< IHasColorSpace > >( "IHasFormatAndColorspace_Wrapper" );
    // __DEPRECATED__
    // Instead of using complicated wrappers, we decided to discard the inheritance path from IHasColorSpace for now.



    /////////
    // ISample
    class_< ISample, base< IHasFormat > >( "ISample" )
        .function( "Eq", optional_override( []( const ISample& iA, const ISample& iB  ){ return  iA == iB; } ) )
        .function( "Neq", optional_override( []( const ISample& iA, const ISample& iB  ){ return  iA != iB; } ) )
        .function( "Bits", select_overload< uint8*() >( &ISample::Bits ), allow_raw_pointers() )
        .function( "Channel8", &ISample::Channel8 )
        .function( "Channel16", &ISample::Channel16 )
        .function( "ChannelF", &ISample::ChannelF )
        .function( "SetChannel8", &ISample::SetChannel8 )
        .function( "SetChannel16", &ISample::SetChannel16 )
        .function( "SetChannelF", &ISample::SetChannelF )
        .function( "Alpha8", &ISample::Alpha8 )
        .function( "Alpha16", &ISample::Alpha16 )
        .function( "AlphaF", &ISample::AlphaF )
        .function( "A8", &ISample::A8 )
        .function( "A16", &ISample::A16 )
        .function( "AF", &ISample::AF )
        .function( "SetAlpha8", &ISample::SetAlpha8 )
        .function( "SetAlpha16", &ISample::SetAlpha16 )
        .function( "SetAlphaF", &ISample::SetAlphaF )
        .function( "SetA8", &ISample::SetA8 )
        .function( "SetA16", &ISample::SetA16 )
        .function( "SetAF", &ISample::SetAF )
        .function( "Grey8", &ISample::Grey8 )
        .function( "Red8", &ISample::Red8 )
        .function( "Green8", &ISample::Green8 )
        .function( "Blue8", &ISample::Blue8 )
        .function( "R8", &ISample::R8 )
        .function( "G8", &ISample::G8 )
        .function( "B8", &ISample::B8 )
        .function( "Hue8", &ISample::Hue8 )
        .function( "Saturation8", &ISample::Saturation8 )
        .function( "Lightness8", &ISample::Lightness8 )
        .function( "Value8", &ISample::Value8 )
        .function( "Cyan8", &ISample::Cyan8 )
        .function( "Magenta8", &ISample::Magenta8 )
        .function( "Yellow8", &ISample::Yellow8 )
        .function( "Key8", &ISample::Key8 )
        .function( "Luma8", &ISample::Luma8 )
        .function( "U8", &ISample::U8 )
        .function( "V8", &ISample::V8 )
        .function( "K8", &ISample::K8 )
        .function( "X8", &ISample::X8 )
        .function( "Y8", &ISample::Y8 )
        .function( "Z8", &ISample::Z8 )
        .function( "L8", &ISample::L8 )
        .function( "a8", &ISample::a8 )
        .function( "b8", &ISample::b8 )
        .function( "x8", &ISample::x8 )
        .function( "y8", &ISample::y8 )
        .function( "Grey16", &ISample::Grey16 )
        .function( "Red16", &ISample::Red16 )
        .function( "Green16", &ISample::Green16 )
        .function( "Blue16", &ISample::Blue16 )
        .function( "R16", &ISample::R16 )
        .function( "G16", &ISample::G16 )
        .function( "B16", &ISample::B16 )
        .function( "Hue16", &ISample::Hue16 )
        .function( "Saturation16", &ISample::Saturation16 )
        .function( "Lightness16", &ISample::Lightness16 )
        .function( "Value16", &ISample::Value16 )
        .function( "Cyan16", &ISample::Cyan16 )
        .function( "Magenta16", &ISample::Magenta16 )
        .function( "Yellow16", &ISample::Yellow16 )
        .function( "Key16", &ISample::Key16 )
        .function( "Luma16", &ISample::Luma16 )
        .function( "U16", &ISample::U16 )
        .function( "V16", &ISample::V16 )
        .function( "K16", &ISample::K16 )
        .function( "X16", &ISample::X16 )
        .function( "Y16", &ISample::Y16 )
        .function( "Z16", &ISample::Z16 )
        .function( "L16", &ISample::L16 )
        .function( "a16", &ISample::a16 )
        .function( "b16", &ISample::b16 )
        .function( "x16", &ISample::x16 )
        .function( "y16", &ISample::y16 )
        .function( "GreyF", &ISample::GreyF )
        .function( "RedF", &ISample::RedF )
        .function( "GreenF", &ISample::GreenF )
        .function( "BlueF", &ISample::BlueF )
        .function( "RF", &ISample::RF )
        .function( "GF", &ISample::GF )
        .function( "BF", &ISample::BF )
        .function( "HueF", &ISample::HueF )
        .function( "SaturationF", &ISample::SaturationF )
        .function( "LightnessF", &ISample::LightnessF )
        .function( "ValueF", &ISample::ValueF )
        .function( "CyanF", &ISample::CyanF )
        .function( "MagentaF", &ISample::MagentaF )
        .function( "YellowF", &ISample::YellowF )
        .function( "KeyF", &ISample::KeyF )
        .function( "LumaF", &ISample::LumaF )
        .function( "UF", &ISample::UF )
        .function( "VF", &ISample::VF )
        .function( "KF", &ISample::KF )
        .function( "XF", &ISample::XF )
        .function( "YF", &ISample::YF )
        .function( "ZF", &ISample::ZF )
        .function( "LF", &ISample::LF )
        .function( "aF", &ISample::aF )
        .function( "bF", &ISample::bF )
        .function( "xF", &ISample::xF )
        .function( "yF", &ISample::yF )
        .function( "SetGrey8", &ISample::SetGrey8 )
        .function( "SetRed8", &ISample::SetRed8 )
        .function( "SetGreen8", &ISample::SetGreen8 )
        .function( "SetBlue8", &ISample::SetBlue8 )
        .function( "SetR8", &ISample::SetR8 )
        .function( "SetG8", &ISample::SetG8 )
        .function( "SetB8", &ISample::SetB8 )
        .function( "SetHue8", &ISample::SetHue8 )
        .function( "SetSaturation8", &ISample::SetSaturation8 )
        .function( "SetLightness8", &ISample::SetLightness8 )
        .function( "SetValue8", &ISample::SetValue8 )
        .function( "SetCyan8", &ISample::SetCyan8 )
        .function( "SetMagenta8", &ISample::SetMagenta8 )
        .function( "SetYellow8", &ISample::SetYellow8 )
        .function( "SetKey8", &ISample::SetKey8 )
        .function( "SetLuma8", &ISample::SetLuma8 )
        .function( "SetU8", &ISample::SetU8 )
        .function( "SetV8", &ISample::SetV8 )
        .function( "SetK8", &ISample::SetK8 )
        .function( "SetX8", &ISample::SetX8 )
        .function( "SetY8", &ISample::SetY8 )
        .function( "SetZ8", &ISample::SetZ8 )
        .function( "SetL8", &ISample::SetL8 )
        .function( "Seta8", &ISample::Seta8 )
        .function( "Setb8", &ISample::Setb8 )
        .function( "Setx8", &ISample::Setx8 )
        .function( "Sety8", &ISample::Sety8 )
        .function( "SetGrey16", &ISample::SetGrey16 )
        .function( "SetRed16", &ISample::SetRed16 )
        .function( "SetGreen16", &ISample::SetGreen16 )
        .function( "SetBlue16", &ISample::SetBlue16 )
        .function( "SetR16", &ISample::SetR16 )
        .function( "SetG16", &ISample::SetG16 )
        .function( "SetB16", &ISample::SetB16 )
        .function( "SetHue16", &ISample::SetHue16 )
        .function( "SetSaturation16", &ISample::SetSaturation16 )
        .function( "SetLightness16", &ISample::SetLightness16 )
        .function( "SetValue16", &ISample::SetValue16 )
        .function( "SetCyan16", &ISample::SetCyan16 )
        .function( "SetMagenta16", &ISample::SetMagenta16 )
        .function( "SetYellow16", &ISample::SetYellow16 )
        .function( "SetKey16", &ISample::SetKey16 )
        .function( "SetLuma16", &ISample::SetLuma16 )
        .function( "SetU16", &ISample::SetU16 )
        .function( "SetV16", &ISample::SetV16 )
        .function( "SetK16", &ISample::SetK16 )
        .function( "SetX16", &ISample::SetX16 )
        .function( "SetY16", &ISample::SetY16 )
        .function( "SetZ16", &ISample::SetZ16 )
        .function( "SetL16", &ISample::SetL16 )
        .function( "Seta16", &ISample::Seta16 )
        .function( "Setb16", &ISample::Setb16 )
        .function( "Setx16", &ISample::Setx16 )
        .function( "Sety16", &ISample::Sety16 )
        .function( "SetGreyF", &ISample::SetGreyF )
        .function( "SetRedF", &ISample::SetRedF )
        .function( "SetGreenF", &ISample::SetGreenF )
        .function( "SetBlueF", &ISample::SetBlueF )
        .function( "SetRF", &ISample::SetRF )
        .function( "SetGF", &ISample::SetGF )
        .function( "SetBF", &ISample::SetBF )
        .function( "SetHueF", &ISample::SetHueF )
        .function( "SetSaturationF", &ISample::SetSaturationF )
        .function( "SetLightnessF", &ISample::SetLightnessF )
        .function( "SetValueF", &ISample::SetValueF )
        .function( "SetCyanF", &ISample::SetCyanF )
        .function( "SetMagentaF", &ISample::SetMagentaF )
        .function( "SetYellowF", &ISample::SetYellowF )
        .function( "SetKeyF", &ISample::SetKeyF )
        .function( "SetLumaF", &ISample::SetLumaF )
        .function( "SetUF", &ISample::SetUF )
        .function( "SetVF", &ISample::SetVF )
        .function( "SetKF", &ISample::SetKF )
        .function( "SetXF", &ISample::SetXF )
        .function( "SetYF", &ISample::SetYF )
        .function( "SetZF", &ISample::SetZF )
        .function( "SetLF", &ISample::SetLF )
        .function( "SetaF", &ISample::SetaF )
        .function( "SetbF", &ISample::SetbF )
        .function( "SetxF", &ISample::SetxF )
        .function( "SetyF", &ISample::SetyF )
        .function( "ToFormat", &ISample::ToFormat )
        .class_function( "ConvertFormat", &ISample::ConvertFormat )
        .class_function( "MixFormat", &ISample::MixFormat )
        .class_function( "MixRGB", &ISample::MixRGB )
        .class_function( "MixLab", &ISample::MixLab )
        .function( "Premultiply", &ISample::Premultiply )
        .function( "Unpremultiply", &ISample::Unpremultiply )
        .function( "Premultiplied", &ISample::Premultiplied )
        .function( "Unpremultiplied", &ISample::Unpremultiplied );



    /////////
    // FColor
    class_< FColor, base< ISample > >( "FColor" )
        .constructor( optional_override( []( eFormat iFormat ) { return  FColor( iFormat ); } ) )
        .constructor( optional_override( []( uint8* iData, eFormat iFormat ) { return  FColor( iData, iFormat ); } ), allow_raw_pointers() )
        .class_function( "FromPixel", optional_override( []( const FPixel& iValue ) { return  FColor( iValue ); } ) )
        .class_function( "FromColor", optional_override( []( const FColor& iValue ) { return  FColor( iValue ); } ) )
        .class_function( "FromSample", optional_override( []( const ISample& iValue ) { return  FColor( iValue ); } ) )
        .class_function( "RGB", &FColor::RGB )
        .class_function( "CMYA16", &FColor::CMYA16 )
        .class_function( "CMYA8", &FColor::CMYA8 )
        .class_function( "CMYAF", &FColor::CMYAF )
        .class_function( "CMYKA16", &FColor::CMYKA16 )
        .class_function( "CMYKA8", &FColor::CMYKA8 )
        .class_function( "CMYKAF", &FColor::CMYKAF )
        .class_function( "GreyA16", &FColor::GreyA16 )
        .class_function( "GreyA8", &FColor::GreyA8 )
        .class_function( "GreyAF", &FColor::GreyAF )
        .class_function( "HSLA16", &FColor::HSLA16 )
        .class_function( "HSLA8", &FColor::HSLA8 )
        .class_function( "HSLAF", &FColor::HSLAF )
        .class_function( "HSVA16", &FColor::HSVA16 )
        .class_function( "HSVA8", &FColor::HSVA8 )
        .class_function( "HSVAF", &FColor::HSVAF )
        .class_function( "LabA16", &FColor::LabA16 )
        .class_function( "LabA8", &FColor::LabA8 )
        .class_function( "LabAF", &FColor::LabAF )
        .class_function( "RGB", &FColor::RGB )
        .class_function( "RGBA16", &FColor::RGBA16 )
        .class_function( "RGBA8", &FColor::RGBA8 )
        .class_function( "RGBAF", &FColor::RGBAF )
        .class_function( "XYZA16", &FColor::XYZA16 )
        .class_function( "XYZA8", &FColor::XYZA8 )
        .class_function( "XYZAF", &FColor::XYZAF )
        .class_function( "YUVA16", &FColor::YUVA16 )
        .class_function( "YUVA8", &FColor::YUVA8 )
        .class_function( "YUVAF", &FColor::YUVAF )
        .class_function( "YxyA16", &FColor::YxyA16 )
        .class_function( "YxyA8", &FColor::YxyA8 )
        .class_function( "YxyAF", &FColor::YxyAF )
        .class_property( "Black", (const FColor*)&FColor::Black )
        .class_property( "White", (const FColor*)&FColor::White )
        .class_property( "Red", (const FColor*)&FColor::Red )
        .class_property( "Green", (const FColor*)&FColor::Green )
        .class_property( "Blue", (const FColor*)&FColor::Blue )
        .class_property( "Yellow", (const FColor*)&FColor::Yellow )
        .class_property( "Magenta", (const FColor*)&FColor::Magenta )
        .class_property( "Cyan", (const FColor*)&FColor::Cyan )
        .class_property( "Transparent", (const FColor*)&FColor::Transparent );



    /////////
    // FPixel
    class_< FPixel, base< ISample > >( "FPixel" )
        .constructor( optional_override( []( uint8* iData, eFormat iFormat ) { return  FPixel( iData, iFormat ); } ), allow_raw_pointers() )
        .class_function( "FromPixel", optional_override( []( const FPixel& iValue ) { return  FPixel( iValue ); } ) )
        .class_function( "FromColor", optional_override( []( const FColor& iValue ) { return  FPixel( iValue ); } ) )
        .class_function( "FromSample", optional_override( []( const ISample& iValue ) { return  FPixel( iValue ); } ) )
        .function( "SetPointer", &FPixel::SetPointer, allow_raw_pointers() )
        .function( "Next", &FPixel::Next )
        .function( "Prev", &FPixel::Prev );



    /////////
    // FBlock
    class_< FBlock, base< IHasFormat > >( "FBlock" )
        .constructor<
              ULIS::uint16
            , ULIS::uint16
            , eFormat
            , const FColorSpace*
            , const FOnInvalidBlock&
            , const FOnCleanupData&
        >
        ( allow_raw_pointers() )
        .constructor<
              uint8*
            , ULIS::uint16
            , ULIS::uint16
            , eFormat
            , const FColorSpace*
            , const FOnInvalidBlock&
            , const FOnCleanupData&
        >
        ( allow_raw_pointers() )
        .constructor(
            optional_override(
                [](
                      ULIS::uint16 iWidth
                    , ULIS::uint16 iHeight
                )
                {
                    return  new FBlock( iWidth, iHeight );
                }
            )
        )
        .constructor(
            optional_override(
                [](
                      ULIS::uint16 iWidth
                    , ULIS::uint16 iHeight
                    , eFormat iFormat
                )
                {
                    return  new FBlock( iWidth, iHeight, iFormat );
                }
            )
        )
        .function( "JSCanvasCompatibleData", optional_override( []( const FBlock& iSelf ){ return val( typed_memory_view( iSelf.BytesTotal(), iSelf.Bits() ) ); } ) )
        .function( "Area", &FBlock::Area )
        .function( "Bits", select_overload< uint8*() >( &FBlock::Bits ), allow_raw_pointers() )
        .function( "BytesPerScanLine", &FBlock::BytesPerScanLine )
        .function( "BytesTotal", &FBlock::BytesTotal )
        .function( "Color", &FBlock::Color )
        .function( "Dirty", select_overload< void( bool )const >( &FBlock::Dirty) )
        .function( "Dirty", select_overload< void ( const FRectI&, bool ) const >( &FBlock::Dirty ) )
        .function( "Height", &FBlock::Height )
        .function( "IsHollow", &FBlock::IsHollow )
        .function( "OnCleanup", &FBlock::OnCleanup )
        .function( "OnInvalid", &FBlock::OnInvalid )
        .function( "Pixel", select_overload< FPixel ( uint16, uint16 ) >( &FBlock::Pixel ) )
        .function( "PixelBits", select_overload< uint8* ( uint16, uint16 ) >( &FBlock::PixelBits ), allow_raw_pointers() )
        .function( "Rect", &FBlock::Rect )
        .function( "LoadFromData", &FBlock::LoadFromData, allow_raw_pointers() )
        .function( "ReallocInternalData", &FBlock::ReallocInternalData, allow_raw_pointers() )
        .function( "Rect", &FBlock::Rect )
        .function( "Sample", &FBlock::Sample )
        .function( "SampleSubpixel", &FBlock::SampleSubpixel )
        .function( "ScanlineBits", select_overload< uint8*( uint16 ) >( &FBlock::ScanlineBits ), allow_raw_pointers() )
        .function( "SetPixel", &FBlock::SetPixel )
        .function( "Width", &FBlock::Width );



    /////////
    // FKernel
    class_< FKernel, base< FBlock > >( "FKernel" )
        .constructor< const FVec2I&, float >()
        .constructor< const FKernel& >()
        .function( "At", select_overload< float ( int, int ) const >( &FKernel::At ) )
        .function( "At", select_overload< float ( const FVec2I& ) const >( &FKernel::At ) )
        .function( "SetAt", select_overload< void ( int, int, float ) >( &FKernel::SetAt ) )
        .function( "SetAt", select_overload< void ( const FVec2I&, float ) >( &FKernel::SetAt ) )
        .function( "Clear", &FKernel::Clear )
        .function( "Fill", &FKernel::Fill )
        .function( "SetZeroes", &FKernel::SetZeroes )
        .function( "SetOnes", &FKernel::SetOnes )
        .function( "Sum", &FKernel::Sum )
        .function( "Add", &FKernel::Add )
        .function( "Mul", &FKernel::Mul )
        .function( "Normalize", &FKernel::Normalize )
        .function( "IsNormalized", &FKernel::IsNormalized )
        .function( "FlipX", &FKernel::FlipX )
        .function( "FlipY", &FKernel::FlipY )
        .function( "Rotate90CW", &FKernel::Rotate90CW )
        .function( "Rotate90CCW", &FKernel::Rotate90CCW )
        .function( "Rotate180", &FKernel::Rotate180 )
        .function( "Normalized", &FKernel::Normalized )
        .function( "FlippedX", &FKernel::FlippedX )
        .function( "FlippedY", &FKernel::FlippedY )
        .function( "Rotated90CW", &FKernel::Rotated90CW )
        .function( "Rotated90CCW", &FKernel::Rotated90CCW )
        .function( "Rotated180", &FKernel::Rotated180 )
        .function( "Size", &FKernel::Size )
        .function( "Pivot", &FKernel::Pivot )
        .function( "SetPivot", &FKernel::SetPivot )
        .class_property( "Identity",       ( const FKernel* )&FKernel::Identity      )
        .class_property( "Edge4",          ( const FKernel* )&FKernel::Edge4         )
        .class_property( "Edge8",          ( const FKernel* )&FKernel::Edge8         )
        .class_property( "Sharpen",        ( const FKernel* )&FKernel::Sharpen       )
        .class_property( "BoxBlur",        ( const FKernel* )&FKernel::BoxBlur       )
        .class_property( "GaussianBlur",   ( const FKernel* )&FKernel::GaussianBlur  )
        .class_property( "UnsharpMask",    ( const FKernel* )&FKernel::UnsharpMask   );



    /////////
    // FStructuringElement
    class_< FStructuringElement, base< FBlock > >( "FStructuringElement" )
        .constructor< const FVec2I&, eMorphologicalElementValue >()
        .constructor< const FStructuringElement& >()
        .function( "At", select_overload< eMorphologicalElementValue ( int, int ) const >( &FStructuringElement::At ) )
        .function( "At", select_overload< eMorphologicalElementValue ( const FVec2I& ) const >( &FStructuringElement::At ) )
        .function( "SetAt", select_overload< void ( int, int, eMorphologicalElementValue ) >( &FStructuringElement::SetAt ) )
        .function( "SetAt", select_overload< void ( const FVec2I&, eMorphologicalElementValue ) >( &FStructuringElement::SetAt ) )
        .function( "Clear", &FStructuringElement::Clear )
        .function( "Fill", &FStructuringElement::Fill )
        .function( "SetZeroes", &FStructuringElement::SetZeroes )
        .function( "SetOnes", &FStructuringElement::SetOnes )
        .function( "FlipX", &FStructuringElement::FlipX )
        .function( "FlipY", &FStructuringElement::FlipY )
        .function( "Rotate90CW", &FStructuringElement::Rotate90CW )
        .function( "Rotate90CCW", &FStructuringElement::Rotate90CCW )
        .function( "Rotate180", &FStructuringElement::Rotate180 )
        .function( "FlippedX", &FStructuringElement::FlippedX )
        .function( "FlippedY", &FStructuringElement::FlippedY )
        .function( "Rotated90CW", &FStructuringElement::Rotated90CW )
        .function( "Rotated90CCW", &FStructuringElement::Rotated90CCW )
        .function( "Rotated180", &FStructuringElement::Rotated180 )
        .function( "Size", &FStructuringElement::Size )
        .function( "Pivot", &FStructuringElement::Pivot )
        .function( "SetPivot", &FStructuringElement::SetPivot );



    /////////
    // FFontStyleEntry
    class_< FFontStyleEntry >( "FFontStyleEntry" )
        .constructor< const std::string&, const std::string&, const std::string& >()
        .function( "Family", &FFontStyleEntry::Family )
        .function( "Style", &FFontStyleEntry::Style )
        .function( "Path", &FFontStyleEntry::Path );



    /////////
    // FFontFamilyEntry
    class_< FFontFamilyEntry >( "FFontFamilyEntry" )
        .constructor< const std::string& >()
        .function( "AddFontStyleKey", &FFontFamilyEntry::AddFontStyleKey )
        .function( "StyleCount", &FFontFamilyEntry::StyleCount )
        .function( "Styles", &FFontFamilyEntry::Styles )
        .function( "Family", &FFontFamilyEntry::Family )
        .function( "FuzzyFindFontStyleKey", &FFontFamilyEntry::FuzzyFindFontStyleKey, allow_raw_pointers() );



    /////////
    // FFontEngine
    class_< FFontEngine >( "FFontEngine" )
        .constructor<>()
        .function( "LibraryHandle", &FFontEngine::LibraryHandle, allow_raw_pointers() )
        .function( "AddLookupPath", &FFontEngine::AddLookupPath )
        .function( "AddLookupPaths", &FFontEngine::AddLookupPaths )
        .function( "Refresh", &FFontEngine::Refresh )
        .function( "FamilyCount", &FFontEngine::FamilyCount )
        .function( "Records", &FFontEngine::Records )
        .function( "LookupPaths", &FFontEngine::LookupPaths )
        .function( "FuzzyFindFontFamily", &FFontEngine::FuzzyFindFontFamily, allow_raw_pointers() )
        .function( "FuzzyFindFontStyle", &FFontEngine::FuzzyFindFontStyle, allow_raw_pointers() )
        .function( "FuzzyFindFontPath", &FFontEngine::FuzzyFindFontPath );



    /////////
    // FFont
    class_< FFont >( "FFont" )
        .constructor< const FFontEngine&, const std::string&, const std::string& >()
        //.constructor< const FFontEngine&, const FFontStyleEntry* >() // Cannot use this because we can't overload by type, only by parameter count.
        .constructor< const FFontEngine&, const std::string& >()
        .constructor< const FFont& >()
        .function( "FontHandle", &FFont::FontHandle, allow_raw_pointers() )
        //.function( "FontEngine", &FFont::FontEngine ) // Cannot use this since copy constructor and copy assignment are deleted.
        .function( "Family", &FFont::Family )
        .function( "Style", &FFont::Style );



    /////////
    // FColorStep
    class_< FColorStep >( "FColorStep" )
        .constructor<>()
        .constructor< ufloat, const FColor& >()
        .class_function( "MakeShared", &FColorStep::MakeShared )
        .function( "Param", select_overload< void ( ufloat ) >( &FColorStep::Param ) )
        .function( "Param", select_overload< ufloat () const >( &FColorStep::Param ) )
        .function( "Value", select_overload< FColor& () >( &FColorStep::Value ) );



    /////////
    // FGradient
    class_< FGradient >( "FGradient" )
        .constructor< eFormat >()
        //.function( "ColorSteps", select_overload< TArray< FSharedColorStep >& () >( &FGradient::ColorSteps ) )
        //.function( "AlphaSteps", select_overload< TArray< FSharedAlphaStep >& () >( &FGradient::AlphaSteps ) )
        .function( "Reset", select_overload< void () >( &FGradient::Reset ) )
        .function( "Reset", select_overload< void ( eFormat ) >( &FGradient::Reset ) )
        .function( "ReinterpretInterpolationFormat", &FGradient::ReinterpretInterpolationFormat )
        .function( "AddColorStep", &FGradient::AddColorStep )
        .function( "AddAlphaStep", &FGradient::AddAlphaStep )
        .function( "Sort", &FGradient::Sort );



    /////////
    // FSanitizedGradient
    class_< FSanitizedGradient >( "FSanitizedGradient" )
        .constructor< eFormat, const FGradient& >()
        //.function( "ColorSteps", &FSanitizedGradient::ColorSteps )
        //.function( "AlphaSteps", &FSanitizedGradient::AlphaSteps )
        .function( "IndexLUTColor", &FSanitizedGradient::IndexLUTColor, allow_raw_pointers() )
        .function( "IndexLUTAlpha", &FSanitizedGradient::IndexLUTAlpha, allow_raw_pointers() )
        .function( "ReinterpretInterpolationFormat", &FSanitizedGradient::ReinterpretInterpolationFormat )
        .function( "FastColorIndexAtParameter", &FSanitizedGradient::FastColorIndexAtParameter )
        .function( "FastAlphaIndexAtParameter", &FSanitizedGradient::FastAlphaIndexAtParameter );



    /////////
    // FContext
    // using regexp:
    // (FContext::)(.+)( >\w+)
    // $1$3$2 >\(\) \)
    class_< FContext >( "FContext" )
        .constructor< FCommandQueue&, eFormat, ePerformanceIntent >()
        .function( "Flush", &FContext::Flush )
        .function( "Finish", &FContext::Finish )
        .function( "Fence", &FContext::Fence )
        .function( "Wait", &FContext::Wait )
        .function( "Format", &FContext::Format )
        //.function( "FontEngine", static_cast< FFontEngine& ( FContext::* )() >( &FContext::FontEngine ) )
        .function( "FinishEventNo_OP", &FContext::FinishEventNo_OP, allow_raw_pointers() )
        .function( "Dummy_OP", &FContext::Dummy_OP, allow_raw_pointers() )
        .function( "AccumulateSample", ctxCallAdapter< WULIS_FUNK( &FContext::AccumulateSample ), const FBlock&, FColor*, const FRectI& >(), allow_raw_pointers() )
        .function( "AlphaBlend", ctxCallAdapter< WULIS_FUNK( &FContext::AlphaBlend ), const FBlock&, FBlock&, const FRectI&, const FVec2I&, ufloat >() )
        .function( "AlphaBlendAA", ctxCallAdapter< WULIS_FUNK( &FContext::AlphaBlendAA ), const FBlock&, FBlock&, const FRectI&, const FVec2F&, ufloat >() )
        .function( "AnalyzeSmallestVisibleRect", ctxCallAdapter< WULIS_FUNK( &FContext::AnalyzeSmallestVisibleRect ), const FBlock&, FRectI*, const FRectI& >(), allow_raw_pointers() )
        .class_function( "BezierDisplacementFieldMetrics", &FContext::BezierDisplacementFieldMetrics )
        .class_function( "BezierDisplacementMaskMetrics", &FContext::BezierDisplacementMaskMetrics )
        .function( "Blend", ctxCallAdapter< WULIS_FUNK( &FContext::Blend ), const FBlock&, FBlock&, const FRectI&, const FVec2I&, eBlendMode, eAlphaMode, ufloat >() )
        .function( "BlendAA", ctxCallAdapter< WULIS_FUNK( &FContext::BlendAA ), const FBlock&, FBlock&, const FRectI&, const FVec2F&, eBlendMode, eAlphaMode, ufloat >() )
        .function( "BlendColor", ctxCallAdapter< WULIS_FUNK( &FContext::BlendColor ), const ISample&, FBlock&, const FRectI&, eBlendMode, eAlphaMode, ufloat >() )
        .function( "BlendTiled", ctxCallAdapter< WULIS_FUNK( &FContext::BlendTiled ), const FBlock&, FBlock&, const FRectI&, const FRectI&, const FVec2I&, eBlendMode, eAlphaMode, ufloat >() )
        .function( "BrownianNoise", ctxCallAdapter< WULIS_FUNK( &FContext::BrownianNoise ), FBlock&, float, float, float, ULIS::uint8, int, const FRectI& >() )
        .function( "BuildSummedAreaTable", ctxCallAdapter< WULIS_FUNK( &FContext::BuildSummedAreaTable ), const FBlock&, FBlock& >() )
        .function( "Clear", ctxCallAdapter< WULIS_FUNK( &FContext::Clear ), FBlock&, const FRectI& >() )
        .function( "Clouds", ctxCallAdapter< WULIS_FUNK( &FContext::Clouds ), FBlock&, int, const FRectI& >() )
        .function( "ConvertFormat", ctxCallAdapter< WULIS_FUNK( &FContext::ConvertFormat ), const FBlock&, FBlock&, const FRectI&, const FVec2I& >() )
        .function( "Convolve", ctxCallAdapter< WULIS_FUNK( &FContext::Convolve ), const FBlock&, FBlock&, const FKernel&, const FRectI&, const FVec2I&, eResamplingMethod, eBorderMode, const ISample& >() )
        .function( "ConvolvePremult", ctxCallAdapter< WULIS_FUNK( &FContext::ConvolvePremult ), const FBlock&, FBlock&, const FKernel&, const FRectI&, const FVec2I&, eResamplingMethod, eBorderMode, const ISample& >() )
        .function( "Copy", ctxCallAdapter< WULIS_FUNK( &FContext::Copy ), const FBlock&, FBlock&, const FRectI&, const FVec2I& >() )
        .function( "DrawArc", ctxCallAdapter< WULIS_FUNK( &FContext::DrawArc ), FBlock&, const FVec2I&, const int, const int, const int, const FColor&, const FRectI& >() )
        .function( "DrawArcAA", ctxCallAdapter< WULIS_FUNK( &FContext::DrawArcAA ), FBlock&, const FVec2I&, const int, const int, const int, const FColor&, const FRectI& >() )
        .function( "DrawArcSP", ctxCallAdapter< WULIS_FUNK( &FContext::DrawArcSP ), FBlock&, const FVec2F&, const float, const int, const int, const FColor&, const FRectI& >() )
        .function( "DrawCircle", ctxCallAdapter< WULIS_FUNK( &FContext::DrawCircle ), FBlock&, const FVec2I&, const int, const FColor&, const bool,  const FRectI& >() )
        .function( "DrawCircleAA", ctxCallAdapter< WULIS_FUNK( &FContext::DrawCircleAA ), FBlock&, const FVec2I&, const int, const FColor&, const bool,  const FRectI& >() )
        .function( "DrawCircleSP", ctxCallAdapter< WULIS_FUNK( &FContext::DrawCircleSP ), FBlock&, const FVec2F&, const float, const FColor&, const bool,  const FRectI& >() )
        .function( "DrawEllipse", ctxCallAdapter< WULIS_FUNK( &FContext::DrawEllipse ), FBlock&, const FVec2I&, const int, const int, const FColor&, const bool, const FRectI& >() )
        .function( "DrawEllipseAA", ctxCallAdapter< WULIS_FUNK( &FContext::DrawEllipseAA ), FBlock&, const FVec2I&, const int, const int, const FColor&, const bool, const FRectI& >() )
        .function( "DrawEllipseSP", ctxCallAdapter< WULIS_FUNK( &FContext::DrawEllipseSP ), FBlock&, const FVec2F&, const float, const float, const FColor&, const bool, const FRectI& >() )
        .function( "DrawLine", ctxCallAdapter< WULIS_FUNK( &FContext::DrawLine ), FBlock&, const FVec2I&, const FVec2I&, const FColor&, const FRectI& >() )
        .function( "DrawLineAA", ctxCallAdapter< WULIS_FUNK( &FContext::DrawLineAA ), FBlock&, const FVec2I&, const FVec2I&, const FColor&, const FRectI& >() )
        .function( "DrawLineSP", ctxCallAdapter< WULIS_FUNK( &FContext::DrawLineSP ), FBlock&, const FVec2F&, const FVec2F&, const FColor&, const FRectI& >() )
        .function( "DrawPolygon", ctxCallAdapter< WULIS_FUNK( &FContext::DrawPolygon ), FBlock&, const std::vector< FVec2I >&, const FColor&, const bool, const FRectI& >() )
        .function( "DrawPolygonAA", ctxCallAdapter< WULIS_FUNK( &FContext::DrawPolygonAA ), FBlock&, const std::vector< FVec2I >&, const FColor&, const bool, const FRectI& >() )
        .function( "DrawPolygonSP", ctxCallAdapter< WULIS_FUNK( &FContext::DrawPolygonSP ), FBlock&, const std::vector< FVec2F >&, const FColor&, const bool, const FRectI& >() )
        .function( "DrawQuadraticBezier", ctxCallAdapter< WULIS_FUNK( &FContext::DrawQuadraticBezier ), FBlock&, const FVec2I&, const FVec2I&, const FVec2I&, const float, const FColor&, const FRectI& >() )
        .function( "DrawQuadraticBezierAA", ctxCallAdapter< WULIS_FUNK( &FContext::DrawQuadraticBezierAA ), FBlock&, const FVec2I&, const FVec2I&, const FVec2I&, const float, const FColor&, const FRectI& >() )
        .function( "DrawQuadraticBezierSP", ctxCallAdapter< WULIS_FUNK( &FContext::DrawQuadraticBezierSP ), FBlock&, const FVec2F&, const FVec2F&, const FVec2F&, const float, const FColor&, const FRectI& >() )
        .function( "DrawRectangle", ctxCallAdapter< WULIS_FUNK( &FContext::DrawRectangle ), FBlock&, const FVec2I&, const FVec2I&, const FColor&, const bool, const FRectI& >() )
        .function( "DrawRotatedEllipse", ctxCallAdapter< WULIS_FUNK( &FContext::DrawRotatedEllipse ), FBlock&, const FVec2I&, const int, const int, const int, const FColor&, const bool, const FRectI& >() )
        .function( "DrawRotatedEllipseAA", ctxCallAdapter< WULIS_FUNK( &FContext::DrawRotatedEllipseAA ), FBlock&, const FVec2I&, const int, const int, const int, const FColor&, const bool, const FRectI& >() )
        .function( "DrawRotatedEllipseSP", ctxCallAdapter< WULIS_FUNK( &FContext::DrawRotatedEllipseAA ), FBlock&, const FVec2F&, const float, const float, const int, const FColor&, const bool, const FRectI& >() )
        .function( "Dummy_OP", &FContext::Dummy_OP, allow_raw_pointers() )
        .function( "Extract", ctxCallAdapter< WULIS_FUNK( &FContext::Extract ), const FBlock&, FBlock&, ULIS::uint8, ULIS::uint8, bool, const FRectI&, const FVec2I& >() )
        .function( "Fill", ctxCallAdapter< WULIS_FUNK( &FContext::Fill ), FBlock&, const ISample&, const FRectI& >() )
        .function( "FillPreserveAlpha", ctxCallAdapter< WULIS_FUNK( &FContext::FillPreserveAlpha ), FBlock&, const ISample&, const FRectI& >() )
        .function( "FinishEventNo_OP", &FContext::FinishEventNo_OP, allow_raw_pointers() )
        .function( "LinearTosRGB", ctxCallAdapter< WULIS_FUNK( &FContext::LinearTosRGB ), FBlock&, const FRectI& >() )
        .class_function( "LoadBlockFromDiskMetrics", &FContext::LoadBlockFromDiskMetrics, allow_raw_pointers() )
        .class_function( "LoadPSDFromDiskMetrics", &FContext::LoadPSDFromDiskMetrics, allow_raw_pointers() )
        .class_function( "MaxMipLevelMetrics", &FContext::MaxMipLevelMetrics )
        .class_function( "MipLevelMetrics", &FContext::MipLevelMetrics )
        .class_function( "MipMapMetrics", &FContext::MipMapMetrics )
        .class_function( "MipRectsMetrics", &FContext::MipRectsMetrics, allow_raw_pointers() )
        .function( "MorphologicalProcess", ctxCallAdapter< WULIS_FUNK( &FContext::MorphologicalProcess ), const FBlock&, FBlock&, const FStructuringElement&, const FRectI&, const FVec2I&, eResamplingMethod, eBorderMode, const ISample& >() )
        .function( "Premultiply", ctxCallAdapter< WULIS_FUNK( &FContext::Premultiply ), FBlock&, const FRectI& >() )
        .function( "RasterGradient", ctxCallAdapter< WULIS_FUNK( &FContext::RasterGradient ), FBlock&, const FVec2F&, const FVec2F&, const FSanitizedGradient&, float, eGradientType, const FRectI& >() )
        .function( "RasterText", ctxCallAdapter< WULIS_FUNK( &FContext::RasterText ), FBlock&, const std::wstring&, const FFont&, ULIS::uint32, const FMat3F&, const ISample& >() )
        .function( "RasterTextAA", ctxCallAdapter< WULIS_FUNK( &FContext::RasterTextAA ), FBlock&, const std::wstring&, const FFont&, ULIS::uint32, const FMat3F&, const ISample& >() )
        .function( "Resize", ctxCallAdapter< WULIS_FUNK( &FContext::Resize ), const FBlock&, FBlock&, const FRectI&, const FRectF&, eResamplingMethod, eBorderMode, const ISample&, const FBlock* >(), allow_raw_pointers() )
        .function( "SanitizeZeroAlpha", ctxCallAdapter< WULIS_FUNK( &FContext::SanitizeZeroAlpha ), FBlock&, const FRectI& >() )
        .function( "SaveBlockToDisk", ctxCallAdapter< WULIS_FUNK( &FContext::SaveBlockToDisk ), const FBlock&, const std::string&, eFileFormat, int >() )
        .function( "sRGBToLinear", ctxCallAdapter< WULIS_FUNK( &FContext::sRGBToLinear ), FBlock&, const FRectI& >() )
        .class_function( "SummedAreaTableMetrics", &FContext::SummedAreaTableMetrics )
        .function( "Swap", ctxCallAdapter< WULIS_FUNK( &FContext::Swap ), FBlock&, ULIS::uint8, ULIS::uint8, const FRectI& >() )
        .class_function( "TextMetrics", &FContext::TextMetrics )
        .function( "TransformAffine", ctxCallAdapter< WULIS_FUNK( &FContext::TransformAffine ), const FBlock&, FBlock&, const FRectI&, const FMat3F&, eResamplingMethod, eBorderMode, const ISample& >() )
        .class_function( "TransformAffineMetrics", &FContext::TransformAffineMetrics )
        .function( "TransformAffineTiled", ctxCallAdapter< WULIS_FUNK( &FContext::TransformAffineTiled ), const FBlock&, FBlock&, const FRectI&, const FRectI&, const FMat3F&, eResamplingMethod, eBorderMode, const ISample& >() )
        .function( "TransformBezier", ctxCallAdapter< WULIS_FUNK( &FContext::TransformBezier ), const FBlock&, FBlock&, const TArray< FCubicBezierControlPoint >&, float, ULIS::uint32, const FRectI&, eResamplingMethod, eBorderMode, const ISample& >() )
        .class_function( "TransformBezierMetrics", &FContext::TransformBezierMetrics )
        .function( "TransformPerspective", ctxCallAdapter< WULIS_FUNK( &FContext::TransformPerspective ), const FBlock&, FBlock&, const FRectI&, const FMat3F&, eResamplingMethod, eBorderMode, const ISample& >() )
        .class_function( "TransformPerspectiveMetrics", &FContext::TransformBezierMetrics )
        .function( "Unpremultiply", ctxCallAdapter< WULIS_FUNK( &FContext::Unpremultiply ), FBlock&, const FRectI& >() )
        .function( "ValueNoise", ctxCallAdapter< WULIS_FUNK( &FContext::ValueNoise ), FBlock&, float, int, const FRectI& >() )
        .function( "VoronoiNoise", ctxCallAdapter< WULIS_FUNK( &FContext::VoronoiNoise ), FBlock&, ULIS::uint32, int, const FRectI& >() )
        .function( "Wait", &FContext::Wait )
        .function( "WhiteNoise", ctxCallAdapter< WULIS_FUNK( &FContext::WhiteNoise ), FBlock&, int, const FRectI& >() )
        .function( "XAllocateBlockData", ctxCallAdapter< WULIS_FUNK( &FContext::XAllocateBlockData ), FBlock&, ULIS::uint16, ULIS::uint16, eFormat, const FColorSpace*, const FOnInvalidBlock&, const FOnCleanupData& >(), allow_raw_pointers() )
        .function( "XBuildMipMap", ctxCallAdapter< WULIS_FUNK( &FContext::XBuildMipMap ), const FBlock&, FBlock&, int, const FRectI&, eResamplingMethod >() )
        .function( "XCreateTestBlock", ctxCallAdapter< WULIS_FUNK( &FContext::XCreateTestBlock ), FBlock& >() )
        .function( "XDeallocateBlockData", ctxCallAdapter< WULIS_FUNK( &FContext::XDeallocateBlockData ), FBlock& >() )
        .function( "XLoadBlockFromDisk", ctxCallAdapter< WULIS_FUNK( &FContext::XLoadBlockFromDisk ), FBlock&, const std::string& >() )
        .function( "XProcessBezierDisplacementField", ctxCallAdapter< WULIS_FUNK( &FContext::XProcessBezierDisplacementField ), const FBlock&, const FBlock&, FBlock&, FBlock&, const TArray< FCubicBezierControlPoint >&, float, ULIS::uint32, const FRectI&, eResamplingMethod, eBorderMode, const ISample& >() );
}

