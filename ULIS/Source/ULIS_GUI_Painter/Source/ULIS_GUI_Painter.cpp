/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS_GUI.cpp
* Thomas Schmitt
* Please refer to LICENSE.md
*/

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <ULIS_CORE>
#include "ULIS/Maths/ULIS.Maths.Geometry.h"



int main( int argc, char *argv[] )
{
    
    QApplication app( argc, argv );

    ::ULIS::IBlock* block = ::ULIS::FMakeContext::MakeBlock( 1024, 1024, ::ULIS::FBlockRGBA8::TypeId() );
    ::ULIS::FClearFillContext::Fill( block, ::ULIS::CColor( 255, 255, 255 ) );

    /*
    //Vertical
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(500,50), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(500,950), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,0 ) );
    
    //Horizontal
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(50,500), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(950,500), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,0 ) );
    
    
    //x slope
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(100,300), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,255 ) );
    
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(900,300), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,255 ) );

    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(100,700), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,255 ) );
    
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(900,700), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 0, 0 ,255 ) );
    
    //y slope
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(300,100), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 255, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(700,100), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 255, 0 ,0 ) );

    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(300,900), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 255, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawLine( block, ::ULIS::FPoint(700,900), ::ULIS::FPoint(500, 500), ::ULIS::CColor( 255, 0 ,0 ) );
    */
    
    //circles
    /*for( int i = 0; i < 255; i+=5)
    {
        ::ULIS::FPainterContext::DrawCircleBresenham( block, ::ULIS::FPoint(500,500), i, ::ULIS::CColor( 0, i ,0 ) );
    }*/
    
    //Arcs
    /*
        ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 255, 246, 152, ::ULIS::CColor( 0, 0 ,0 ) );
        ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 127, 123, 52, ::ULIS::CColor( 0, 0 ,0 ) );
        ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 350, 16, 278, ::ULIS::CColor( 0, 0 ,0 ) );
        ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 200, 127, 78, ::ULIS::CColor( 0, 0 ,0 ) );
        ::ULIS::FPainterContext::DrawArcBresenham( block, ::ULIS::FPoint(500,500), 50, 301, 28, ::ULIS::CColor( 0, 0 ,0 ) );
    */
    
    
    //Rectangles
    /*
    ::ULIS::FPainterContext::DrawRectangle( block, ::ULIS::FPoint(50,950), ::ULIS::FPoint(950, 975), ::ULIS::CColor( 0, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawRectangle( block, ::ULIS::FPoint(300,960), ::ULIS::FPoint(303, 965), ::ULIS::CColor( 0, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawRectangle( block, ::ULIS::FPoint(325,960), ::ULIS::FPoint(328, 965), ::ULIS::CColor( 0, 0 ,0 ), true );
    */
    
    //Polygons
    /*
    std::vector< ::ULIS::FPoint > points;
    points.push_back( ::ULIS::FPoint( 500, 200 ) );
    points.push_back( ::ULIS::FPoint( 666, 750 ) );
    points.push_back( ::ULIS::FPoint( 250, 333 ) );
    points.push_back( ::ULIS::FPoint( 750, 333 ) );
    points.push_back( ::ULIS::FPoint( 333, 750 ) );
    
    for( int i = 0; i < points.size(); i++)
    {
        points.at(i) = points.at(i).RotateAround( ::ULIS::FPoint(500, 500), 1.57);
    }
    
    ::ULIS::FPainterContext::DrawPolygon( block, points, ::ULIS::CColor( 0, 0 ,0 ) );
    */
    
    //Ellipses
    //::ULIS::FPainterContext::DrawEllipse( block, ::ULIS::FPoint(500, 500), 300, 200, ::ULIS::CColor( 0, 0 ,0 ) );
    //::ULIS::FPainterContext::DrawEllipse( block, ::ULIS::FPoint(500, 500), 200, 300, ::ULIS::CColor( 0, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 200, 300, 214, ::ULIS::CColor( 0, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 300, 200, 18, ::ULIS::CColor( 0, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 200, 300, 85, ::ULIS::CColor( 0, 0 ,0 ) );
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 150, 100, 28, ::ULIS::CColor( 0, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 150, 100, 35, ::ULIS::CColor( 0, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 5, 3, 35, ::ULIS::CColor( 0, 0 ,0 ) );
    
    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 200, 150, 68, ::ULIS::CColor( 0, 0 ,0 ) );

    ::ULIS::FPainterContext::DrawRotatedEllipse( block, ::ULIS::FPoint(500, 500), 200, 400, 0, ::ULIS::CColor( 0, 0 ,0 ) );



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

