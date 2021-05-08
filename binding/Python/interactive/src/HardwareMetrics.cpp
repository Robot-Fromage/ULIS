// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HardwareMetrics.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "HardwareMetrics.h"

#include <QFontDatabase>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QColor>

#include <ULIS>

SHardwareMetrics::~SHardwareMetrics() {
}

SHardwareMetrics::SHardwareMetrics( QWidget* iParent )
    : QTableView( iParent )
{
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
    this->setModel( mod );
    this->setShowGrid( false );
    this->setSelectionBehavior( QAbstractItemView::SelectionBehavior::SelectRows );
    this->setSelectionMode( QAbstractItemView::SelectionMode::NoSelection );
    this->verticalHeader()->setVisible( false );
    this->verticalHeader()->setDefaultSectionSize( 8 );
    this->horizontalHeader()->setVisible( false );
    this->horizontalHeader()->setStretchLastSection( true );
    this->resizeColumnsToContents();
    this->setEditTriggers( QAbstractItemView::EditTrigger::NoEditTriggers );
    this->setFocusPolicy( Qt::NoFocus );
    this->setFrameStyle( QFrame::NoFrame );
}

