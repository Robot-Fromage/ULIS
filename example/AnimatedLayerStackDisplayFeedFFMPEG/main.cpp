// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        AnimatedBezier application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "Editor.h"
#include "VReader.h"
#include <QApplication>

#include <cstdlib>
#include <ctime>
int
main( int argc, char *argv[] ) {
    // Init srand
    srand( time( NULL ) );

    // ULIS Common
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );

    // Document data
    uint16 w = 512;
    uint16 h = 256;
    FAnimatedLayerStack* stack;
    ULAAssignStack( stack, w, h, fmt )
    ULADef( SetName( "stack" ) )
    ULADef( SetFps( 30 ) )
    [
        ULACreateChild( FAnimatedLayerImage )
        ULADef( SetName( "layer0" ) )
    ];

    FVideoReaderState videoReader;

    const char* inputFile = "C:/Users/conta/Documents/work/FFMPEG/in.mp4";
    if( !video_reader_open( &videoReader, inputFile ) ) {
        printf( "Couldn't open video file ( make sure you set a video file that exists )\n" );
        return -1;
    }

    // Allocate frame buffer
    constexpr int ALIGNMENT = 128;
    const int frame_width = videoReader.width;
    const int frame_height = videoReader.height;
    double duration = double( videoReader.av_format_ctx->duration ) / AV_TIME_BASE;
    #define posix_memalign(p, a, s) (((*(p)) = _aligned_malloc((s), (a))), *(p) ?0 :errno)
    uint8* frameData = new uint8[ frame_width * frame_height * 4 ];
    if( !frameData ) {
        printf( "Couldn't allocate frame buffer\n" );
        return -1;
    }

    // Read a new frame and load it into texture
    int64_t pts;
    if (!video_reader_read_frame( &videoReader, frameData, &pts ) ) {
        printf( "Couldn't load video frame\n" );
        return -1;
    }

    delete [] frameData;
    video_reader_close( &videoReader );


    // Fill layer anim
    FAnimatedLayerImage& layer0 = stack->Find< FAnimatedLayerImage >( "layer0" );
    for( int i = 0; i < 5; ++i )
        layer0.PushNewCel();

    // Paint solid colors at each frames manually
    ctx.Fill( *layer0.Instances()[0]->Data(), FColor::Red );
    ctx.Fill( *layer0.Instances()[1]->Data(), FColor::Green );
    ctx.Fill( *layer0.Instances()[2]->Data(), FColor::Blue );
    ctx.Fill( *layer0.Instances()[3]->Data(), FColor::Magenta );
    ctx.Fill( *layer0.Instances()[4]->Data(), FColor::Yellow );
    ctx.Finish();

    // Display
    QApplication app( argc, argv );
    SEditor* editor = new SEditor( ctx, *stack );
    editor->show();

    // Main exec loop
    int exit_code = app.exec();

    // Cleanup
    delete  editor;
    delete  stack;
    return  exit_code;
}

