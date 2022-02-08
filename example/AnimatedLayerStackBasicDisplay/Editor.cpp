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
    mView = new FBlock( mDocument.Width(), mDocument.Height(), mDocument.Format() );
    ULIS_ASSERT( mDocument.Format() == Format_RGBA8, "Bad format" ); // For compat with QImage::Format::Format_RGBA8888
    mImage = new QImage( mView->Bits(), mView->Width(), mView->Height(), mView->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap = new QPixmap( QPixmap::fromImage( *mImage ) );
    mLabel = new QLabel( this );
    mLabel->setPixmap( *mPixmap );
    this->QWidget::setFixedSize( mPixmap->size() );
    mTimer = new QTimer();
    // This is not particularly precise but enough for a demo
    mTimer->setInterval( mDocument.GetInterval_ms() );
    QObject::connect( mTimer, SIGNAL( timeout() ), this, SLOT( tickEvent() ) );
    mTimer->start();
}

void
SEditor::tickEvent() {
    // This is not particularly precise but enough for a demo
    mElapsed += mDocument.GetInterval_ms();

    // Work on mView...
    // Nothing ATM

    // Finish and update display
    mCtx.Finish();
    mPixmap->convertFromImage( *mImage );
    mLabel->setPixmap( *mPixmap );
}

