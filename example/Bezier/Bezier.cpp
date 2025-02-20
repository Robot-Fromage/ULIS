// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Bezier.cpp
* @author       Clement Berthaud
* @brief        Transform application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_AVX );

    FBlock* blockSRC = new FBlock();
    FBlock* blockDST = new FBlock( 1024, 1024, fmt );
    {
        std::string path = "C:/Users/PRAXINOS/Documents/work/TEST.png";
        ctx.XLoadBlockFromDisk( *blockSRC, path );
        ctx.Fill( *blockDST, FColor::RGB( 30, 255, 150 ), blockDST->Rect() );
    }
    ctx.Finish();

    int size = 256;
    int shift = 100;
    FVec2F A( shift, shift );
    FVec2F B( shift + size, shift );
    FVec2F C( shift + size, shift + size );
    FVec2F D( shift, shift + size );
    TArray< FCubicBezierControlPoint > ctrlpoints;
    ctrlpoints.PushBack( { A, A + FVec2F( 50, -50 ), A } );
    ctrlpoints.PushBack( { B, B, B } );
    ctrlpoints.PushBack( { C, C + FVec2F( -50, -50 ), C } );
    ctrlpoints.PushBack( { D, D + FVec2F( 50, -50 ), D } );
    {
        ctx.TransformBezier(
              *blockSRC
            , *blockDST
            , ctrlpoints
            , 0.5f
            , 1
            , blockSRC->Rect()
            , Resampling_Bilinear
            , Border_Transparent
            , FColor::Transparent
        );
    }
    ctx.Finish();

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( blockDST->Bits()
                                        , blockDST->Width()
                                        , blockDST->Height()
                                        , blockDST->BytesPerScanLine()
                                        , QImage::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();

    int exit_code = app.exec();

    delete  label;
    delete  image;
    delete  widget;

    delete  blockDST;
    delete  blockSRC;

    return  exit_code;
}

