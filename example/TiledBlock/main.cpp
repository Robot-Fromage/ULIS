// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        TiledBlock application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <cstdlib>
#include <ctime>
using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    srand( time( NULL ) );

    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat format = Format_RGBA8;
    FContext ctx( queue, format );

    FTilePool tilePool( format );
    FTiledBlock blockA( tilePool );
    FTiledBlock blockB( tilePool );
    ctx.Fill( blockA, FColor::Red, FRectI( 0, 0, 128, 128 ) );
    ctx.Fill( blockB, FColor::Blue, FRectI( 0, 0, 128, 128 ) );
    ctx.Finish();
    ctx.Clear( blockA, FRectI( 32, 32, 64, 64 ) );
    ctx.Finish();
    ctx.Blend( blockB, blockA, FRectI( 0, 0, 128, 128 ), FVec2I( 16, 16 ), Blend_Normal, Alpha_Normal, 0.5f );
    ctx.Finish();

    //while( true ) {
    //    system("cls");
    //    tilePool.PrintDiagnosis();
    //    std::this_thread::sleep_for( std::chrono::duration< double, std::milli >( 1000 ) );
    //    blockA.SanitizeNow();
    //}


    blockA.RecomputeLeafGeometry();
    FRectI rect = blockA.LeafGeometry();
    FBlock result( rect.w, rect.h, format );
    ctx.Dump( blockA, result );
    ctx.Finish();

    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( result.Bits()
                                        , result.Width()
                                        , result.Height()
                                        , result.BytesPerScanLine()
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
    return  exit_code;
}

