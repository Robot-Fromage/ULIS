// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_GUI_Chromaticity.cpp
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
#include <ULIS/Color/ULIS.Color.ColorMatching.h>
#include "ULIS/Conv/ULIS.Conv.ConversionContext.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    ::ULIS::IBlock* block = ::ULIS::FMakeContext::MakeBlock( 512, 512, ::ULIS::FBlockRGBA8::TypeId() );
    ::ULIS::FClearFillContext::Fill( block, ::ULIS::CColor( 20, 20, 20 ) );
    cmsHPROFILE hInProfile;
    cmsHPROFILE hOutProfile;
    cmsHTRANSFORM hTransform;
    hInProfile  = cmsCreateXYZProfile();
    hOutProfile = cmsCreate_sRGBProfile();
    hTransform = cmsCreateTransform( hInProfile
                                   , TYPE_XYZ_DBL
                                   , hOutProfile
                                   , TYPE_RGBA_8
                                   , INTENT_PERCEPTUAL, 0 );
    cmsCloseProfile( hInProfile  );
    cmsCloseProfile( hOutProfile );

    for( int i = 60; i < 280; ++i ) // 1nanometer step 830 - 360 = 470
    {
        // CIE1931 chromaticity diagram
        cmsCIEXYZ XYZ = ::ULIS::Chroma::CMF_XYZ_CIE_2_1931_1nm[i];
        cmsCIExyY xyY;
        cmsXYZ2xyY( &xyY, &XYZ );
        int x = xyY.x * block->Width();
        int y = (1.0 - xyY.y) * block->Height();
        cmsDoTransform( hTransform, &XYZ, block->PixelPtr( x, y ), 1 );

        // Uniform chromaticity scale
        /*
        cmsCIEXYZ XYZ = ::ULIS::Chroma::CMF_XYZ_CIE_2_1931_1nm[i];
        double sum = ( XYZ.X + 15 * XYZ.Y + 3 * XYZ.Z );
        double u = ( 4 * XYZ.X ) / sum;
        double v = ( 9 * XYZ.Y ) / sum;
        int x = u * block->Width();
        int y = (1.0 - v) * block->Height();
        cmsDoTransform( hTransform, &XYZ, block->PixelPtr( x, y ), 1 );
        */
    }
    
    cmsDeleteTransform( hTransform );

    cmsHPROFILE hXYZProfile  = ::ULIS::FGlobalProfileRegistry::Get().GetDefaultProfileForModel( ::ULIS::e_cm::kXYZ )->ProfileHandle();
    cmsHPROFILE hCMYKProfile  = ::ULIS::FGlobalProfileRegistry::Get().GetDefaultProfileForModel( ::ULIS::e_cm::kCMYK )->ProfileHandle();
    cmsHPROFILE hsRGBProfile = ::ULIS::FGlobalProfileRegistry::Get().GetDefaultProfileForModel( ::ULIS::e_cm::kRGB )->ProfileHandle();
    cmsHPROFILE hLabProfile = ::ULIS::FGlobalProfileRegistry::Get().GetDefaultProfileForModel( ::ULIS::e_cm::kLab )->ProfileHandle();
    ::ULIS::uint8 rgb_buf[4] = { 255, 0, 0, 0 };
    ::ULIS::uint16 cmyk_buf[5] = { 0, 0, 0, 0 };
    float lab_buf[4] = { 0, 0, 0, 0 };
    //cmsHTRANSFORM trans = cmsCreateTransform( hCMYKProfile, ULIS_LCMS_DTYPE_CMYKA_FLT, hsRGBProfile, ULIS_LCMS_DTYPE_RGBA_FLT, INTENT_PERCEPTUAL, 0 );
    //cmsHTRANSFORM trans = cmsCreateTransform( hCMYKProfile, ULIS_LCMS_DTYPE_CMYKA_FLT, hsRGBProfile, TYPE_RGBA_8, INTENT_PERCEPTUAL, 0 );
    cmsHTRANSFORM trans = cmsCreateTransform( hsRGBProfile, TYPE_RGBA_8, hCMYKProfile, ULIS_LCMS_DTYPE_CMYKA_U16, INTENT_PERCEPTUAL, 0 );
    //cmsHTRANSFORM trans = cmsCreateTransform( hsRGBProfile, ULIS_LCMS_DTYPE_RGBA_FLT, hLabProfile, TYPE_LabA_FLT, INTENT_PERCEPTUAL, 0 );
    cmsDoTransform( trans, rgb_buf, cmyk_buf, 1 );
    ::ULIS::TPixelValue< ::ULIS::Format::Format_floatCMYKhasAlphaCMYKAnormalized > cmyk;
    cmyk.Set( 1.0, 0.0, 0.0, 0.0, 1.0 );
    ::ULIS::FValueBGRA8 rgb;
    ::ULIS::TConversionContext::Convert( cmyk, rgb );
    auto _r = rgb.R();
    auto _g = rgb.G();
    auto _b = rgb.B();
    auto _a = rgb.GetAlpha();
    auto breakpoint_dummy = 0;

    QImage* image   = new QImage( block->DataPtr(), block->Width(), block->Height(), block->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    QPixmap pixmap  = QPixmap::fromImage( *image );
    QWidget* w      = new QWidget();
    QLabel* label   = new QLabel( w );
    label->setPixmap( pixmap );

    w->resize( pixmap.size() );
    w->show();

    int exit_code = app.exec();

    delete label;
    delete image;
    delete w;
    delete block;

    return  exit_code;
}

