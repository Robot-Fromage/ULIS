// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS
*__________________
*
* @file         Blend.cpp
* @author       Clement Berthaud
* @brief        Blend application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>

#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>

#include <chrono>

using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat format = Format_RGBA8;
    FRasterContext ctx( queue, format );

    FBlock blockA( 256, 256, format );
    FBlock blockB( 256, 256, format );
    FBlock blockC( 256, 256, format );

    FEvent blendEventBA;
    FEvent blendEventCA;

    FSchedulePolicy policy;

    ctx.Blend( blockB, blockA, blockB.Rect(), FVec2F(), Blend_Normal, Alpha_Normal, 1.f, policy, 0, 0, &blendEventBA );
    ctx.Blend( blockC, blockA, blockC.Rect(), FVec2F(), Blend_Normal, Alpha_Normal, 1.f, policy, 1, &blendEventBA, 0 );
    ctx.Finish();

    return  0;
}

