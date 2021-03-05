// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Test.cpp
* @author       Clement Berthaud
* @brief        Test application for ULIS3.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
main(int argc,char *argv[]) 
{
    FThreadPool pool;
    FCommandQueue queue(pool);
    eFormat fmt = Format_RGBA8;
    FContext ctx(queue,fmt,PerformanceIntent_AVX);
    FHardwareMetrics hw;
    FSchedulePolicy policy_cache_efficient(ScheduleRun_Multi,ScheduleMode_Chunks,ScheduleParameter_Length,hw.L1CacheSize());
    FSchedulePolicy policy_mono_chunk(ScheduleRun_Mono,ScheduleMode_Chunks,ScheduleParameter_Count,1);

    _sleep(500);
    //Data
    FBlock canvas(800,800,fmt);
    ctx.Clear(canvas);
    ctx.Finish();
    {
        //ctx.DrawLine( canvas, FVec2F( 200, 200 ), FVec2F( 2154, 3254 ), FColor::RGBA8(255,0,0, 255) );
        //ctx.DrawLineAA( canvas, FVec2F( 200, 200 ), FVec2F( 600, 400 ), FColor::RGBAF(0,0,0,1) );
        
        //ctx.DrawRectangle( canvas, FVec2I( 200, 200 ), FVec2I( 300, 400 ), FColor::RGBA8( 0, 255, 0, 255 ), true);
        //ctx.DrawCircleAndres( canvas, FVec2I( 300, 300 ), 250, FColor::RGBA8(0,0,0,255) );
        //ctx.DrawCircleAndres(canvas,FVec2I(23,300),250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawCircleBresenhamAA(canvas,FVec2I(23,300),250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawCircleBresenhamAA(canvas,FVec2I(950,300),250,FColor::RGBA8(50,0,0,100), true);
        //ctx.DrawCircleBresenhamAA(canvas,FVec2I(950,950),250,FColor::RGBA8(50,0,0,100), true);

        //ctx.DrawCircleAndresAA(canvas,FVec2I(950,300),250,FColor::RGBA8(50,0,0,100), true);
        //ctx.DrawCircleBresenhamAA(canvas,FVec2I(950,300),250,FColor::RGBA8(50,0,0,100), true);

        //ctx.DrawArcAndres(canvas,FVec2I(950,300),250,13,250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawArcAndresAA(canvas,FVec2I(950,300),250,13,250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawArcBresenhamAA(canvas,FVec2I(950,300),250,13,250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawArcBresenhamAA(canvas,FVec2I(450,300),250,311,95,FColor::RGBA8(50,0,0,100));

        //ctx.DrawEllipse( canvas, FVec2I( 500, 850 ), 150, 400, FColor::RGBA8(50,0,0,100), true);
        //ctx.DrawEllipseAA(canvas,FVec2I(500,200),213,352,FColor::RGBA8(50,0,0,100));
        
        //ctx.DrawRotatedEllipse(canvas,FVec2I(500,800),300,100,285,FColor::RGBA8(50,0,0,100),true);
        //ctx.DrawRotatedEllipseAA(canvas,FVec2I(500,800),100,300,285,FColor::RGBA8(50,0,0,100),true);
        ctx.DrawRotatedEllipseAA(canvas,FVec2I(200,200),320,750,148,FColor::RGBA8(50,0,0,100),true);
        //ctx.DrawRotatedEllipseAA(canvas,FVec2I(352,400),215,120,124,FColor::RGBA8(50,0,0,100), true);

        //ctx.DrawRotatedEllipseAA(canvas,FVec2I(800,800),458,236,148,FColor::RGBA8(50,0,0,100), true);

        //ctx.DrawRotatedEllipseAA(canvas,FVec2I(500,500),400,100,285,FColor::RGBA8(50,0,0,100),true);

        //ctx.DrawEllipseAA( canvas, FVec2I( 500, 500), 100, 200 );

        //ctx.DrawRotatedEllipseAA( canvas, FVec2I( 700, 700), 100, 200, 53 );

        /*ctx.DrawRotatedEllipse(canvas,FVec2I(700,700),100,200,1, FColor::RGBA8( 255, 0, 0, 255 ) );
        ctx.DrawRotatedEllipse(canvas,FVec2I(300,300),200,300,79,FColor::RGBA8(255,0,0,255));

        ctx.DrawQuadraticBezier( canvas, FVec2I( 150, 150 ),  FVec2I( 250, 126 ), FVec2I( 72, 59 ), 1 );*/

        ctx.Finish();
    }

    FBlock canvasOut(1024,1024,Format_RGBA8);
    ctx.ConvertFormat( canvas, canvasOut );

    ctx.Finish();

    // Bake Qt App / Window
    QApplication    app(argc,argv);
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage(canvasOut.Bits(),canvasOut.Width(),canvasOut.Height(),canvasOut.BytesPerScanLine(),QImage::Format::Format_RGBA8888);
    QPixmap         pixmap  = QPixmap::fromImage(*image);
    QLabel*         label   = new QLabel(widget);
    label->setPixmap(pixmap);
    widget->resize(pixmap.size());
    widget->show();

    // Main exec loop in GUI Thread
    int exit_code = app.exec();

    // Cleanup
    delete  label;
    delete  image;
    delete  widget;

    return  exit_code;
}
