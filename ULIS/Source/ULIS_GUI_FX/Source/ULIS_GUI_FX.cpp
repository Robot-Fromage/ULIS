// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_GUI_Transform.cpp
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
    using namespace ::ULIS;
    QApplication app( argc, argv );

    int width, height, channels;
    unsigned char *raw = stbi_load( "C:/Users/PRAXINOS/Documents/work/ULIS/Test/coboi_alpha.png", &width, &height, &channels, STBI_rgb_alpha );
    ::ULIS::IBlock* src = ::ULIS::FMakeContext::MakeBlockFromExternalDataTakeOwnership( width, height, raw, ::ULIS::Format::Format_RGBA8 );
    IBlock* dst = FMakeContext::MakeBlock( width, height, Format::Format_RGBA8 );

    /*
    FKernel kernel( FSize( 3, 3 )
                  , {  255,  255,  255
                    ,  255, -4080,  255
                    ,  255,  255,  255 } );
    */
    FKernel kernel( FSize( 3, 3 )
                  , {  1,  1,  1
                    ,  1, -8,  1
                    ,  1,  1,  1 } );
    FFXContext::Convolution( src, dst, kernel, true );

    QImage*     image   = new QImage( dst->DataPtr()
                                    , dst->Width()
                                    , dst->Height()
                                    , dst->BytesPerScanLine()
                                    , QImage::Format::Format_RGBA8888 );
    QPixmap     pixmap  = QPixmap::fromImage( *image );
    QWidget*    w       = new QWidget();
    QLabel*     label   = new QLabel( w );
    label->setPixmap( pixmap );

    w->resize( pixmap.size() );
    w->show();

    int exit_code = app.exec();

    delete label;
    delete image;
    delete w;
    delete src;
    delete dst;

    return  exit_code;
}

