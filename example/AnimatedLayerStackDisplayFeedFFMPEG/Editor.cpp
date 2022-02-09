// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Editor.cpp
* @author       Clement Berthaud
* @brief        AnimatedLayerStackBasicDisplay application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "Editor.h"
#include "AnimatedLayerStackRenderer.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <cstdlib>

SEditor::~SEditor() {
    mCtx.Finish();
    delete  mImage;
    delete  mPixmap;
    delete  mLabel;
    delete  mTimer;
    delete  mView;
    video_reader_close( &mVideoReader );
}

SEditor::SEditor( FContext& iCtx, FAnimatedLayerStack& iDocument )
    : mCtx( iCtx )
    , mDocument( iDocument )
    , mElapsed( 0.f )
    , mImage( nullptr )
    , mPixmap( nullptr )
    , mLabel( nullptr )
    , mTimer( nullptr )
{
    const char* inputFile = "C:/Users/conta/Documents/work/FFMPEG/in.mp4";
    if( !video_reader_open( &mVideoReader, inputFile ) ) {
        printf( "Couldn't open video file ( make sure you set a video file that exists )\n" );
        return;
    }

    // Allocate frame buffer
    constexpr int ALIGNMENT = 128;
    const int frame_width = mVideoReader.width;
    const int frame_height = mVideoReader.height;
    #define posix_memalign(p, a, s) (((*(p)) = _aligned_malloc((s), (a))), *(p) ?0 :errno)
    if( posix_memalign( ( void** )&mFrameData, ALIGNMENT, frame_width * frame_height * 4 ) != 0 ) {
        printf( "Couldn't allocate frame buffer\n" );
        return;
    }


    mView = new FBlock( mDocument.Width(), mDocument.Height(), mDocument.Format() );
    ULIS_ASSERT( mDocument.Format() == Format_RGBA8, "Bad format" ); // For compat with QImage::Format::Format_RGBA8888
    mImage = new QImage( mFrameData, frame_width, frame_height, frame_width * 4, QImage::Format::Format_RGBA8888 );
    mPixmap = new QPixmap( QPixmap::fromImage( *mImage ) );
    mLabel = new QLabel( this );
    mLabel->setPixmap( *mPixmap );
    this->QWidget::setFixedSize( mPixmap->size() );
    mTimer = new QTimer();
    // This is not particularly precise but enough for a demo
    mTimer->setInterval( mDocument.GetIntervalSeconds() * 1000.0f );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();
}

void
SEditor::tickEvent() {
    // This is not particularly precise but enough for a demo
    mElapsed += mDocument.GetIntervalSeconds();
    std::cout << mElapsed << std::endl;

    // Loop play
    //if( mElapsed >= static_cast< float >( mDocument.GetNumFrames() ) / static_cast< float >( mDocument.Fps() ) )
    //    mElapsed = 0;

    // Work on mView...
    //FAnimatedLayerStackRenderer::Render( mCtx, mDocument, *mView, mElapsed );
    //mCtx.Finish();

    // Read a new frame and load it into texture
    int64_t pts;
    if (!video_reader_read_frame( &mVideoReader, mFrameData, &pts ) ) {
        printf( "Couldn't load video frame\n" );
        return;
    }

    // Finish and update display
    mCtx.Finish();
    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );
}

