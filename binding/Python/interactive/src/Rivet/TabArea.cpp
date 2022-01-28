// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TabArea.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#include "TabArea.h"
#include "CustomButton.h"
#include "GeometryUtils.h"
#include "DockingManager.h"
#include <assert.h>
#include <QApplication>
#include <QEvent>
#include <QEasingCurve>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QShortcut>
#include <QScrollArea>
#include <QScrollBar>
#include <QScroller>
#include <QTimer>
#include <QWheelEvent>
#include <QStackedWidget>

#define DEFAULT_TAB_WIDTH           200
#define DEFAULT_MAXIMUM_TAB_WIDTH   200
#define DEFAULT_MINIMUM_TAB_WIDTH   100
#define DEFAULT_WHEEL_TIME          50
#define DEFAULT_NAV_TIME            200
#define DEFAULT_ENSURE_TIME         250
#define DEFAULT_TABS_STYLE          FTab::eTabShape::kBezier


#define DEFAULT_NAV_WORK_FPS        60
#define DEFAULT_PROCESS_FPS         16


////////////////////////////////////////////////////////////////////////////////////////
////                                STATIC TOOLS                                    ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//----------------------------------------------- Static functions & tools for tab color


#define  _N_DEFAULT_COLORS 18
QColor  _default_color_scheme[ _N_DEFAULT_COLORS ] = {
    QColor(  42, 160, 154 ),
    QColor(  38, 140, 207 ),
    QColor(  32, 121, 114 ),
    QColor( 182, 140,  51 ),
    QColor(  45,  43,  50 ),
    QColor( 175,  62,  25 ),
    QColor( 247, 247, 249 ),
    QColor( 191,  49,  99 ),
    QColor( 147, 203,  68 ),
    QColor( 175, 155,  68 ),
    QColor( 229, 127,  33 ),
    QColor( 128, 207, 172 ),
    QColor(  72,  86,  73 ),
    QColor( 214, 214, 214 ),
    QColor( 181, 174, 104 ),
    QColor(  79,  63,  64 ),
    QColor( 230,  46,  44 ),
    QColor(  57,  34,  43 )
};


#define  _DEFAULT_COLOR( i ) _default_color_scheme[ i ]


QColor
_NextDefaultColor()
{
    static int _index = 0;
    return  _DEFAULT_COLOR( _index = ( _index + 1 ) % _N_DEFAULT_COLORS );
}


//--------------------------------------------------------------------------------------
//-------------------------------------- Static functions & tools for sorting & ordering


struct FTabOrderingPair
{
    QRect  mGeometry;
    FTab*    mTab;
};


static
bool
SortBasic( const  FTabOrderingPair& iA, const  FTabOrderingPair& iB )
{
    int termA = iA.mGeometry.x(); // semantic comparision element for A
    int termB = iB.mGeometry.x(); // semantic comparision element for B
    return termA < termB;
}


static
bool
SortBasicTabs( FTab* iA, FTab* iB )
{
    int termA = iA->TargetGeometry().x(); // semantic comparision element for A
    int termB = iB->TargetGeometry().x(); // semantic comparision element for B
    return termA < termB;
}

static
bool
SortBasicGeo( FTab* iA, FTab* iB )
{
    int termA = iA->geometry().x(); // semantic comparision element for A
    int termB = iB->geometry().x(); // semantic comparision element for B
    return termA < termB;
}


////////////////////////////////////////////////////////////////////////////////////////
////                                PUBLIC API                                      ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


FTabArea::~FTabArea()
{
    Unregister();
    Destroy();
}


FTabArea::FTabArea( QWidget* iParent ) :
    tSuperClass( iParent ),
    mDomesticTabs( QVector< FTab* >() ),

    mScrollArea(            NULL ),
    mScrollWidgetWrapper(   NULL ),
    mScroller(              NULL ),
    mLeftButton(            NULL ),
    mRightButton(           NULL ),

    mTabWidth(              DEFAULT_TAB_WIDTH ),
    mMaximumTabWidth(       DEFAULT_MAXIMUM_TAB_WIDTH ),
    mMinimumTabWidth(       DEFAULT_MINIMUM_TAB_WIDTH ),
    mOverlap(               0 ),

    mLeftDropShadowEffect(  NULL ),
    mRightDropShadowEffect( NULL ),
    mTabsShapeStyle(            DEFAULT_TABS_STYLE ),
    mTabsClosable( true ),
    mTabsLiftable( true ),

    mCurrentTab(            NULL ),

    mNavTimerLeft(          NULL ),
    mNavTimerRight(         NULL ),
    mProcessCandidateTimer( NULL ),

    mTag( "" ),

    mOnAreaBecomesEmptyCB( NULL ),
    mOnTabDroppedOutCB( NULL ),

    mLinkedStack( NULL )
{
    setObjectName( "TabArea" );
    Init();
    Build();
    Compose();
    Register();

    //TMP:CHECK:
    // Debug shortcut
    //new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_T ), this, SLOT( _NewTab() ), SLOT( _NewTab() ) );

    new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_W ), this, SLOT( CloseCurrentTab() ), SLOT( CloseCurrentTab() ) );
    new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_Tab ), this, SLOT( SwitchToNextTab() ), SLOT( SwitchToNextTab() ) );

}



//--------------------------------------------------------------------------------------
//------------------------------------Custom Hit Test Handling implementation in TabArea


bool
FTabArea::HitEmptySpace( long iX, long iY )
{
    // Window
    //  Caption                             we receive iX iY expressed in parent coord sys
    //      NavLeft                         use local
    //      NavRight                        use local
    //      TabArea                         use local
    //          ScrollArea                  map from TabArea
    //              ScrollWrapper           map from ScrollArea
    //                  Tab Tab Tab Tab Tab ...
    QPoint input( iX, iY );
    QPoint local(                                     mapFromParent( input )            );
    QPoint scrollAreaLocal(              mScrollArea->mapFromParent( local )            );
    QPoint scrollWrapperLocal(  mScrollWidgetWrapper->mapFromParent( scrollAreaLocal )  );

    QList< FTab *> list = mScrollWidgetWrapper->findChildren< FTab* >();
    for(QWidget *w : list)
        if( w->geometry().contains( scrollWrapperLocal ) )
            return  false;

    if( mLeftButton->geometry().contains(   local ) || mRightButton->geometry().contains(  local ) )
        return  false;

    return  true;
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------------------- Tab Control Interface


void
FTabArea::ManualAddNewTab( FTab* iTab )
{
    // Set tab parent to the wrapper.
    iTab->setParent( mScrollWidgetWrapper );
    // Append to domestic tabs
    mDomesticTabs.append( iTab );
    // Set tab geometry as the last one
    iTab->move( ( NTabs() - 1 ) * ( GetTabWidth() - mOverlap ), 0 );
    iTab->resize( GetTabWidth(), height() );
    iTab->SetAnimatedMovement( QPoint( ( NTabs() - 1 ) * ( GetTabWidth() - mOverlap ), 0 ) );

    // Setup lifted connection
    QObject::connect( iTab, SIGNAL( Lifted( FTab* ) ), this, SLOT( DomesticTabLifted( FTab* ) ) );
    QObject::connect( iTab, SIGNAL( Selected( FTab* ) ), this, SLOT( DomesticTabSelected( FTab* ) ) );
    QObject::connect( iTab, SIGNAL( CloseClicked( FTab* ) ), this, SLOT( CloseTab( FTab* ) ) );
    // This is generally required for a dynamically created widget to show properly

    iTab->SetClosable( mTabsClosable );
    iTab->SetLiftable( mTabsLiftable );
    iTab->SetTabShape( mTabsShapeStyle );
    iTab->SetOnTabDroppedOutCB( mOnTabDroppedOutCB );
    iTab->SetTag( mTag );

    if( iTab->GetLinkWidget() )
        if( GetLinkedStack() )
            if( GetLinkedStack()->indexOf( iTab->GetLinkWidget() ) == -1 )
                GetLinkedStack()->addWidget( iTab->GetLinkWidget() );

    SetCurrentTab( iTab );
    iTab->show();
    Compose();
}


void
FTabArea::DockHere( FTab* iTab )
{
    // Remove previous Hooks
    SetNoCandidateTab();
    iTab->removeEventFilter(this );
    QObject::disconnect( iTab, SIGNAL( Dropped( FTab* ) ), this, SLOT( DomesticTabDropped(FTab*) ) );
    QObject::disconnect( iTab, SIGNAL( Dropped( FTab* ) ), this, SLOT( ForeignTabDropped(FTab*) ) );
    // Install lift signal
    QObject::connect( iTab, SIGNAL( Lifted( FTab* ) ), this, SLOT( DomesticTabLifted( FTab* ) ) );
    QObject::connect( iTab, SIGNAL( Selected( FTab* ) ), this, SLOT( DomesticTabSelected( FTab* ) ) );
    QObject::connect( iTab, SIGNAL( CloseClicked( FTab* ) ), this, SLOT( CloseTab( FTab* ) ) );

    mDomesticTabs.append( iTab );
    iTab->setParent( mScrollWidgetWrapper );
    QRect tabGeometry = MapRectFromGlobal( mScrollWidgetWrapper, iTab->geometry() );
    iTab->setGeometry(tabGeometry);
    iTab->resize( GetTabWidth(), height() );

    iTab->SetClosable( mTabsClosable );
    iTab->SetLiftable( mTabsLiftable );
    iTab->SetTabShape( mTabsShapeStyle );
    iTab->SetOnTabDroppedOutCB( mOnTabDroppedOutCB );

    if( iTab->GetLinkWidget() )
        if( GetLinkedStack() )
            if( GetLinkedStack()->indexOf( iTab->GetLinkWidget() ) == -1 )
                GetLinkedStack()->addWidget( iTab->GetLinkWidget() );

    SetCurrentTab( iTab );
    iTab->show();
    Compose();
}


void
FTabArea::SetCurrentTab( FTab* iTab )
{
    if( iTab )
    {
        assert( mDomesticTabs.contains( iTab ) );

        if( iTab != mCurrentTab )
            emit  CurrentTabChanged( iTab );


        if( iTab->GetLinkWidget() )
        {
            if( GetLinkedStack() )
            {
                if( GetLinkedStack()->indexOf( iTab->GetLinkWidget() ) != -1 )
                {
                    GetLinkedStack()->setCurrentWidget( iTab->GetLinkWidget() );
                    iTab->GetLinkWidget()->setFocus();
                }
            }
        }

        mCurrentTab = iTab;
        mCurrentTab->SetActive();
        Reorder();
        mScroller->ensureVisible( mCurrentTab->TargetGeometry(), 0, 0, DEFAULT_ENSURE_TIME );
        repaint();
    }
    else
    {
        mCurrentTab = NULL;
        Reorder();
        repaint();
    }
}


int
FTabArea::NTabs()  const
{
    return  mDomesticTabs.count();
}



void
FTabArea::SetCandidateTab( FTab* iTab )
{
    if( iTab == mCandidateTab )
        return;

    mCandidateTab = iTab;

    if( mCandidateTab == NULL )
        mProcessCandidateTimer->stop();
    else
        mProcessCandidateTimer->start();
}


void
FTabArea::SetNoCandidateTab()
{
    SetCandidateTab( NULL );
    mProcessCandidateTimer->stop();
}


void
FTabArea::SetLinkedStack( QStackedWidget* iStack )
{
    mLinkedStack = iStack;
}


QStackedWidget*
FTabArea::GetLinkedStack()  const
{
    return  mLinkedStack;
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------- Tab-Specific Geometry Ordering


void
FTabArea::Reorder()
{
    // Do sort
    qSort( mDomesticTabs.begin(), mDomesticTabs.end(), SortBasicGeo );

    // After sort, process ordering.
    for( int i = 0; i < mDomesticTabs.count(); ++i )
    {
        auto debug_geom = mDomesticTabs[i]->geometry();

        // setup animated movement
        mDomesticTabs[i]->SetAnimatedMovement( QPoint( i * ( GetTabWidth() - mOverlap ), 0 ) );
        if( mDomesticTabs[i] != mCurrentTab )
            mDomesticTabs[i]->SetActive( false );

        // Tab z order
        if( i > 0 )
        {
            if( mDomesticTabs[i] == mCurrentTab )
                mCurrentTab->raise();
            else
                mDomesticTabs[i]->stackUnder( mDomesticTabs[ i - 1] );
        }
    }

    if( mCurrentTab )
        mCurrentTab->raise();
}


void
FTabArea::Recompose()
{
    Compose();
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Overlap Size Style accessors


void
FTabArea::SetOverlap( int iValue )
{
    mOverlap = iValue;
    Compose();
    repaint();
}


void
FTabArea::SetMaximumTabWidth( int iWidth )
{
    mMaximumTabWidth = iWidth;
    Compose();
    repaint();
}


void
FTabArea::SetMinimumTabWidth( int iWidth )
{
    mMinimumTabWidth = iWidth;
    Compose();
    repaint();
}


void
FTabArea::SetTabsShapeStyle( FTab::eTabShape iValue )
{
    mTabsShapeStyle = iValue;
    for ( FTab* t : mDomesticTabs )
        t->SetTabShape( iValue );
}


void
FTabArea::SetTabsClosable( bool iValue )
{
    mTabsClosable = iValue;
    for ( FTab* t : mDomesticTabs )
        t->SetClosable( iValue );
}


void
FTabArea::SetTabsLiftable( bool iValue )
{
    mTabsLiftable = iValue;
    for ( FTab* t : mDomesticTabs )
        t->SetLiftable( iValue );
}



void
FTabArea::SetOnAreaBecomesEmptyCB( OnAreaBecomesEmptyCB iOnAreaBecomesEmptyCB )
{
    mOnAreaBecomesEmptyCB = iOnAreaBecomesEmptyCB;
}


void
FTabArea::SetOnTabDroppedOutCB( OnTabDroppedOutCB iOnTabDroppedOutCB )
{
    mOnTabDroppedOutCB = iOnTabDroppedOutCB;
}


int
FTabArea::GetOverlap()  const
{
    return  mOverlap;
}


int
FTabArea::GetMaximumTabWidth()  const
{
    return  mMaximumTabWidth;
}


int
FTabArea::GetMinimumTabWidth()  const
{
    return  mMinimumTabWidth;
}


int
FTabArea::GetTabWidth()  const
{
    return  mTabWidth;
}

FTab::eTabShape
FTabArea::GetTabsShapeStyle()  const
{
    return  mTabsShapeStyle;
}


bool
FTabArea::GetTabsClosable()  const
{
    return  mTabsClosable;
}


bool
FTabArea::GetTabsLiftable()  const
{
    return  mTabsLiftable;
}


OnAreaBecomesEmptyCB
FTabArea::GetOnAreaBecomesEmptyCB()  const
{
    return  mOnAreaBecomesEmptyCB;
}


OnTabDroppedOutCB
FTabArea::GetOnTabDroppedOutCB()  const
{
    return  mOnTabDroppedOutCB;
}

void
FTabArea::SetBlackControls()
{
    QColor black = Qt::black;
    mLeftButton->SetColor( black );
    mLeftButton->SetHoveredColor( black );
    mLeftButton->SetPressedColor( black );

    mRightButton->SetColor( black );
    mRightButton->SetHoveredColor( black );
    mRightButton->SetPressedColor( black );
}


void
FTabArea::SetWhiteControls()
{
    QColor white = Qt::white;
    mLeftButton->SetColor( white );
    mLeftButton->SetHoveredColor( white );
    mLeftButton->SetPressedColor( white );

    mRightButton->SetColor( white );
    mRightButton->SetHoveredColor( white );
    mRightButton->SetPressedColor( white );
}


void
FTabArea::EnableBlurEffectControls( bool iValue )
{
    mLeftDropShadowEffect->setEnabled( iValue );
    mRightDropShadowEffect->setEnabled( iValue );
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------- ID tag Interface


void
FTabArea::SetTag( const  QString&  iTag )
{
    mTag = iTag;
}


const  QString&
FTabArea::GetTag()  const
{
    return  mTag;
}

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//---------------------------------------------- Private Tab Composing & Style utilities


void
FTabArea::SetTabWidth( int iWidth )
{
    // Avoid recursive calls in reorder.
    mTabWidth = iWidth;
    mTabWidth = mTabWidth > mMaximumTabWidth ? mMaximumTabWidth : mTabWidth;
    mTabWidth = mTabWidth < mMinimumTabWidth ? mMinimumTabWidth : mTabWidth;

    for ( FTab* t : mDomesticTabs )
        t->resize( mTabWidth, height() );
}


int
FTabArea::ComputeSmartWidthWithOverlap()  const
{
    // N Tabs
    int n = NTabs();
    // Width + extra overlap space for N - 1
    int extraw = width() + mOverlap * ( n - 1 );
    // new tabWidth default
    int smartWidth = GetTabWidth();
    if( n > 0 )
        smartWidth = extraw / n;

    return  smartWidth;
}


void
FTabArea::RecomputeCurrentTabOnImminentChange()
{
    int indexOfCurrent = mDomesticTabs.indexOf( mCurrentTab );

    if( mDomesticTabs.count() == 0 )
    {
        // Error
    }
    else if( mDomesticTabs.count() == 1 )
    {
        mCurrentTab = NULL;
    }
    else
    {
        if( indexOfCurrent == mDomesticTabs.count() - 1 )
            --indexOfCurrent;
        else
            ++indexOfCurrent;

        mCurrentTab = mDomesticTabs[ indexOfCurrent ];
    }
}


void
FTabArea::TakeActionOnEmpty()
{
    // The dropped tab was the last in this area
    bool last       = mDomesticTabs.count() == 0;

    if( last && DockingManager()->CurrentTargetArea() != this )
    {
        emit  AreaEmpty( this );
        if( mOnAreaBecomesEmptyCB )
            mOnAreaBecomesEmptyCB( this );
    }
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Qt Events overrides


void
FTabArea::resizeEvent( QResizeEvent *event )
{
    tSuperClass::resizeEvent(event);
    Compose();
}


void
FTabArea::wheelEvent( QWheelEvent* event )
{
    tSuperClass::wheelEvent( event );

    QPointF target = mScroller->finalPosition();
    int targetx = target.x();

    int currentx = mScrollArea->horizontalScrollBar()->value();
    int oldDelta = targetx - currentx;
    float  wheelRatio = 0.5f;
    int newDelta = event->angleDelta().y() * wheelRatio + event->angleDelta().x() * wheelRatio;
    mScroller->stop();
    mScroller->scrollTo( QPoint( targetx - newDelta, 0 ), 50 );
}


//--------------------------------------------------------------------------------------
//----------------------------------------------------- Private GUI Processing Functions


void
FTabArea::Init()
{
    if( !mScrollArea )              mScrollArea             = new  QScrollArea( this );
    if( !mScrollWidgetWrapper )     mScrollWidgetWrapper    = new  QWidget( this );
    if( !mLeftButton )              mLeftButton             = new  FCustomButton( this );
    if( !mRightButton )             mRightButton            = new  FCustomButton( this );
    if( !mLeftDropShadowEffect )    mLeftDropShadowEffect   = new  QGraphicsDropShadowEffect( this );
    if( !mRightDropShadowEffect )   mRightDropShadowEffect  = new  QGraphicsDropShadowEffect( this );
    if( !mNavTimerLeft )            mNavTimerLeft           = new  QTimer( this );
    if( !mNavTimerRight )           mNavTimerRight          = new  QTimer( this );
    if( !mProcessCandidateTimer )   mProcessCandidateTimer  = new  QTimer( this );
}


void
FTabArea::Build()
{
    mScrollArea->verticalScrollBar()->setEnabled( false );
    mScrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    mScrollArea->setWidget( mScrollWidgetWrapper );

    mScroller = QScroller::scroller( mScrollArea->viewport() );
    mScroller->grabGesture( mScrollArea->viewport(), QScroller::RightMouseButtonGesture );

    QScrollerProperties ScrollerProperties = mScroller->scrollerProperties();
    QVariant OvershootPolicyOff = QVariant::fromValue< QScrollerProperties::OvershootPolicy >( QScrollerProperties::OvershootAlwaysOff );
    QVariant OvershootPolicyOn  = QVariant::fromValue< QScrollerProperties::OvershootPolicy >( QScrollerProperties::OvershootAlwaysOn );
    ScrollerProperties.setScrollMetric( QScrollerProperties::VerticalOvershootPolicy,       OvershootPolicyOff );
    ScrollerProperties.setScrollMetric( QScrollerProperties::HorizontalOvershootPolicy,     OvershootPolicyOn );
    ScrollerProperties.setScrollMetric( QScrollerProperties::OvershootDragDistanceFactor,   0.3 );
    ScrollerProperties.setScrollMetric( QScrollerProperties::OvershootScrollDistanceFactor, 0.3 );
    ScrollerProperties.setScrollMetric( QScrollerProperties::OvershootScrollTime,           0.3 );
    ScrollerProperties.setScrollMetric( QScrollerProperties::ScrollingCurve,                QEasingCurve::InOutQuart );
    mScroller->setScrollerProperties( ScrollerProperties );

    // Temporary debug stylesheet should move to a qss separate file.
    /*
    setStyleSheet(  "QScrollArea { background: transparent; border: none; }"
                    "QScrollArea > QWidget > QWidget { background: transparent; border: none; }"
                    "QScrollArea > QWidget > QScrollBar { background: palette(base); border: none; }" );
                    */

    mRightButton->SetIconShape( FCustomButton::eButtonIconShape::kRightArrow );
    mLeftButton->SetIconShape( FCustomButton::eButtonIconShape::kLeftArrow );
    mRightButton->SetBackgroundColor( QColor( 58, 58, 58 ) );
    mLeftButton->SetBackgroundColor( QColor( 58, 58, 58 ) );
    mRightButton->SetHoveredBackgroundColor( QColor( 68, 68, 68 ) );
    mLeftButton->SetHoveredBackgroundColor( QColor( 68, 68, 68 ) );
    mRightButton->SetPressedBackgroundColor( QColor( 48, 48, 48 ) );
    mLeftButton->SetPressedBackgroundColor( QColor( 48, 48, 48 ) );

    // Temporary debug stylesheet should move to a qss separate file.
    /*
    mRightButton->setStyleSheet( "background: rgb( 58, 58, 58 ); border-left:1px solid rgb( 100, 100, 100 );" );
    mLeftButton->setStyleSheet( "background: rgb( 58, 58, 58 ); border-right: 1px solid rgb( 100, 100, 100 );" );
    */

    QObject::connect( mRightButton, SIGNAL( pressed() ), this, SLOT( NavRightDown() ) );
    QObject::connect( mLeftButton, SIGNAL( pressed() ), this, SLOT( NavLeftDown() ) );

    QObject::connect( mRightButton, SIGNAL( released() ), this, SLOT( NavRightUp() ) );
    QObject::connect( mLeftButton, SIGNAL( released() ), this, SLOT( NavLeftUp() ) );

    QObject::connect( mRightButton, SIGNAL( DoubleClicked() ), this, SLOT( NavRightDoubleClick() ) );
    QObject::connect( mLeftButton, SIGNAL( DoubleClicked() ), this, SLOT( NavLeftDoubleClick() ) );

    mLeftDropShadowEffect->setBlurRadius( 8 );
    mLeftDropShadowEffect->setColor( QColor( 0, 0, 0, 255 ) );
    mLeftDropShadowEffect->setOffset( 0, 0 );
    mLeftDropShadowEffect->setEnabled( true );
    mLeftButton->setGraphicsEffect( mLeftDropShadowEffect );

    mRightDropShadowEffect->setBlurRadius( 8 );
    mRightDropShadowEffect->setColor( QColor( 0, 0, 0, 255 ) );
    mRightDropShadowEffect->setOffset( 0, 0 );
    mRightDropShadowEffect->setEnabled( true );
    mRightButton->setGraphicsEffect( mRightDropShadowEffect );

    mNavTimerLeft->setInterval( 1000 / DEFAULT_NAV_WORK_FPS );
    mNavTimerRight->setInterval( 1000 / DEFAULT_NAV_WORK_FPS );
    QObject::connect( mNavTimerLeft, SIGNAL( timeout() ), this, SLOT( NavLeftWork() ) );
    QObject::connect( mNavTimerRight, SIGNAL( timeout() ), this, SLOT( NavRightWork() ) );

    mProcessCandidateTimer->setInterval( 1000 / DEFAULT_PROCESS_FPS );
    QObject::connect( mProcessCandidateTimer, SIGNAL( timeout() ), this, SLOT( ProcessCandidateDragHover() ) );
}


void
FTabArea::Compose()
{
    ComposeScrollArea();
    Reorder();
}


void
FTabArea::Destroy()
{
    // Delete all domestic tabs
    for( FTab* t : mDomesticTabs )
        delete  t;

    delete  mProcessCandidateTimer;
    delete  mNavTimerLeft;
    delete  mNavTimerRight;
    delete  mRightDropShadowEffect;
    delete  mLeftDropShadowEffect;
    delete  mScrollWidgetWrapper;
    delete  mScrollArea;
    delete mLeftButton;
    delete mRightButton;

    mScrollArea             = 0;
    mScrollWidgetWrapper    = 0;
    mLeftButton             = 0;
    mRightButton            = 0;
    mRightDropShadowEffect  = 0;
    mLeftDropShadowEffect   = 0;
    mNavTimerLeft           = 0;
    mNavTimerRight          = 0;
    mProcessCandidateTimer  = 0;
}


//--------------------------------------------------------------------------------------
//-------------------- Private GUI Processing Functions specific to tab area with scroll

void
FTabArea::ComposeScrollArea()
{
    SetTabWidth( ComputeSmartWidthWithOverlap() );

    if( GetTabWidth() <= GetMinimumTabWidth() )
    {
        mLeftButton->setGeometry( 0, 0, height(), height() );
        mRightButton->setGeometry( width() - height(), 0, height(), height() );
        mScrollArea->setGeometry( height(), 0, width() - height() * 2, height() * 2 );
    }
    else
    {
        mLeftButton->setGeometry( -height() + 1, 0, height(), height() );
        mRightButton->setGeometry( width() - 1, 0, height(), height() );
        mScrollArea->setGeometry( 1, 0, width() - 2, height() * 2 );
    }

    int wrapw = ( NTabs() * GetTabWidth() ) - ( ( NTabs() - 1 ) * mOverlap );
    int ref = mScrollArea->width();
    wrapw = wrapw < ref ? ref : wrapw;
    int arbitraryFixWiggle = 2;
    mScrollWidgetWrapper->resize( wrapw - arbitraryFixWiggle, height() );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------ Docking Manager Registering API


void
FTabArea::Register()
{
    DockingManager()->RegisterTabArea( this );
}


void
FTabArea::Unregister()
{
    DockingManager()->UnregisterTabArea( this );
}


////////////////////////////////////////////////////////////////////////////////////////
////                              SIGNAL SLOTS API                                  ////
////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------- Debug Slots


/*
void
TabArea::_NewTab()
{
    // This is a debug function that shouldn't be used in a real application
    static  int  count = 0;

    // Dynamically alloc new tab instance, delete is handled by the tab area upon close event.
    Tab*  newTab = new  Tab( mScrollWidgetWrapper );

    // Append here to domestic proprietary tabs
    mDomesticTabs.append( newTab );

    // Set tab geometry as the last one
    newTab->move( ( NTabs() - 1 ) * GetTabWidth(), 0 );
    newTab->resize( GetTabWidth(), height() );

    // Compute title
    QString title = QString( "Screen #" ) + QString::number( count );
    newTab->SetTitle( title );
    newTab->SetColor( _NextDefaultColor() );
    newTab->SetTabShape( mTabs_Style );

    // Important !! Setup lifted connection signal.
    QObject::connect( newTab, SIGNAL( Lifted( Tab* ) ), this, SLOT( DomesticTabLifted( Tab* ) ) );
    QObject::connect( newTab, SIGNAL( Selected( Tab* ) ), this, SLOT( DomesticTabSelected( Tab* ) ) );
    QObject::connect( newTab, SIGNAL( CloseClicked( Tab* ) ), this, SLOT( CloseTab( Tab* ) ) );

    // This is generally required for a dynamically created widget to show properly.
    newTab->show();

    // inc debug count
    ++count;

    // Reorder geometry & tab ordering, and scroll to the newly added widget.
    Compose();
    SetCurrentTab( newTab );
}
*/


//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------- Dragging Slots


void
FTabArea::DomesticTabLifted( FTab* iTab )
{
    assert( mDomesticTabs.contains( iTab ) );
    assert( mCurrentTab == iTab );
    RecomputeCurrentTabOnImminentChange();

    // Disconnect this slot
    QObject::disconnect( iTab, SIGNAL( Lifted( FTab* ) ), this, SLOT( DomesticTabLifted( FTab* ) ) );
    QObject::disconnect( iTab, SIGNAL( Selected( FTab* ) ), this, SLOT( DomesticTabSelected( FTab* ) ) );
    QObject::disconnect( iTab, SIGNAL( CloseClicked( FTab* ) ), this, SLOT( CloseTab( FTab* ) ) );
    // Remove iTab from Domestic Tabs ( only one occurence )
    mDomesticTabs.removeAll( iTab );

    // Connect iTab Dropped Signal to DomesticTabDropped
    QObject::connect( iTab, SIGNAL( Dropped( FTab* ) ), this, SLOT( DomesticTabDropped( FTab* ) ) );

    DockingManager()->SetLastLiftedFrom( this );
}


void
FTabArea::DomesticTabDropped( FTab* iTab )
{
    if( DockingManager()->CurrentTargetArea() == this )
        return;

    if( iTab->GetLinkWidget() )
        if( GetLinkedStack() )
            if( GetLinkedStack()->indexOf( iTab->GetLinkWidget() ) != -1 )
                GetLinkedStack()->removeWidget( iTab->GetLinkWidget() );

    Compose();
    SetCurrentTab( mCurrentTab );
    QObject::disconnect( iTab, SIGNAL( Dropped( FTab* ) ), this, SLOT( DomesticTabDropped( FTab* ) ) );
    SetNoCandidateTab();
    TakeActionOnEmpty();
}


void
FTabArea::ForeignTabDropped( FTab* iTab )
{
    // This is the important part:
    // If we get this message that means the foreign tab is dropped here.
    SetNoCandidateTab();
    DockHere(iTab);

    // Now this is optional if we want to activate the window once the tab has been docked
    // the set focus method just doesn't work
    topLevelWidget()->setFocus();
    // This does activate & raise the window but doesn't grab the focus
    topLevelWidget()->raise();  // for MacOS
    topLevelWidget()->activateWindow(); // for Windows
    // the event object is released then in event loop (?)
    QFocusEvent* eventFocus = new QFocusEvent( QEvent::FocusIn );
    // posting event for forcing the focus with low priority
    qApp->postEvent( this, (QEvent*)eventFocus, Qt::LowEventPriority );

}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------- Navigation Slots


void
FTabArea::NavDelta( int iDelta, int iTimeMS )
{
    int currentx = mScrollArea->horizontalScrollBar()->value();
    mScrollArea->horizontalScrollBar()->setValue( currentx + iDelta );
}


void
FTabArea::NavLeftDoubleClick()
{
    if( mRightButton->geometry().left() <= mLeftButton->geometry().right() + 5 )
        return;

    mScroller->stop();
    mScroller->ensureVisible( mDomesticTabs.first()->geometry(), 0, 0, DEFAULT_ENSURE_TIME );
}


void
FTabArea::NavLeftDown()
{
    mNavTimerLeft->start();
}


void
FTabArea::NavLeftWork()
{
    NavDelta( -10, DEFAULT_NAV_TIME );
}


void
FTabArea::NavLeftUp()
{
    mNavTimerLeft->stop();
}


void
FTabArea::NavRightDoubleClick()
{
    if( mRightButton->geometry().left() <= mLeftButton->geometry().right() + 5 )
        return;

    mScroller->stop();
    mScroller->ensureVisible( mDomesticTabs.last()->geometry(), 0, 0, DEFAULT_ENSURE_TIME );
}


void
FTabArea::NavRightDown()
{
    mNavTimerRight->start();
}


void
FTabArea::NavRightWork()
{
    NavDelta( 10, DEFAULT_NAV_TIME );
}


void
FTabArea::NavRightUp()
{
    mNavTimerRight->stop();
}


void
FTabArea::ProcessCandidateDragHover()
{
    assert( mCandidateTab );
    assert( mCandidateTab->Dragging() );
    // Collect tab geometry from global, expressed in the same frame of ref as other tabs in this area.
    QRect tabGeometry = MapRectFromGlobal( mScrollWidgetWrapper, mCandidateTab->geometry() );

    QVector< FTabOrderingPair > orderingVector;
    for( FTab* t : mDomesticTabs )
        orderingVector.append( FTabOrderingPair( { t->TargetGeometry(), t } ) );

    orderingVector.append( FTabOrderingPair( { tabGeometry, mCandidateTab } ) );
    qSort( orderingVector.begin(), orderingVector.end(), SortBasic );

    // After sort, process ordering.
    for( int i = 0; i < orderingVector.count(); ++i )
    {
        // But Skip dragging tab
        if( orderingVector[i].mTab == mCandidateTab )
            continue;

        orderingVector[i].mTab->SetAnimatedMovement( QPoint( i * ( GetTabWidth() - mOverlap ), 0 ) );
    }

    QPoint gmousePos = mCandidateTab->pos() + mCandidateTab->DragShift();
    QPoint local = mapFromGlobal( gmousePos );

    if( mLeftButton->geometry().contains( local ) )
        NavLeftWork();

    if( mRightButton->geometry().contains( local ) )
        NavRightWork();
}


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Tab Control Slots


void
FTabArea::DomesticTabSelected( FTab* iTab )
{
    SetCurrentTab( iTab );
}


void
FTabArea::CloseCurrentTab()
{
    CloseTab( mCurrentTab );
}


void
FTabArea::SwitchToNextTab()
{
    if( mDomesticTabs.count() < 2 )
        return;

    int indexOfCurrent = mDomesticTabs.indexOf( mCurrentTab );
    int newIndex = ( ++indexOfCurrent ) % mDomesticTabs.count();
    mCurrentTab = mDomesticTabs[ newIndex ];
    SetCurrentTab( mCurrentTab );
}


void
FTabArea::CloseTab( FTab* iTab )
{
    if( !iTab )
        return;

    if( !iTab->IsClosable() )
        return

    assert( mDomesticTabs.contains( iTab ) );

    if( iTab == mCurrentTab )
        RecomputeCurrentTabOnImminentChange();

    mDomesticTabs.removeAll( iTab );
    iTab->close();
    iTab->deleteLater();

    if( iTab->GetLinkWidget() )
        if( GetLinkedStack() )
            if( GetLinkedStack()->indexOf( iTab->GetLinkWidget() ) != -1 )
                GetLinkedStack()->removeWidget( iTab->GetLinkWidget() );

    Compose();
    repaint();
    SetCurrentTab( mCurrentTab );

    TakeActionOnEmpty();
}

