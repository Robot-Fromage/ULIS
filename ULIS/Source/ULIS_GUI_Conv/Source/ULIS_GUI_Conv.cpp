// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_GUI_Conv.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <ULIS_CORE>
#include "ULIS/Conv/ULIS.Conv.ConversionContext.h"

int main( int argc, char *argv[] )
{
    ::ULIS::FValueRGBA8 valA;
    ::ULIS::FValueRGBA8 valB;
    valA.::ULIS::TPixelBase< ::ULIS::Format::Format_uint8RGBhasAlphaRGBAtypeLimits >::operator=( valB );

    ::ULIS::FValueRGBA8 valRGBA8;
    valRGBA8.AssignColorProfile( "sRGB" );
    valRGBA8.SetR( 255 );
    valRGBA8.SetG( 127 );
    valRGBA8.SetB( 50 );
    valRGBA8.SetAlpha( 255 );

    ::ULIS::FValueHSLAf valHSLAf;
    valHSLAf.AssignColorProfile( "sRGB" );

    using ::ULIS::uint8;
    ::ULIS::TConversionContext::Convert( valRGBA8, valHSLAf );
    float H = valHSLAf.H();
    float S = valHSLAf.S();
    float L = valHSLAf.L();
    float A = valHSLAf.GetAlpha();

    ::ULIS::CColor src = valRGBA8.GetColor();
    ::ULIS::CColor src_transform = src.ToHSL();
    ::ULIS::CColor dst = valHSLAf.GetColor();
    ::ULIS::CColor dst_transform = dst.ToRGB();
    int src_R = src.Red();
    int src_G = src.Green();
    int src_B = src.Blue();
    int src_transform_H = src_transform.HSLHue();
    int src_transform_S = src_transform.HSLSaturation();
    int src_transform_L = src_transform.Lightness();
    int dst_H = dst.HSLHue();
    int dst_S = dst.HSLSaturation();
    int dst_L = dst.Lightness();
    int dst_transform_R = dst_transform.Red();
    int dst_transform_G = dst_transform.Green();
    int dst_transform_B = dst_transform.Blue();

    uint8 check = 0.3f * 255 + 0.59f * 127 + 0.11 * 50;

    auto dummy = 0;

    /*
    //::ULIS::TConversionContext::ConvertTypeAndLayoutInto< ::ULIS::FBlockRGBA8::TypeId(), ::ULIS::FBlockRGBA8::TypeId() >( ::ULIS::PixelValueAutoCastChecked( block8, 0, 0 ), val );
    ::ULIS::FValueRGBA8 v1;
    v1.SetR( 255 );
    v1.SetG( 0 );
    v1.SetB( 0 );
    v1.SetAlpha( 255 );
    ::ULIS::FValueLabAfn v2;
    float lL = v2.L();
    float la = v2.a();
    float lb = v2.b();
    float lA = v2.GetAlpha();
    auto dummyx = 0;
    */

    return  0;
}

