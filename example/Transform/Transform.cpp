// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Transform.cpp
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
    FContext ctx( queue, fmt, PerformanceIntent_SSE );
    FHardwareMetrics hw;
    FSchedulePolicy policy_cache_efficient( ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Chunks,ScheduleParameter_Length,hw.L1CacheSize());
    FSchedulePolicy policy_mono_chunk(ScheduleTime_Sync, ScheduleRun_Mono,ScheduleMode_Chunks,ScheduleParameter_Count,1);
    FSchedulePolicy policy_multi_scanlines(ScheduleTime_Sync, ScheduleRun_Multi,ScheduleMode_Scanlines );

    FBlock* blockSRC = new FBlock();
    FBlock* blockTEMP = new  FBlock( 1024, 1024, fmt );
    FBlock* blockDST = new  FBlock( 1024, 1024, fmt );
    {
        std::string path = "C:/Users/PRAXINOS/Documents/work/TEST.png";
        ctx.XLoadBlockFromDisk( *blockSRC, path );
        ctx.Clear( *blockTEMP, blockTEMP->Rect(), policy_cache_efficient );
        ctx.Fill( *blockDST, FColor::RGB( 255, 200, 14 ), blockDST->Rect(), policy_cache_efficient );
        ctx.Finish();
    }

    FRectI srcRect = blockSRC->Rect();
    FVec2F srcSize( srcRect.w, srcRect.h );
    FMat3F shift( FMat3F::MakeTranslationMatrix( -srcRect.w / 2, -srcRect.h / 2 ) );
    for( int j = 0; j < 3; ++j ) {
        for( int i = 0; i < 6; ++i ) {
            float angle = i / 5.f * FMath::kPIf * 2;
            FMat3F rotation( FMat3F::MakeRotationMatrix( angle ) );
            FMat3F translation( FMat3F::MakeTranslationMatrix( i * 100 + 50, j * 100 + 50 ) );
            FMat3F transform = translation * ( rotation * shift );
            ctx.TransformAffine(
                  *blockSRC
                , *blockTEMP
                , blockSRC->Rect()
                , transform
                , static_cast< eResamplingMethod >( j )
                , Border_Transparent
                , FColor::Transparent
                , policy_multi_scanlines
                , 0
                , nullptr
                , nullptr
            );
        }

        for( int i = 0; i < 6; ++i ) {
            float angle  = i / 5.f * FMath::kPIf;
            float iangle =  angle + FMath::kPIf;
            float rotx0 = ( cos( iangle ) + 1 ) / 2;
            float rotx1 = ( cos(  angle ) + 1 ) / 2;
            float dif = sin( angle ) * 0.15f;
            std::vector< FVec2F > srcpoints = { FVec2F( 0, 0 ), FVec2F( srcSize.x, 0 ), FVec2F( srcSize.x, srcSize.y ), FVec2F( 0, srcSize.y ) };
            std::vector< FVec2F > dstpoints = { FVec2F( rotx0, -dif ), FVec2F( rotx1, dif ), FVec2F( rotx1, 1 - dif ), FVec2F( rotx0, 1 + dif ) };
            FVec2F translation( i * 100 + 18, 350 + j * 100 - 32 );
            for( int i = 0; i < 4; ++i ) {
                dstpoints[i] *= srcSize;
                dstpoints[i] += translation;
            }
            FMat3F persp = FMat3F::MakeHomography( srcpoints.data(), dstpoints.data() );
            ctx.TransformPerspective(
                      *blockSRC
                    , *blockTEMP
                    , blockSRC->Rect()
                    , persp
                    , static_cast< eResamplingMethod >( j )
                    , Border_Transparent
                    , FColor::Transparent
                    , policy_multi_scanlines
                    , 0
                    , nullptr
                    , nullptr
                );
        }
    }
    ctx.Finish();

    ctx.Blend( *blockTEMP, *blockDST, blockTEMP->Rect(), FVec2I(), Blend_Normal, Alpha_Normal, 1.f, policy_multi_scanlines, 0, 0, 0 );
    ctx.Finish();

    delete  blockSRC;
    delete  blockTEMP;

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

    return  exit_code;
}

