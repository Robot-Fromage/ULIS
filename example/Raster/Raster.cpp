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

    //Data
    FBlock canvas(1024,1024,fmt);
    {
        //ctx.DrawLine( canvas, FVec2F( 200, 200 ), FVec2F( 500, 400 ), FColor::RGBA8(200,0,0) );

        //ctx.DrawLineAA( canvas, FVec2F( 200, 200 ), FVec2F( 500, 400 ), FColor::RGBA8(200,0,0, 120) );
        //ctx.DrawLineAA( canvas, FVec2F( 1, 1 ), FVec2F( 63, 48 ), FColor::RGBA8(0,255,0, 255) );

        //ctx.DrawCircleAndres( canvas, FVec2I( 300, 300 ), 250, FColor::RGBA8(0,0,0,255) );
        ctx.DrawCircleAndresAA(canvas,FVec2I(300,300),250,FColor::RGBA8(0,0,0,255));

        ctx.Finish();
    }

    // Bake Qt App / Window
    QApplication    app(argc,argv);
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage(canvas.Bits(),canvas.Width(),canvas.Height(),canvas.BytesPerScanLine(),QImage::Format::Format_RGBA8888);
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
