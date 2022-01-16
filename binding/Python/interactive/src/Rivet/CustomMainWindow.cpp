// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CustomMainWindow.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#include "CustomMainWindow.h"

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default values defines
#define  DEFAULT_MINIMUM_WIDTH 200


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FCustomMainWindow::~FCustomMainWindow()
{
    tSelf::Destroy();
}


FCustomMainWindow::FCustomMainWindow( QWidget* iParent )
    : tSuperClass(      iParent )
    , mCaptionWidget(   nullptr )
    , mCenterWidget(    nullptr )
{
    tSelf::Init();
    tSelf::Build();
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------- Center Caption API
FCustomCaption*
FCustomMainWindow::CaptionWidget()  const
{
    return  mCaptionWidget;
}


QWidget*
FCustomMainWindow::CenterWidget()  const
{
    return  mCenterWidget;
}


void
FCustomMainWindow::SetCaptionWidget( FCustomCaption* iCaptionWidget )
{
    if( mCaptionWidget )
    {
        // this owns mCaptionWidget, hence has responsibility to delete
        delete  mCaptionWidget;
        mCaptionWidget = nullptr;
    }

    if( iCaptionWidget )
    {
        mCaptionWidget = iCaptionWidget;
        iCaptionWidget->setParent( this );
        iCaptionWidget->show();
        QObject::connect( mCaptionWidget, SIGNAL( CloseClicked() ), this, SLOT( ProcessCloseClicked() ) );
        QObject::connect( mCaptionWidget, SIGNAL( MaximizeClicked() ), this, SLOT( ProcessMaximizeClicked() ) );
        QObject::connect( mCaptionWidget, SIGNAL( MinimizeClicked() ), this, SLOT( ProcessMinimizeClicked() ) );
    }
}


void
FCustomMainWindow::SetCenterWidget( QWidget* iCenterWidget )
{
    if( mCenterWidget )
    {
        delete  mCenterWidget;
        mCenterWidget = NULL;
    }

    if( iCenterWidget )
    {
        mCenterWidget = iCenterWidget;
        iCenterWidget->setParent( this );
        iCenterWidget->show();
    }
}


void
FCustomMainWindow::Recompose()
{
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//--------------------------------- Protected Non-Client OS behaviour handling overrides
bool
FCustomMainWindow::NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )
{
    if( !tSuperClass::NCHitCaption( iRect, iBorderWidth, iX, iY ) )
        return  false;

    QPoint local = mapFromGlobal( QPoint( iX, iY ) );
    if( mCaptionWidget )
        return  mCaptionWidget->HitEmptySpace( local.x(), local.y() );
    else
        return  true;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt events override
void
FCustomMainWindow::resizeEvent( QResizeEvent*  event )
{
    // Very important !
    tSuperClass::resizeEvent( event );

    // Recompute internal geometry
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------- Private Qt Slots
void
FCustomMainWindow::ProcessCloseClicked()
{
    close();
}

void
FCustomMainWindow::ProcessMaximizeClicked()
{
    const auto state = windowState();
    if( state & Qt::WindowMaximized )
    {
        Restore();
    }
    else
    {
        showMaximized();
    }
}


void
FCustomMainWindow::ProcessMinimizeClicked()
{
    showMinimized();
}


//--------------------------------------------------------------------------------------
//--------------------------------- Protected Non-Client OS behaviour handling overrides
void
FCustomMainWindow::Init()
{
    mCaptionWidget  = nullptr;
    mCenterWidget   = nullptr;
}


void
FCustomMainWindow::Build()
{
    setMinimumWidth( DEFAULT_MINIMUM_WIDTH );
}


void
FCustomMainWindow::Compose()
{
    if( mCaptionWidget )
        mCaptionWidget->setGeometry( CaptionGeometry() );

    if( mCenterWidget )
        mCenterWidget->setGeometry( ContentsGeometry() );
}


void
FCustomMainWindow::Destroy()
{
    // Qt Composite Design Pattern should ensure destruction for us.
    /*
    if( mCaptionWidget )
    {
        // This is owner of mCaptionWidget, responsibility to delete
        delete  mCaptionWidget;
        mCaptionWidget = nullptr;
    }

    if( mCenterWidget )
    {
        // This is owner of mCaptionWidget, responsibility to delete
        delete  mCenterWidget;
        mCenterWidget = nullptr;
    }
    */
}

