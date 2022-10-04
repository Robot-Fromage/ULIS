// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ToolBar.cpp
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "ToolBar.h"
#include "ULISLoader.h"
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>

SToolBar::~SToolBar() {
    for( auto it : mButtons )
        delete  it;

    delete  mFormats;
    delete  mBlendModes;
    delete  mAlphaModes;
    delete  mFilled;
}

SToolBar::SToolBar( QWidget* iParent, FULISLoader& iHandle )
    : QWidget( iParent )
    , mHandle( iHandle )
    , mButtons()
    , mFormats( nullptr )
    , mBlendModes( nullptr )
    , mAlphaModes( nullptr )
    , mFilled( nullptr )
{
    eFormat fmt = mHandle.FormatGUI();
    FContext& ctx = mHandle.ContextGUI();
    FBlock miniBlockAA( 14, 10, fmt );
    FBlock miniBlockSP( 14, 10, fmt );
    {
        ctx.Clear( miniBlockAA, miniBlockAA.Rect(), FSchedulePolicy::CacheEfficient );
        ctx.Clear( miniBlockSP, miniBlockSP.Rect(), FSchedulePolicy::CacheEfficient );
    }
    ctx.Flush();

    FFont font( mHandle.FontEngine(), "Segoe UI", "Black" );
    FEvent eventText;
    {
        ctx.Fence();
        int size = miniBlockAA.Height() - 2;
        ctx.RasterTextAA( miniBlockAA, L"AA", font, size, FMat3F::MakeTranslationMatrix( 0, size ), FColor::Black, FSchedulePolicy::MonoChunk, 0, nullptr, &eventText );
        ctx.RasterTextAA( miniBlockSP, L"SP", font, size, FMat3F::MakeTranslationMatrix( 0, size ), FColor::Black, FSchedulePolicy::MonoChunk, 1, &eventText, nullptr );
    }
    ctx.Finish();

    this->QWidget::setFixedSize( 200, 800 );

    mButtons.reserve( kNumRasterOP * kNumRasterMode );
    for( int i = 0; i < kNumRasterOP; ++i ) {
        for( int j = 0; j < kNumRasterMode; ++j ) {
            mButtons.push_back( new QPushButton( this ) );
            BuildButton( mButtons.back(), eRasterOp(i), eRasterMode(j), miniBlockAA, miniBlockSP );
        }
    }
    ctx.Finish();

    mFormats = new QComboBox( this );
    mFormats->move( 5, 380 );
    mFormats->setFixedWidth( 106 );
    for( int i = 0; i < kNumDocumentFormat; ++i ) {
        mFormats->addItem( QString( kwDocumentFormat[i] ) );
    }

    mBlendModes = new QComboBox( this );
    mBlendModes->move( 5, 410 );
    mBlendModes->setFixedWidth( 106 );
    for( int i = 0; i < NumBlendModes; ++i ) {
        mBlendModes->addItem( QString( kwBlendMode[i] ) );
    }

    mAlphaModes = new QComboBox( this );
    mAlphaModes->move( 5, 440 );
    mAlphaModes->setFixedWidth( 106 );
    for( int i = 0; i < NumAlphaModes; ++i ) {
        mAlphaModes->addItem( QString( kwAlphaMode[i] ) );
    }

    mFilled = new QCheckBox( "Filled", this );
    mFilled->move( 5, 470 );

    QObject::connect( mFormats, SIGNAL( currentIndexChanged( int ) ), this, SIGNAL( FormatChanged( int ) ) );
    QObject::connect( mBlendModes, SIGNAL( currentIndexChanged( int ) ), this, SIGNAL( BlendChanged( int ) ) );
    QObject::connect( mAlphaModes, SIGNAL( currentIndexChanged( int ) ), this, SIGNAL( AlphaChanged( int ) ) );
    QObject::connect( mFilled, SIGNAL( stateChanged( int ) ), this, SIGNAL( FilledChanged( int ) ) );

}

void
SToolBar::BuildButton( QPushButton* ioButton, eRasterOp iRasterOp, eRasterMode iRasterMode, const FBlock& iAA, const FBlock& iSP )
{
    eFormat fmt = mHandle.FormatGUI();
    FContext& ctx = mHandle.ContextGUI();
    const FHardwareMetrics& hw = mHandle.HardwareMetrics();

    FVec2I padding = 5;
    FVec2I size = 32;
    FVec2I center = size / 2;
    int radius = size.x / 2 - padding.x;
    FBlock* block = new FBlock( size.x, size.y, fmt );

    FEvent eventClear;
    FEvent eventRaster;
    FEvent eventText(
        {
            [ block, size, padding, ioButton, iRasterMode, iRasterOp ]( const FRectI& ) {
                QImage image( block->Bits(), size.x, size.y, block->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
                QPixmap pixmap = QPixmap::fromImage( image );
                QIcon icon( pixmap );
                ioButton->setIcon( icon );
                ioButton->setFixedSize( size.x, size.y );
                ioButton->setIconSize( QSize( size.x, size.y ) );
                delete  block;
            }
        }
    );
    QObject::connect( ioButton, &QPushButton::clicked, [=](){ this->SetRaster( iRasterOp, iRasterMode ); } );
    ioButton->move(
          padding.x + int( iRasterMode ) * ( size.x + padding.x )
        , padding.y + int( iRasterOp ) * ( size.y + padding.y )
    );

    ctx.Clear( *block, block->Rect(), FSchedulePolicy::MonoChunk, 0, nullptr, &eventClear );

    if( iRasterMode == kNone || iRasterMode == kGeneratePoints ) {
        ctx.Dummy_OP( 1, &eventRaster, &eventText );
    } else if( iRasterMode == kAA ) {
        ctx.Blend( iAA, *block, iAA.Rect(), FVec2I( size.x - iAA.Width(), 1 ), Blend_Normal, Alpha_Normal, 1.f, FSchedulePolicy::MonoChunk, 1, &eventRaster, &eventText );
    } else if( iRasterMode == kSP ) {
        ctx.Blend( iSP, *block, iSP.Rect(), FVec2I( size.x - iSP.Width(), 1 ), Blend_Normal, Alpha_Normal, 1.f, FSchedulePolicy::MonoChunk, 1, &eventRaster, &eventText );
    }

    switch( iRasterOp ) {
        case kLine: {
            if( iRasterMode == kNone || iRasterMode == kGeneratePoints ) {
                ctx.DrawLine( *block, padding, size - padding, FColor::Black, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                ctx.DrawLineAA( *block, padding, size - padding, FColor::Black, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                ctx.DrawLineSP( *block, padding, size - padding, FColor::Black, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kCircle: {
            if( iRasterMode == kNone || iRasterMode == kGeneratePoints ) {
                ctx.DrawCircle( *block, center, radius, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                ctx.DrawCircleAA( *block, center, radius, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                ctx.DrawCircleSP( *block, center, radius, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kArc: {
            if( iRasterMode == kNone || iRasterMode == kGeneratePoints ) {
                ctx.DrawArc( *block, center, radius, 315, 90, FColor::Black, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                ctx.DrawArcAA( *block, center, radius, 315, 90, FColor::Black, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                ctx.DrawArcSP( *block, center, radius, 315, 90, FColor::Black, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kEllipse: {
            if( iRasterMode == kNone || iRasterMode == kGeneratePoints ) {
                ctx.DrawEllipse( *block, center, radius, radius / 2, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                ctx.DrawEllipseAA( *block, center, radius, radius / 2, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                ctx.DrawEllipseSP( *block, center, radius, radius / 2, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kRotatedEllipse: {
            if( iRasterMode == kNone || iRasterMode == kGeneratePoints ) {
                ctx.DrawRotatedEllipse( *block, center, radius, radius / 2, 45, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                ctx.DrawRotatedEllipseAA( *block, center, radius, radius / 2, 45, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                ctx.DrawRotatedEllipseSP( *block, center, radius, radius / 2, 45, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kRectangle: {
            if( iRasterMode == kNone || iRasterMode == kGeneratePoints ) {
                ctx.DrawRectangle( *block, padding, size - padding, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                // Rectangle AA ?
                ctx.DrawRectangle( *block, padding, size - padding, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                // Rectangle SP ?
                ctx.DrawRectangle( *block, padding, size - padding, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kPolygon: {
            if( iRasterMode == kNone || iRasterMode == kGeneratePoints ) {
                std::vector< FVec2I > vec {
                      FVec2I( padding.x, padding.y )
                    , FVec2I( size.x - padding.x, padding.y * 2 )
                    , FVec2I( size.x - padding.x, size.y - padding.y )
                    , FVec2I( size.x - padding.x * 6, size.y - padding.y )
                    , FVec2I( center )
                };
                ctx.DrawPolygon( *block, vec, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                std::vector< FVec2I > vec {
                      FVec2I( padding.x, padding.y )
                    , FVec2I( size.x - padding.x, padding.y * 2 )
                    , FVec2I( size.x - padding.x, size.y - padding.y )
                    , FVec2I( size.x - padding.x * 6, size.y - padding.y )
                    , FVec2I( center )
                };
                ctx.DrawPolygonAA( *block, vec, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                std::vector< FVec2F > vec {
                      FVec2F( padding.x, padding.y )
                    , FVec2F( size.x - padding.x, padding.y * 2 )
                    , FVec2F( size.x - padding.x, size.y - padding.y )
                    , FVec2F( size.x - padding.x * 6, size.y - padding.y )
                    , FVec2F( center )
                };
                ctx.DrawPolygonSP( *block, vec, FColor::Black, false, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
        case kQuadraticBezier: {
            FVec2I p0( padding.x, size.y - padding.y );
            FVec2I p1( center.x, padding.y );
            FVec2I p2( size.x - padding.x, size.y - padding.y );
            if( iRasterMode == kNone || iRasterMode == kGeneratePoints ) {
                ctx.DrawQuadraticBezier( *block, p0, p1, p2, 1.f, FColor::Black, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kAA ) {
                ctx.DrawQuadraticBezierAA( *block, p0, p1, p2, 1.f, FColor::Black, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            } else if( iRasterMode == kSP ) {
                ctx.DrawQuadraticBezierSP( *block, p0, p1, p2, 1.f, FColor::Black, block->Rect(), FSchedulePolicy::MonoChunk, 1, &eventClear, &eventRaster );
            }
            break;
        }
    }
}

void
SToolBar::SetRaster( eRasterOp iRasterOp, eRasterMode iRasterMode ) {
    emit RasterChanged( iRasterOp, iRasterMode );
}

