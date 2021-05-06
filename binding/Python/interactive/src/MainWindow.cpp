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
#include <QSplitter>
#include <QBoxLayout>
#include <functional>

FMainWindow::~FMainWindow()
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

FMainWindow::FMainWindow()
    : FCustomMainWindow( nullptr )
    , mCaption( nullptr )
    , mCenter( nullptr )
    , mCode( nullptr )
    , mConsole( nullptr )
    , mMetrics( nullptr )
    , mViewport( nullptr )
{
    mCaption = new FCaption();
    mCenter = new QWidget();
    setMinimumSize( 400, 400 );
    SetCaptionWidget( mCaption );
    SetCenterWidget( mCenter );

    mCode = new QWidget();
    mConsole = new QWidget();
    mMetrics = new QWidget();
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
            SDef( setMinimumWidth( 200 ) )
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
                        SDef( setMinimumHeight( 200 ) )
                        SAddLayout(
                            SCreateChild( QHBoxLayout )
                            SDef( setMargin( 0 ) )
                            SAddWidget(
                                // splitter metrics-viewport
                                SCreateChild( QSplitter )
                                SDef( setChildrenCollapsible( false ) )
                                SAddWidget(
                                    SExistingChild( QWidget, mMetrics )
                                    SDef( setMinimumWidth( 100 ) )
                                )
                                SAddWidget(
                                    SExistingChild( QWidget, mViewport )
                                    SDef( setMinimumWidth( 100 ) )
                                )
                            )
                        )
                    )
                    SAddWidget(
                        // console
                        SExistingChild( QWidget, mConsole )
                        SDef( setMinimumHeight( 200 ) )
                    )
                )
            )
        )
        SAddWidget(
            // mCode
            SExistingChild( QWidget, mCode )
            SDef( setMinimumWidth( 200 ) )
        )
    );
}

