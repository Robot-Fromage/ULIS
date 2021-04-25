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
#include <ULIS>
#include <pybind11/pybind11.h>
using namespace ::ULIS;
using namespace pybind11::literals;
namespace py = pybind11;

PYBIND11_MODULE( pyULIS4, m ) {
    m.doc() = "pyULIS4 module, a python binding for ULIS.";

    py::enum_< eColorModel >( m, "eColorModel", py::arithmetic() )
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

    py::enum_< eType >( m, "eType", py::arithmetic() )
        .value( "Type_uint8",   eType::Type_uint8   )
        .value( "Type_uint16",  eType::Type_uint16  )
        .value( "Type_ufloat",  eType::Type_ufloat  );

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

    py::class_< FFormatMetrics >( m, "FFormatMetrics" )
        .def( py::init< eFormat >(), "format"_a )
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

    py::class_< FColorSpace >( m, "FColorSpace" )
        .def( py::init<>() );

    py::class_< FOnInvalidBlock >( m, "FOnInvalidBlock" )
        .def( py::init<>() )
        .def( py::init< FOnInvalidBlock::tFptr, void* >(), "fptr"_a, "info"_a = nullptr )
        .def( "ExecuteIfBound", &FOnInvalidBlock::ExecuteIfBound )
        .def( "Execute", &FOnInvalidBlock::Execute );

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

    py::class_< FBlock >( m, "FBlock" )
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
        .def( "Area", &FBlock::Area )
        .def( "Bits", static_cast< uint8* ( FBlock::* )() >( &FBlock::Bits ) )
        .def( "BytesPerScanLine", &FBlock::BytesPerScanLine )
        .def( "BytesTotal", &FBlock::BytesTotal )
        .def( "Color", &FBlock::Color )
        .def( "Dirty", static_cast< void ( FBlock::* )( bool ) const >( &FBlock::Dirty), "call"_a = true )
        .def( "Dirty", static_cast< void ( FBlock::* )( const FRectI&, bool ) const >( &FBlock::Dirty ), "rect"_a, "call"_a = true )
        .def( "Height", &FBlock::Height )
        .def( "IsHollow", &FBlock::IsHollow )
        .def( "OnCleanup", &FBlock::OnCleanup, "callback"_a )
        .def( "OnInvalid", &FBlock::OnInvalid, "callback"_a )
        .def( "Pixel", static_cast< FPixel ( FBlock::* )( uint16, uint16 ) >( &FBlock::Pixel ) )
        .def( "PixelBits", static_cast< uint8* ( FBlock::* )( uint16, uint16 ) >( &FBlock::PixelBits ) )
        .def( "ReallocInternalData", &FBlock::ReallocInternalData )
        .def( "Rect", &FBlock::Rect )
        .def( "Sample", &FBlock::Sample )
        .def( "SampleSubpixel", &FBlock::SampleSubpixel )
        .def( "ScanlineBits", static_cast< uint8* ( FBlock::* )( uint16 ) >( &FBlock::ScanlineBits ) )
        .def( "SetPixel", &FBlock::SetPixel )
        .def( "Width", &FBlock::Width );
}
