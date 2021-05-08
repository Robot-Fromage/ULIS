// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainWindow.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "MainWindow.h"
#include "Caption.h"
#include "SyntaxHighlight.h"
#include "Code.h"
#include "Console.h"
#include "HardwareMetrics.h"
#include "Rivet/TabArea.h"
#include "Rivet/Tab.h"

#include <QBoxLayout>
#include <QColor>
#include <QSplitter>
#include <QStackedWidget>

#include <functional>

#include <ULIS>

SMainWindow::~SMainWindow()
{
}

template< typename T >
class SSlateQtAdapter {
public:
    SSlateQtAdapter< T >( QWidget* iParent )
        : m( new T( iParent ) )
    {}

    SSlateQtAdapter< T >( T* iElem, QWidget* iParent )
        : m( iElem )
    {
        m->setParent( iParent );
    }

    SSlateQtAdapter< T >& Def( std::function< void( T* ) > iFunc ) {
        iFunc( m );
        return  *this;
    }

    template< typename U >
    SSlateQtAdapter< T >& AddWidget( SSlateQtAdapter< U > iObj ) {
        m->addWidget( iObj.m );
        return  *this;
    }

    template< typename U >
    SSlateQtAdapter< T >& AddLayout( SSlateQtAdapter< U > iObj ) {
        m->setLayout( iObj.m );
        return  *this;
    }

public:
    T* m;
};
#define SCreateRoot( _Class_, _Parent_ )            SSlateQtAdapter< _Class_ >( _Parent_ )
#define SAssignRoot( _Class_, _Elem_, _Parent_ )    SSlateQtAdapter< _Class_ >( _Elem_, _Parent )
#define SCreateChild( _Class_ )                     SSlateQtAdapter< _Class_ >( nullptr )
#define SExistingChild( _Class_, _Child_ )          SSlateQtAdapter< _Class_ >( _Child_, nullptr )
#define SDef( ... )                                 .Def( [&]( auto i ){ i-> __VA_ARGS__ ; } )
#define SAddWidget( _Elem_ )                        .AddWidget( _Elem_ )
#define SAddLayout( _Elem_ )                        .AddLayout( _Elem_ )

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
    EnableBlurBehind();

    mCode = new SCode();
    mConsole = new SConsole();
    mMetrics = new SHardwareMetrics();
    mViewport = new QWidget();

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
                                    // metrics
                                    SExistingChild( SHardwareMetrics, mMetrics )
                                    SDef( setMinimumWidth( 10 ) )
                                )
                                SAddWidget(
                                    // viewport
                                    SExistingChild( QWidget, mViewport )
                                    SDef( setMinimumSize( 10, 10 ) )
                                )
                            )
                        )
                    )
                    SAddWidget(
                        // console
                        SExistingChild( SConsole, mConsole )
                        SDef( setMinimumHeight( 10 ) )
                    )
                    SDef( setSizes( QList< int >( { 800, 200 } ) ) )
                )
            )
        )
        SAddWidget(
            // mCode
            SExistingChild( SCode, mCode )
            SDef( setMinimumWidth( 10 ) )
        )
        SDef( setSizes( QList< int >( { 400, 400 } ) ) )
    );
}

