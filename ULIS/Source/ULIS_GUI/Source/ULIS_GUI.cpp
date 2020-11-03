// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_GUI.cpp
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
#define STB_IMAGE_IMPLEMENTATION
#include <ULIS/stb/stb_image.h>

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    int width   = 1024;
    int height  = 1024;
    ::ULIS::IBlock* block   = ::ULIS::FMakeContext::MakeBlock( width, height, ::ULIS::Format::Format_uint16RGBhasAlphaRGBAtypeLimits );
    ::ULIS::IBlock* overlay = ::ULIS::FMakeContext::MakeBlock( width, height, ::ULIS::Format::Format_uint16RGBhasAlphaRGBAtypeLimits );
    ::ULIS::FClearFillContext::Fill( block, ::ULIS::CColor( 255, 0, 0 ) );
    ::ULIS::FClearFillContext::Fill( overlay, ::ULIS::CColor( 255, 255, 0 ) );
    ::ULIS::FBlendingContext::Blend( overlay, block, 0, 0, ::ULIS::eBlendingMode::kNormal, ::ULIS::eAlphaMode::kNormal, 0.5f );

    ::ULIS::TPixelValue< ::ULIS::Format::Format_uint16RGBhasAlphaRGBAtypeLimits > val = ::ULIS::PixelProxyAutoCastChecked( block, 0, 0 );
    ::ULIS::uint16 R = val.R();
    ::ULIS::uint16 G = val.G();
    ::ULIS::uint16 B = val.G();
    ::ULIS::uint16 A = val.GetAlpha();

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
    delete overlay;

    return  exit_code;
}

