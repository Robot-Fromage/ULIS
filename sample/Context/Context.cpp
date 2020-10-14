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

using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat format = Format_RGBA8;
    FContext ctx( queue, format );
    FBlock canvas( 256, 256, format );
    FBlock test( 64, 64, format );

    FEvent eventBlendTile;
    FSchedulePolicy policy( ScheduleRun_Mono );
    ctx.Blend( test, canvas, test.Rect(), FVec2I( 0, 0 ),  Blend_Normal, Alpha_Normal, 1.f, policy, 0, 0, &eventBlendTile );
    ctx.Blend( test, canvas, test.Rect(), FVec2F( 64, 0 ), Blend_Normal, Alpha_Normal, 1.f, policy, 1, &eventBlendTile, 0 );
    ctx.Finish();

    return  0;
}

