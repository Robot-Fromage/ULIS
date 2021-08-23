// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Widgets.Canvas.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definition for the SCanvas class.
 */
#include "Application/Widgets/Application.Widgets.Canvas.h"

#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <ULIS_CORE>

namespace  nApplication {
namespace  nWidgets {
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
SCanvas::~SCanvas()
{
    tSelf::Destroy();
}


SCanvas::SCanvas( QWidget* iParent )
    : tSuperClass( iParent )
    , mBlock( nullptr )
    , mImage( nullptr )
    , mPixmap()
    , mLabel( nullptr )
    , bIsLeftMouseDown( false )
    , bIsRightMouseDown( false )
{
    tSelf::Init();
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
void
SCanvas::resizeEvent( QResizeEvent*  event )
{
    tSelf::Compose();
}


void
SCanvas::mousePressEvent( QMouseEvent*  event )
{
    if( event->button() == Qt::LeftButton )
        bIsLeftMouseDown = true;

    if( event->button() == Qt::RightButton )
        bIsRightMouseDown = true;
}


void
SCanvas::mouseMoveEvent( QMouseEvent*  event )
{
    QPoint pos = event->pos();
    ::ULIS::CColor color;
    int brushSize = 20;
    int brushSize2 = brushSize / 2;

    if( bIsLeftMouseDown )
        color = ::ULIS::CColor::FromRGB( 0, 0, 0, 255 );
    else if( bIsRightMouseDown )
        color = ::ULIS::CColor::FromRGB( 0, 0, 0, 0 );

    ::ULIS::FClearFillContext::FillRect( mBlock, color, ::ULIS::FRect( pos.x() - brushSize2, pos.y() - brushSize2, brushSize, brushSize ) );

    mPixmap.convertFromImage( *mImage );
    mLabel->setPixmap( mPixmap );
}


void
SCanvas::mouseReleaseEvent( QMouseEvent*  event )
{
    if( event->button() == Qt::LeftButton )
        bIsLeftMouseDown = false;

    if( event->button() == Qt::RightButton )
        bIsRightMouseDown = false;
}


void
SCanvas::keyPressEvent( QKeyEvent*  event )
{
    if( event->key() == Qt::Key_Backspace )
    {
        ::ULIS::FClearFillContext::Clear( mBlock );
        mPixmap.convertFromImage( *mImage );
        mLabel->setPixmap( mPixmap );
    }
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities
void
SCanvas::Init()
{
    setFocusPolicy( Qt::FocusPolicy::StrongFocus );

    int w = width();
    int h = height();
    mBlock  = ::ULIS::FMakeContext::MakeBlock( w, h, ::ULIS::Format::Format_RGBA8 );
    ::ULIS::FClearFillContext::Fill( mBlock, ::ULIS::CColor( 255, 0, 0 ) );
    mImage  = new  QImage( mBlock->DataPtr(), w, h, mBlock->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap = QPixmap::fromImage( *mImage );
    mLabel  = new  QLabel( this );
    mLabel->setPixmap( mPixmap );

    this->setStyleSheet( "border: 1px solid rgb( 40, 40, 40 ); background: rgb( 200, 200, 200 );" );
}


void
SCanvas::Compose()
{
    int w = width();
    int h = height();

    ::ULIS::IBlock* temp = ::ULIS::FMakeContext::MakeBlock( w, h, ::ULIS::Format::Format_RGBA8 );
    ::ULIS::FClearFillContext::Clear( temp );
    ::ULIS::FMakeContext::CopyBlockRectInto( mBlock, temp, ::ULIS::FRect( 0, 0, mBlock->Width(), mBlock->Height() ) );
    delete  mBlock;
    mBlock = temp;

    delete  mImage;
    mImage  = new  QImage( mBlock->DataPtr(), w, h, mBlock->BytesPerScanLine(), QImage::Format::Format_RGBA8888 );
    mPixmap.convertFromImage( *mImage );
    mLabel->setPixmap( mPixmap );
    mLabel->resize( w, h );
}


void
SCanvas::Destroy()
{
    delete  mLabel;
    delete  mImage;
    delete  mBlock;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
::ULIS::IBlock*
SCanvas::GetBlock()
{
    return  mBlock;
}


void
SCanvas::RefreshBlock()
{
    mPixmap.convertFromImage( *mImage );
    mLabel->setPixmap( mPixmap );
}


} // namespace  nWidgets
} // namespace  nApplication

