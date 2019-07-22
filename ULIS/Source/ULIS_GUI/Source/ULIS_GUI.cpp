/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS_Test.cpp
* Clement Berthaud - Layl
* Please refer to LICENSE.md
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

    ::ULIS::IBlock* blockA = ::ULIS::FMakeContext::MakeBlock( 1024, 1024, ::ULIS::FBlockRGBA8::TypeId() );
    ::ULIS::IBlock* blockB = ::ULIS::FMakeContext::MakeBlock( 1024, 1024, ::ULIS::FBlockRGBA8::TypeId() );
    blockA->Fill( ::ULIS::CColor::FromRGB( 255, 0, 0, 255 ) );

    for( int i = 0; i < blockA->Height(); ++i )
        for( int j = 0; j < blockA->Width(); ++j )
            blockA->SetPixelColor( j, i, ::ULIS::CColor::FromHSLF( j / (float)blockA->Width(), 1.f, i / (float)blockA->Height() ) );

    ::ULIS::FClearFillContext::Fill( blockB, ::ULIS::CColor( 255, 255, 255 ) );
    ::ULIS::FBlendingContext::Blend( blockB, blockA, ::ULIS::eBlendingMode::kNormal, 0.5f );

    QImage* image   = new QImage( blockA->DataPtr(), blockA->Width(), blockA->Height(), blockA->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
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

