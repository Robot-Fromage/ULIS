// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_GUI_Painter.cpp
 * @author      Thomas Schmitt
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

    ::ULIS::IBlock* block = ::ULIS::FMakeContext::MakeBlock( 1024, 1024, ::ULIS::FBlockRGBA8::TypeId() );
    ::ULIS::FClearFillContext::Fill( block, ::ULIS::CColor( 255, 255, 255, 0 ) );

    
    //Vertical
    //::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(500,50), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 255 ,0 ) );
    //::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(500,950), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 255 ,0 ) );

    //Horizontal
    //::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(50,500), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 255 ,0 ) );
    //::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(950,500), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 255 ,0 ) );
     /*
    //x slope
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(100,300), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,255 ), true );
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(900,300), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,255 ), true );
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(100,700), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,255 ), true );
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(900,700), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,255 ), true );
    
    //y slope
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(300,100), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 255, 0 ,0 ), true );
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(700,100), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 255, 0 ,0 ), true );
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(300,900), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 255, 0 ,0 ), true );
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(700,900), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 255, 0 ,0 ), true );
    
    
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(750,950), ::ULIS::FPoint(753, 950), ::ULIS::CColor( 255, 0 ,0 ), true );
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(750,950), ::ULIS::FPoint(750, 953), ::ULIS::CColor( 255, 0 ,0 ), true );

    
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(250,950), ::ULIS::FPoint(247, 950), ::ULIS::CColor( 255, 0 ,0 ), true );
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(250,950), ::ULIS::FPoint(250, 947), ::ULIS::CColor( 255, 0 ,0 ), true );*/

    ::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(300,400), ::ULIS::FPoint(750, 600), ::ULIS::CColor( 255, 0 ,0 ), ULIS::FRect(500, 500, 200, 200) );
    ::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(-20,2), ::ULIS::FPoint(600, 1500), ::ULIS::CColor( 255, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(600, -500), ::ULIS::FPoint(900, 800), ::ULIS::CColor( 255, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(-500,500), ::ULIS::FPoint(1500, 600), ::ULIS::CColor( 255, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(500,1100), ::ULIS::FPoint(1100, 500), ::ULIS::CColor( 255, 0 ,0 ) );

    //::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(3,6), ::ULIS::FPoint(30, 8), ::ULIS::CColor( 255, 0 ,0 ) );
    
    //::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(50,50), ::ULIS::FPoint(50, 55), ::ULIS::CColor( 255, 0 ,0 ) );
    
    /*::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(51,64), ::ULIS::FPoint(750, 354), ::ULIS::CColor( 255, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(654,41), ::ULIS::FPoint(25, 200), ::ULIS::CColor( 255, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(275, 28), ::ULIS::FPoint(465, 888), ::ULIS::CColor( 0, 255 ,0 ) );
    
    ::ULIS::FPainterContext::DrawLineAA( block, ::ULIS::FPoint(275, 888), ::ULIS::FPoint(465, 25), ::ULIS::CColor( 0, 255 ,0 ) );*/
    
    //circles
    /*
    for( int i = 0; i < 255; i+=5)
    {
        ::ULIS::FPainterContext::DrawCircleBresenham( block, ::ULIS::FPoint(500,500), i, ::ULIS::CColor( 0, i ,0 ) );
    }*/

    /*::ULIS::FPainterContext::DrawCircleAndresAA( block, ::ULIS::FPoint(500, 200), 200, ::ULIS::CColor( 255, 0 ,0 ), false );
    ::ULIS::FPainterContext::DrawCircleAndresAA( block, ::ULIS::FPoint(720,500), 200, ::ULIS::CColor( 255, 0 ,0 ), false );
    
    ::ULIS::FPainterContext::DrawCircleAndresAA( block, ::ULIS::FPoint(-72,1000), 200, ::ULIS::CColor( 255, 0 ,0 ), false );

    ::ULIS::FPainterContext::DrawCircleAndresAA( block, ::ULIS::FPoint(1100,300), 200, ::ULIS::CColor( 255, 0 ,0 ), false );
    
    ::ULIS::FPainterContext::DrawCircleAndresAA( block, ::ULIS::FPoint(1000,1000), 200, ::ULIS::CColor( 255, 0 ,0 ), false );
    
    ::ULIS::FPainterContext::DrawCircleAndresAA( block, ::ULIS::FPoint(-50,-50), 200, ::ULIS::CColor( 255, 0 ,0 ), false );*/

    //::ULIS::FPainterContext::DrawCircleAndres( block, ::ULIS::FPoint(250,300), 78, ::ULIS::CColor( 0, 255 ,0 ), true );
    //::ULIS::FPainterContext::DrawCircleAndres( block, ::ULIS::FPoint(150,200), 200, ::ULIS::CColor( 255, 0 ,0 ), true );
    //::ULIS::FPainterContext::DrawCircleBresenhamAA( block, ::ULIS::FPoint(1000,1000), 180, ::ULIS::CColor( 255, 0 ,0 ), true );
    //::ULIS::FPainterContext::DrawCircleBresenhamAA( block, ::ULIS::FPoint(-50,-50), 180, ::ULIS::CColor( 255, 0 ,0 ), true );
    //::ULIS::FPainterContext::DrawCircleBresenhamAA( block, ::ULIS::FPoint(-50,1000), 180, ::ULIS::CColor( 255, 0 ,0 ), true );
    //::ULIS::FPainterContext::DrawCircleBresenhamAA( block, ::ULIS::FPoint(1000,-50), 180, ::ULIS::CColor( 255, 0 ,0 ), true );


    /*::ULIS::FPainterContext::DrawCircleBresenham( block, ::ULIS::FPoint(250,300), 78, ::ULIS::CColor( 0, 255 ,0 ), true );
    ::ULIS::FPainterContext::DrawCircleBresenham( block, ::ULIS::FPoint(400,200), 25, ::ULIS::CColor( 0, 0 ,255 ), true );*/
    
    
    //::ULIS::FPainterContext::DrawCircleAndres( block, ::ULIS::FPoint(500,250), 200, ::ULIS::CColor( 255, 0 ,0 ) );
    //::ULIS::FPainterContext::DrawCircleAndresAA( block, ::ULIS::FPoint(500,750), 195, ::ULIS::CColor( 255, 0 ,0 ) );
    
    //::ULIS::FPainterContext::DrawCircleBresenham( block, ::ULIS::FPoint(500,250), 200, ::ULIS::CColor( 255, 0 ,0 ) );
    //::ULIS::FPainterContext::DrawCircleBresenhamAA( block, ::ULIS::FPoint(500,750), 195, ::ULIS::CColor( 255, 0 ,0 ) );


    //Arcs
    /*
    ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 255, 246, 152, ::ULIS::CColor( 0, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 127, 123, 52, ::ULIS::CColor( 0, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 350, 16, 278, ::ULIS::CColor( 0, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 200, 127, 78, ::ULIS::CColor( 0, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 50, 301, 28, ::ULIS::CColor( 0, 0 ,0 ) );
    */
    
    /*::ULIS::FPainterContext::DrawArcBresenhamAA( block, ::ULIS::FPoint(-20,-20), 200, 1, 359, ::ULIS::CColor( 255, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawArcBresenhamAA( block, ::ULIS::FPoint(1000,30), 200, 1, 359, ::ULIS::CColor( 255, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawArcBresenhamAA( block, ::ULIS::FPoint(1000,1003), 200, 1, 359, ::ULIS::CColor( 255, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawArcBresenhamAA( block, ::ULIS::FPoint(10,1004), 200, 1, 359, ::ULIS::CColor( 255, 0 ,0 ) );

    ::ULIS::FPainterContext::DrawArcBresenhamAA( block, ::ULIS::FPoint(500,500), 195, 0, 180, ::ULIS::CColor( 255, 0 ,0 ) );

    ::ULIS::FPainterContext::DrawRectangle( block, ::ULIS::FPoint(800,800), ::ULIS::FPoint(1300,1350), ::ULIS::CColor( 255, 0 ,0 ), false );*/


    //Rectangles
    /*
    ::ULIS::FPainterContext::DrawRectangle( block, ::ULIS::FPoint(50,950), ::ULIS::FPoint(950, 975), ::ULIS::CColor( 0, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawRectangle( block, ::ULIS::FPoint(300,960), ::ULIS::FPoint(303, 965), ::ULIS::CColor( 0, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawRectangle( block, ::ULIS::FPoint(325,960), ::ULIS::FPoint(328, 965), ::ULIS::CColor( 0, 0 ,0 ), true );
    */

    //Polygons
    std::vector< ::ULIS::FPoint > star;
    
    star.push_back( ::ULIS::FPoint( 500 - 499, 200 ) );
    star.push_back( ::ULIS::FPoint( 666 - 499, 750 ) );
    star.push_back( ::ULIS::FPoint( 250 - 499, 333 ) );
    star.push_back( ::ULIS::FPoint( 750 - 499, 333 ) );
    star.push_back( ::ULIS::FPoint( 333 - 499, 750 ) );
    
    
    std::vector< ::ULIS::FPoint > pentagon;
    pentagon.push_back( ::ULIS::FPoint( 500, 200 ) );
    pentagon.push_back( ::ULIS::FPoint( 750, 333 ) );
    pentagon.push_back( ::ULIS::FPoint( 666, 750 ) );
    pentagon.push_back( ::ULIS::FPoint( 333, 750 ) );
    pentagon.push_back( ::ULIS::FPoint( 250, 333 ) );

    /*for( int i = 0; i < star.size(); i++)
    {
        star.at(i) = star.at(i).RotateAround( ::ULIS::FPoint(500, 500), 1.57);
    }*/
    
   /* for( int i = 0; i < pentagon.size(); i++)
    {
        pentagon.at(i) = pentagon.at(i).RotateAround( ::ULIS::FPoint(500, 500), 1.57);
    }*/
    
    
    //::ULIS::FPainterContext::DrawPolygonAA( block, star, ::ULIS::CColor( 0, 0 ,0 ), true );
    //::ULIS::FPainterContext::DrawPolygonAA( block, pentagon, ::ULIS::CColor( 0, 0 ,0 ), true );
    
    //Ellipses
    
    ::ULIS::FPainterContext::DrawEllipseAA( block, ::ULIS::FPoint(500, 250), 200, 100, ::ULIS::CColor( 0, 255 ,0 ) );

    ::ULIS::FPainterContext::DrawEllipseAA( block, ::ULIS::FPoint(950, 50), 200, 100, ::ULIS::CColor( 0, 255 ,0 ), true );
    
    ::ULIS::FPainterContext::DrawEllipseAA( block, ::ULIS::FPoint(30, 250), 200, 300, ::ULIS::CColor( 0, 255 ,0 ), true );

    ::ULIS::FPainterContext::DrawEllipseAA( block, ::ULIS::FPoint(950, 900), 74, 350, ::ULIS::CColor( 0, 255 ,0 ), true );

    ::ULIS::FPainterContext::DrawEllipseAA( block, ::ULIS::FPoint(-50, 900), 200, 100, ::ULIS::CColor( 0, 255 ,0 ), true );
   

    
    //::ULIS::FPainterContext::DrawEllipse( block, ::ULIS::FPoint(500, 750), 300, 90, ::ULIS::CColor( 255, 0 ,0 ), true );
    
    //::ULIS::FPainterContext::DrawEllipse( block, ::ULIS::FPoint(500, 750), 300, 90, ::ULIS::CColor( 0, 255 ,0 ) );
    
    /*::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 200, 300, 69, ::ULIS::CColor( 0, 0 ,0 ), true );
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 300, 200, 30, ::ULIS::CColor( 0, 0 ,0 ), true );
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 300, 100, 0, ::ULIS::CColor( 0, 0 ,0 ), true );
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 150, 100, 28, ::ULIS::CColor( 0, 0 ,0 ), false );
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 200, 400, 0, ::ULIS::CColor( 0, 0 ,0 ), false );
    
    */
    
    
    //::ULIS::FPainterContext::DrawRotatedEllipseAA( block, ::ULIS::FPoint(500, 500), 300, 400, 30, ::ULIS::CColor( 255, 0 ,0 ), false );

    //::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 750), 100, 200, 42, ::ULIS::CColor( 0, 0 ,255 ), true );

    //::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 250), 200, 100, 75, ::ULIS::CColor( 0, 255 ,0 ), true );
    

    //::ULIS::FPainterContext::DrawRotatedEllipseAA( block, ::ULIS::FPoint(200, 750), 400, 100, 45, ::ULIS::CColor( 255, 0 ,0 ) );
    
    //::ULIS::FPainterContext::DrawRotatedEllipseAA( block, ::ULIS::FPoint(500, 750), 200, 100, 75, ::ULIS::CColor( 0, 255 ,0 ) );

    //::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 300, 100, 0, ::ULIS::CColor( 0, 0 ,0 ), true );
    //::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 350, 200, 2, ::ULIS::CColor( 0, 0 ,0 ), true );

    //::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 200, 400, 12, ::ULIS::CColor( 0, 0 ,0 ), true );
    
    // Beziers
    
   /* ULIS::FPainterContext::DrawQuadraticBezierAA( block, ::ULIS::FPoint( 200, 200 ), ::ULIS::FPoint( 400, 200 ), ::ULIS::FPoint( -200, -400 ), 1.0, ::ULIS::CColor( 255, 0 ,0 ) );
    
    ULIS::FPainterContext::DrawQuadraticBezierAA( block, ::ULIS::FPoint( -100, -200 ), ::ULIS::FPoint( 400, 50 ), ::ULIS::FPoint( 800, 700 ), 1.0, ::ULIS::CColor( 255, 255 ,0 ) );
    
    ULIS::FPainterContext::DrawQuadraticBezierAA( block, ::ULIS::FPoint( 200, 1200 ), ::ULIS::FPoint( -500, 50 ), ::ULIS::FPoint( 500, 300 ), 1.0, ::ULIS::CColor( 0, 255 ,0 ) );*/
    

    // Qt Windowing
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

