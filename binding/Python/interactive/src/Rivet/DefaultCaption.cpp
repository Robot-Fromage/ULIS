// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         DefaultCaption.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#include "DefaultCaption.h"
#include <QPainter>

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------- Default values defines
#define     DEFAULT_ACTIVE_BACKGROUND_COLOR     QColor( 0, 0, 0, 127 )
#define     DEFAULT_INACTIVE_BACKGROUND_COLOR   QColor( 0, 0, 0, 50 )


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


FDefaultCaption::~FDefaultCaption()
{
    Destroy();
}

FDefaultCaption::FDefaultCaption( QWidget* iParent )
    : tSuperClass(              iParent                             )
    , mCloseButton(             nullptr                             )
    , mMaximizeButton(          nullptr                             )
    , mMinimizeButton(          nullptr                             )
    , mActiveBackgroundColor(   DEFAULT_ACTIVE_BACKGROUND_COLOR     )
    , mInactiveBackgroundColor( DEFAULT_INACTIVE_BACKGROUND_COLOR   )
{
    Init();
    Build();
    Compose();
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Public Geometry API
QRect
FDefaultCaption::ButtonsGeometry()
{
    return  QRect( mMinimizeButton->x(), 0, width() - mMinimizeButton->x(), height() );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------- Public customization utilities
void
FDefaultCaption::ActiveBackgroundColor( const  QColor& iColor )
{
    mActiveBackgroundColor = iColor;
}


const  QColor&
FDefaultCaption::ActiveBackgroundColor()
{
    return  mActiveBackgroundColor;
}

void
FDefaultCaption::InactiveBackgroundColor( const  QColor& iColor )
{
    mInactiveBackgroundColor = iColor;
}


const  QColor&
FDefaultCaption::InactiveBackgroundColor()
{
    return  mInactiveBackgroundColor;
}


void
FDefaultCaption::SetBlackControls()
{
    QColor black = Qt::black;
    mCloseButton->SetColor( black );
    mCloseButton->SetHoveredColor( black );
    mCloseButton->SetPressedColor( black );

    mMaximizeButton->SetColor( black );
    mMaximizeButton->SetHoveredColor( black );
    mMaximizeButton->SetPressedColor( black );

    mMinimizeButton->SetColor( black );
    mMinimizeButton->SetHoveredColor( black );
    mMinimizeButton->SetPressedColor( black );
}


void
FDefaultCaption::SetWhiteControls()
{
    QColor white = Qt::white;
    mCloseButton->SetColor( white );
    mCloseButton->SetHoveredColor( white );
    mCloseButton->SetPressedColor( white );

    mMaximizeButton->SetColor( white );
    mMaximizeButton->SetHoveredColor( white );
    mMaximizeButton->SetPressedColor( white );

    mMinimizeButton->SetColor( white );
    mMinimizeButton->SetHoveredColor( white );
    mMinimizeButton->SetPressedColor( white );
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
void
FDefaultCaption::paintEvent(  QPaintEvent*    event )
{
    QPainter painter(this);
    painter.setRenderHint( QPainter::Antialiasing, false );
    QColor  bgColor = topLevelWidget()->isActiveWindow() ? mActiveBackgroundColor : mInactiveBackgroundColor;

    int w = width();
    int h = height();
    painter.setBrush( bgColor );
    painter.setPen( Qt::NoPen );
    painter.drawRect( 0, 0, w, h );
}


void
FDefaultCaption::resizeEvent( QResizeEvent *event )
{
    QWidget::resizeEvent(event);
    Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities
void
FDefaultCaption::Init()
{
    if( !mCloseButton )     mCloseButton    = new  FCustomButton( this );
    if( !mMaximizeButton )  mMaximizeButton = new  FCustomButton( this );
    if( !mMinimizeButton )  mMinimizeButton = new  FCustomButton( this );
}


void
FDefaultCaption::Build()
{
    mCloseButton->SetIconShape( FCustomButton::eButtonIconShape::kClose );
    mMaximizeButton->SetIconShape( FCustomButton::eButtonIconShape::kMaximize );
    mMinimizeButton->SetIconShape( FCustomButton::eButtonIconShape::kMinimize );

    mCloseButton->SetHoveredBackgroundColor( QColor( 230, 20, 35 ) );

    QObject::connect( mCloseButton, SIGNAL( clicked() ), this, SLOT( ProcessCloseClicked() ) );
    QObject::connect( mMaximizeButton, SIGNAL( clicked() ), this, SLOT( ProcessMaximizeClicked() ) );
    QObject::connect( mMinimizeButton, SIGNAL( clicked() ), this, SLOT( ProcessMinimizeClicked() ) );
}


void
FDefaultCaption::Compose()
{
    int w = width();
    int h = height();

    float buttonRatio   = 1.5f;
    int buttonHeight    = h;
    int buttonWidth     = buttonHeight * buttonRatio;

    QSize buttonSize( buttonWidth, h );
    mCloseButton->resize( buttonSize );
    mMaximizeButton->resize( buttonSize );
    mMinimizeButton->resize( buttonSize );

    int n = 0;
    mCloseButton->move(     w - buttonWidth * ++n, 0 );
    mMaximizeButton->move(  w - buttonWidth * ++n, 0 );
    mMinimizeButton->move(  w - buttonWidth * ++n, 0 );
}


void
FDefaultCaption::Destroy()
{
    delete  mCloseButton;
    delete  mMaximizeButton;
    delete  mMinimizeButton;

    mCloseButton = 0;
    mMaximizeButton = 0;
    mMinimizeButton = 0;
}

