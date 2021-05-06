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

#include <QBoxLayout>
#include <QColor>
#include <QHeaderView>
#include <QScrollBar>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTableView>
#include <QFontDatabase>
#include <QTextEdit>

#include <functional>

#include <ULIS>

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
    setMinimumSize( 800, 600 );
    SetCaptionWidget( mCaption );
    SetCenterWidget( mCenter );
    EnableBlurBehind();

    mCode = new QTextEdit();
    mConsole = new QTextEdit();
    mMetrics = new QTableView();
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
            SDef( setMinimumWidth( 270 ) )
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
                                    // metrics
                                    SExistingChild( QTableView, mMetrics )
                                    SDef( setMinimumWidth( 250 ) )
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
                        SExistingChild( QTextEdit, mConsole )
                        SDef( setMinimumHeight( 200 ) )
                    )
                    SDef( setSizes( QList< int >( { 800, 200 } ) ) )
                )
            )
        )
        SAddWidget(
            // mCode
            SExistingChild( QTextEdit, mCode )
            SDef( setMinimumWidth( 200 ) )
        )
        SDef( setSizes( QList< int >( { 800, 200 } ) ) )
    );


    ::ULIS::FHardwareMetrics hw;
    const char* out[] = { "NO", "YES" };
    QString keys[] = {
          "Vendor_AMD"
        , "Vendor_Intel"
        , "OS_x64"
        , "OS_AVX"
        , "OS_AVX512"
        , "HW_MMX"
        , "HW_x64"
        , "HW_ABM"
        , "HW_RDRAND"
        , "HW_BMI1"
        , "HW_BMI2"
        , "HW_ADX"
        , "HW_PREFETCHWT1 "
        , "HW_MPX"
        , "HW_SSE"
        , "HW_SSE2"
        , "HW_SSE3"
        , "HW_SSSE3"
        , "HW_SSE41"
        , "HW_SSE42"
        , "HW_SSE4a"
        , "HW_AES"
        , "HW_SHA"
        , "HW_AVX"
        , "HW_XOP"
        , "HW_FMA3"
        , "HW_FMA4"
        , "HW_AVX2"
        , "HW_AVX512_F"
        , "HW_AVX512_PF"
        , "HW_AVX512_ER"
        , "HW_AVX512_CD"
        , "HW_AVX512_VL"
        , "HW_AVX512_BW"
        , "HW_AVX512_DQ"
        , "HW_AVX512_IFMA "
        , "HW_AVX512_VBMI "
        , "MAXWORKERS"
        , "L1CACHE"
        , "L1CACHELINE"
    };

    QString values[] = {
          out[ hw.IsHardwareAMD()          ]
        , out[ hw.IsHardwareIntel()        ]
        , out[ hw.IsOSx64()                ]
        , out[ hw.HasOSAVX()               ]
        , out[ hw.HasOSAVX512()            ]
        , out[ hw.HasHardwarex64()         ]
        , out[ hw.HasHardwareMMX()         ]
        , out[ hw.HasHardwareABM()         ]
        , out[ hw.HasHardwareRDRAND()      ]
        , out[ hw.HasHardwareBMI1()        ]
        , out[ hw.HasHardwareBMI2()        ]
        , out[ hw.HasHardwareADX()         ]
        , out[ hw.HasHardwarePREFETCHWT1() ]
        , out[ hw.HasHardwareMPX()         ]
        , out[ hw.HasHardwareSSE()         ]
        , out[ hw.HasHardwareSSE2()        ]
        , out[ hw.HasHardwareSSE3()        ]
        , out[ hw.HasHardwareSSSE3()       ]
        , out[ hw.HasHardwareSSE41()       ]
        , out[ hw.HasHardwareSSE42()       ]
        , out[ hw.HasHardwareSSE4a()       ]
        , out[ hw.HasHardwareAES()         ]
        , out[ hw.HasHardwareSHA()         ]
        , out[ hw.HasHardwareAVX()         ]
        , out[ hw.HasHardwareXOP()         ]
        , out[ hw.HasHardwareFMA3()        ]
        , out[ hw.HasHardwareFMA4()        ]
        , out[ hw.HasHardwareAVX2()        ]
        , out[ hw.HasHardwareAVX512_F()    ]
        , out[ hw.HasHardwareAVX512_PF()   ]
        , out[ hw.HasHardwareAVX512_ER()   ]
        , out[ hw.HasHardwareAVX512_CD()   ]
        , out[ hw.HasHardwareAVX512_VL()   ]
        , out[ hw.HasHardwareAVX512_BW()   ]
        , out[ hw.HasHardwareAVX512_DQ()   ]
        , out[ hw.HasHardwareAVX512_IFMA() ]
        , out[ hw.HasHardwareAVX512_VBMI() ]
        , QString::number( hw.MaxWorkers() )
        , QString::number( hw.L1CacheSize() )
        , QString::number( hw.L1CacheLineSize() )
    };

    QFont metricsFont = QFontDatabase::systemFont( QFontDatabase::SmallestReadableFont );
    metricsFont.setPointSize( 8 );

    QStandardItemModel* mod = new QStandardItemModel( sizeof( keys ) / sizeof( QString ), 2 );
    float hue = 0.f;
    for( int i = 0; i < mod->rowCount(); ++i ) {
        QStandardItem* keyItem = new QStandardItem( keys[i] );
        QStandardItem* valItem = new QStandardItem( values[i] );
        keyItem->setSelectable( false );
        float val = i % 2 ? 0.5f : 0.45f;
        keyItem->setBackground( QBrush( QColor::fromHsvF( hue, 0.5f, val ) ) );
        keyItem->setForeground( QBrush( QColor::fromRgbF( 1.f, 1.f, 1.f ) ) );
        keyItem->setFont( metricsFont );

        valItem->setBackground( QBrush( QColor::fromHsvF( hue, 0.5f, val ) ) );
        valItem->setForeground( QBrush( QColor::fromRgbF( 1.f, 1.f, 1.f ) ) );
        valItem->setSelectable( false );
        valItem->setTextAlignment( Qt::AlignRight | Qt::AlignVCenter );
        valItem->setFont( metricsFont );

        mod->setItem( i, 0, keyItem );
        mod->setItem( i, 1, valItem );
        hue = fmodf( hue + 0.025f, 1.f );
    }
    mMetrics->setModel( mod );
    mMetrics->setShowGrid( false );
    mMetrics->setSelectionBehavior( QAbstractItemView::SelectionBehavior::SelectRows );
    mMetrics->setSelectionMode( QAbstractItemView::SelectionMode::NoSelection );
    mMetrics->verticalHeader()->setVisible( false );
    mMetrics->verticalHeader()->setDefaultSectionSize( 8 );
    mMetrics->horizontalHeader()->setVisible( false );
    mMetrics->horizontalHeader()->setStretchLastSection( true );
    mMetrics->resizeColumnsToContents();
    mMetrics->setEditTriggers( QAbstractItemView::EditTrigger::NoEditTriggers );
    mMetrics->setFocusPolicy( Qt::NoFocus );
    mMetrics->setFrameStyle( QFrame::NoFrame );

    mConsole->setFrameStyle( QFrame::NoFrame );
    mConsole->setReadOnly( true );
    mConsole->append( "[cpp]> " + QString( ::ULIS::FullLibraryInformationString().Data() ) );
    mConsole->append( "[cpp]> Hello World !" );
    mConsole->setLineWrapMode( QTextEdit::LineWrapMode::NoWrap );
    mConsole->setOverwriteMode( true );
    //for( int i = 0; i < 10; ++i )
    //    mConsole->append( QString::number( i ) + ">>>" );
    mConsole->verticalScrollBar()->setValue( mConsole->verticalScrollBar()->maximum() );

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    //font.setFamily( "Courier" );
    //font.setHintingPreference( QFont::PreferNoHinting );
    font.setStyleHint( QFont::Monospace );
    font.setStyleStrategy( QFont::NoAntialias );
    font.setFixedPitch( true );
    font.setPointSize( 11 );

    mCode->setFrameStyle( QFrame::NoFrame );
    mCode->setText(
        "from pyULIS4 import *\n"
        "\n"
        "pool = FThreadPool()\n"
        "queue = FCommandQueue( pool )\n"
        "fmt = Format_RGBA8\n"
        "ctx = FContext( pool, fmt )\n"
        "canvas = FBlock( 800, 600, fmt )\n"
        "\n"
        "def start():\n"
        "	ctx.Clear( canvas )\n"
        "	ctx.Finish()\n"
        "\n"
        "def update( delta ):\n"
        "	ctx.Fill( canvas, FColor.Black )\n"
        "	ctx.Finish()\n"
        "\n"
        "\n"
    );
    mCode->setLineWrapMode( QTextEdit::LineWrapMode::NoWrap );
    mCode->setObjectName( "Code" );
    mCode->setFont(font);
    QFontMetrics metrics( font );
    mCode->setTabStopWidth( 4 * metrics.width(' ') );
}

