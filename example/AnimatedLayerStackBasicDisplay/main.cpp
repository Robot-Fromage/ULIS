// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        AnimatedBezier application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "Editor.h"
#include <QApplication>

#include <cstdlib>
#include <ctime>
int
main( int argc, char *argv[] ) {
    // Init srand
    srand( time( NULL ) );

    // ULIS Common
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );

    // Document data
    uint16 w = 512;
    uint16 h = 256;
    FAnimatedLayerStack* stack;
    ULAAssignStack( stack, w, h, fmt )
    ULADef( SetName( "stack" ) )
    ULADef( SetFps( 6 ) )
    [
        ULACreateChild( FAnimatedLayerImage )
        ULADef( SetName( "layer0" ) )
    ];

    // Fill layer anim
    FAnimatedLayerImage& layer0 = stack->Find< FAnimatedLayerImage >( "layer0" );
    for( int i = 0; i < 5; ++i )
        layer0.PushNewCel();

    // Paint solid colors at each frames manually
    ctx.Fill( *layer0.Instances()[0]->Data(), FColor::Red );
    ctx.Fill( *layer0.Instances()[1]->Data(), FColor::Green );
    ctx.Fill( *layer0.Instances()[2]->Data(), FColor::Blue );
    ctx.Fill( *layer0.Instances()[3]->Data(), FColor::Magenta );
    ctx.Fill( *layer0.Instances()[4]->Data(), FColor::Yellow );
    ctx.Finish();

    // Display
    QApplication app( argc, argv );
    SEditor* editor = new SEditor( ctx, *stack );
    editor->show();

    // Main exec loop
    int exit_code = app.exec();

    // Cleanup
    delete  editor;
    delete  stack;
    return  exit_code;
}

