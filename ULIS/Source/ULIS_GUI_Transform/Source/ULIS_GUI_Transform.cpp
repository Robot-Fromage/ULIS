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

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );
    ::ULIS::IBlock* blockA = ::ULIS::FMakeContext::MakeBlock( 16, 16, ::ULIS::FBlockRGBA8::TypeId() );
    ::ULIS::FClearFillContext::Fill( blockA, ::ULIS::CColor( 255, 0, 0 ) );
    for( int j = 0; j < blockA->Height(); ++j )
    {
        for( int i = 0; i < blockA->Width(); ++i )
        {
            ::ULIS::CColor col = ::ULIS::CColor::FromHSVF( i / float( blockA->Width() ), 1.f, 1.f );
            blockA->SetPixelColor( i, j, col );
        }
    }

    ::ULIS::FGlobalThreadPool::Get().SetNumWorkers( 1 );
    glm::mat3 transform = ::ULIS::FTransformContext::GetScaleMatrix( 20, 20 );
    ::ULIS::FPerformanceOptions opt;
    opt.desired_workers = 1;
    ::ULIS::IBlock* blockB = ::ULIS::FTransformContext::GetTransformed( blockA, transform, ::ULIS::eResamplingMethod::kBilinear, opt );

    QImage* image   = new QImage( blockB->DataPtr(), blockB->Width(), blockB->Height(), blockB->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
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
    delete blockA;
    delete blockB;

    return  exit_code;
}

