// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainWindow.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#include "MainWindow.h"
#include "Caption.h"
#include "Code.h"
#include "Console.h"
#include "HardwareMetrics.h"
#include "SlateQtAdapter.h"
#include "SyntaxHighlight.h"
#include "Viewport.h"

#include "Rivet/TabArea.h"
#include "Rivet/Tab.h"
#include "Rivet/DockingCallbackLibrary.h"

#include <QBoxLayout>
#include <QSplitter>
#include <QStackedWidget>

#include <functional>

#include <ULIS>

SMainWindow::~SMainWindow()
{
}

SMainWindow::SMainWindow()
    : FCustomMainWindow( nullptr )
    , mCaption( nullptr )
    , mCenter( nullptr )
    , mCode( nullptr )
    , mConsole( nullptr )
    , mMetrics( nullptr )
    , mViewport( nullptr )
{
    mCaption = new SCaption();
    mCenter = new QWidget();
    setMinimumSize( 800, 600 );
    SetCaptionWidget( mCaption );
    SetCenterWidget( mCenter );
    //EnableBlurBehind();

    mCode = new SCode();
    mConsole = new SConsole();
    mMetrics = new SHardwareMetrics();
    mViewport = new SViewport( nullptr, mCode, mConsole );

    // root
    SCreateRoot( QHBoxLayout, mCenter )
    SDef( setMargin( 0 ) )
    SAddWidget(
        // splitter left-code
        SCreateChild( QSplitter )
        SDef( setChildrenCollapsible( false ) )
        SAddWidget(
            // left
            SCreateChild( QWidget )
            SDef( setMinimumWidth( 10 ) )
            SAddLayout(
                SCreateChild( QHBoxLayout )
                SDef( setMargin( 0 ) )
                SAddWidget(
                    // splitter top-console
                    SCreateChild( QSplitter )
                    SDef( setChildrenCollapsible( false ) )
                    SDef( setOrientation( Qt::Orientation::Vertical ) )
                    SAddWidget(
                        // top
                        SCreateChild( QWidget )
                        SDef( setMinimumHeight( 10 ) )
                        SAddLayout(
                            SCreateChild( QHBoxLayout )
                            SDef( setMargin( 0 ) )
                            SAddWidget(
                                // splitter metrics-viewport
                                SCreateChild( QSplitter )
                                SDef( setChildrenCollapsible( false ) )
                                SAddWidget(
                                    BuildTabSection( "Info", mMetrics )
                                )
                                SAddWidget(
                                    BuildTabSection( "Viewport", mViewport )
                                )
                                SDef( setSizes( QList< int >( { 200, 800 } ) ) )
                            )
                        )
                    )
                    SAddWidget(
                        BuildTabSection( "Console", mConsole )
                    )
                    SDef( setSizes( QList< int >( { 800, 200 } ) ) )
                )
            )
        )
        SAddWidget(
            BuildTabSection( "Code", mCode )
        )
        SDef( setSizes( QList< int >( { 400, 400 } ) ) )
    );
}

QWidget*
SMainWindow::BuildTabSection( const QString& iName, QWidget* iWidget )
{
    QStackedWidget* stack = new QStackedWidget();
    FTabArea* area = new FTabArea();
    FTab* tab = new FTab();
    auto ret = (
        SCreateChild( QWidget )
        SAddLayout(
            SCreateChild( QVBoxLayout )
            SDef( setMargin( 0 ) )
            SDef( setSpacing( 0 ) )
            SAddWidget(
                SExistingChild( FTabArea, area )
                SDef( SetMinimumTabWidth( 50 ) )
                SDef( SetMaximumTabWidth( 100 ) )
                SDef( SetOverlap( 20 ) )
                SDef( SetTabsClosable( false ) )
                SDef( SetWhiteControls() )
                SDef( SetOnAreaBecomesEmptyCB( OnAreaBecomesEmptyCB_DoNothing ) )
                SDef( SetOnTabDroppedOutCB( OnTabDroppedOutCB_RevertBack ) )
                SDef( SetLinkedStack( stack ) )
                SDef( setFixedHeight( 25 ) )
            )
            SAddWidget(
                SExistingChild( QStackedWidget, stack )
            )
        )
    ).m;

    tab->SetColor( QColor( 60, 60, 60 ) );
    tab->SetFadeColor( QColor( 40, 40, 40 ) );
    tab->SetLinkWidget( iWidget );
    tab->SetTitle( iName );
    area->ManualAddNewTab( tab );
    area->Recompose();

    return  ret;
}

