// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Test.cpp
* @author       Clement Berthaud
* @brief        Test application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include "Math/ShapeGeneration/Line.h"
#include "Math/ShapeGeneration/Polygon.h"

using namespace ::ULIS;

int
main(int argc,char *argv[]) 
{
    FThreadPool pool;
    FCommandQueue queue(pool);
    eFormat fmt = Format_RGBAF;
    FContext ctx(queue,fmt,PerformanceIntent_AVX);
    FHardwareMetrics hw;
    FSchedulePolicy policy_cache_efficient(ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Chunks,ScheduleParameter_Length,hw.L1CacheSize());
    FSchedulePolicy policy_mono_chunk(ScheduleTime_Sync, ScheduleRun_Mono,ScheduleMode_Chunks,ScheduleParameter_Count,1);

    //Data
    FBlock canvas(800,800,fmt);
    ctx.Clear(canvas);
    ctx.Finish();
    {
        TArray<FVec2I> pointsGenerated;
        FColor color =  FColor::HSVA8(0,255,255,255);
        /*GenerateLinePoints( canvas, FVec2F( 200, 200 ), FVec2F( 515, 324 ), points);
        for (int i = 0; i < points.Size(); i++)
        {
            color.SetHue8( i % 255 );
            canvas.SetPixelSafe(points[i].x, points[i].y, color );
        }*/

        //ctx.DrawLine( canvas, FVec2F( 200, 200 ), FVec2F( 515, 324 ), FColor::RGBA8(255,0,0, 255) );
        //ctx.DrawLineAA( canvas, FVec2F( 200, 200 ), FVec2F( 515, 324 ), FColor::RGBAF(1,0,0,1) );
        //ctx.DrawLineSP( canvas, FVec2F( 200.45, 200.2 ), FVec2F( 515.1, 321.2 ), FColor::RGBAF(1,0,0,1) );
        //ctx.DrawLineSP(canvas, FVec2F(145.25, 24.2), FVec2F(421.24, 685.2), FColor::RGBAF(1, 0, 0, 1));
        //ctx.DrawRectangle( canvas, FVec2I( 200.2, 200.2 ), FVec2I( 300.2, 400.2 ), FColor::RGBA8( 0, 255, 0, 255 ), true);
        //ctx.DrawCircleAndres( canvas, FVec2I( 300, 300 ), 250, FColor::RGBA8(0,0,0,255) );
        //ctx.DrawCircleAndresSP( canvas, FVec2F( 621, 300 ), 250.22, FColor::RGBA8(0,0,0,255) );
        //ctx.DrawCircleAndresSP( canvas, FVec2F( 650, 300 ), 200, FColor::RGBA8(0,0,0,255) );
        //ctx.DrawCircleAndresSP(canvas, FVec2F(250, 250), 200, FColor::RGBA8(0, 0, 0, 255));

        //ctx.DrawCircleAndres(canvas,FVec2I(500,300),250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawCircleBresenham(canvas, FVec2I(500, 300), 250, FColor::RGBA8(50, 0, 0, 100));

        //ctx.DrawCircleBresenhamAA(canvas,FVec2I(23,300),250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawCircleBresenhamAA(canvas,FVec2I(950,300),250,FColor::RGBA8(50,0,0,100), true);
        //ctx.DrawCircleBresenhamAA(canvas,FVec2I(950,950),250,FColor::RGBA8(50,0,0,100), true);

        //ctx.DrawCircleAndresAA(canvas,FVec2I(950,300),250,FColor::RGBA8(50,0,0,100), true);
        //ctx.DrawCircleBresenhamAA(canvas,FVec2I(950,300),250,FColor::RGBA8(50,0,0,100), true);

        //ctx.DrawArcAndres(canvas,FVec2I(950,300),250,13,250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawArcAndresAA(canvas,FVec2I(950,300),250,13,250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawArcBresenhamAA(canvas,FVec2I(950,300),250,13,250,FColor::RGBA8(50,0,0,100));
        //ctx.DrawArcBresenhamAA(canvas,FVec2I(450,300),250,311,95,FColor::RGBA8(50,0,0,100));

        //ctx.DrawCircleBresenhamSP(canvas, FVec2I(400.5, 400.5), 200, FColor::RGBA8(0, 0, 0, 255) );

        //ctx.DrawCircleAndresAA(canvas, FVec2I(400, 400), 200, FColor::RGBA8(0, 0, 0, 255));

        //ctx.DrawEllipse( canvas, FVec2I( 500, 850 ), 150, 400, FColor::RGBA8(50,0,0,100), true);
        //ctx.DrawEllipseAA(canvas,FVec2I(500,200),213,352,FColor::RGBA8(50,0,0,100));
        //ctx.DrawEllipseSP(canvas,FVec2I(500,200),213,352,FColor::RGBA8(50,0,0,100));

        //ctx.DrawRotatedEllipse(canvas,FVec2I(500,800),300,100,285,FColor::RGBA8(50,0,0,100),true);
       
        //ctx.DrawRotatedEllipseAA(canvas,FVec2I(500,800),100,300,285,FColor::RGBA8(50,0,0,100),true);
        //ctx.DrawRotatedEllipseAA(canvas,FVec2I(200,200),320,750,148,FColor::RGBAF(0.2,0,0,0.45),true);
        //ctx.DrawRotatedEllipseSP(canvas, FVec2I(300, 300), 200, 300, 57, FColor::RGBA8(255, 0, 0, 255), true);
        //ctx.DrawRotatedEllipseAA(canvas, FVec2I(200, 200), 320, 750, 148, FColor::RGBAF(0.2, 0, 0, 0.45), true);

        //ctx.DrawRotatedEllipseAA(canvas,FVec2I(352,400),215,120,124,FColor::RGBA8(50,0,0,100), true);

        //ctx.DrawRotatedEllipseAA(canvas,FVec2I(800,800),458,236,148,FColor::RGBA8(50,0,0,100), true);

        //ctx.DrawRotatedEllipseAA(canvas,FVec2I(500,500),400,100,285,FColor::RGBA8(50,0,0,100),true);

        //ctx.DrawEllipseAA( canvas, FVec2I( 500, 500), 100, 200 );

        //ctx.DrawRotatedEllipseAA( canvas, FVec2I( 700, 700), 100, 200, 53 );

        //ctx.DrawRotatedEllipse(canvas,FVec2I(700,700),100,200,1, FColor::RGBA8( 255, 0, 0, 255 ) );
        //ctx.DrawRotatedEllipseSP(canvas,FVec2I(300,300),200,300,57,FColor::RGBA8(255,0,0,255));

        //ctx.DrawQuadraticBezier( canvas, FVec2I( 150, 150 ),  FVec2I( 250, 126 ), FVec2I( 72, 59 ), 1 );

        std::vector< FVec2I > points;
        points.push_back( FVec2I( 381, 144 )); //Top
        points.push_back(FVec2I(211,586)); //Bottom left
        points.push_back(FVec2I(511,234)); // Right
        points.push_back(FVec2I(158,236)); // Left
        points.push_back(FVec2I(502,589)); // Bottom Right

        //ctx.DrawPolygon( canvas, points, FColor::RGBA8(0,200,0, 155), false);
        GeneratePolygonPoints( points, pointsGenerated );

        for (int i = 0; i < pointsGenerated.Size(); i++)
        {
            color.SetHue8(i % 255);
            canvas.SetPixelSafe(pointsGenerated[i].x, pointsGenerated[i].y, color);
        }

        //ctx.DrawLineAA( canvas, FVec2F( 381, 144 ), FVec2F( 211, 586 ), FColor::RGBA8(0,200,0,155) );

        //ctx.DrawQuadraticBezierAA( canvas, FVec2F( 211, 586 ), FVec2I(511,234), FVec2I(502,589), 1.0, FColor::RGBA8(0,200,0,155) );
        //ctx.DrawQuadraticBezierSP( canvas, FVec2F( 211, 586 ), FVec2I(511,234), FVec2I(502,589), 1.0, FColor::RGBA8(0,200,0,155) );

        ctx.Finish();
    }

    FBlock canvasOut(800,800,Format_RGBA8);
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
