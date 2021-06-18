// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.cpp
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt, PerformanceIntent_Max );

    uint16 w = 1024;
    uint16 h = 1024;
    FBlock canvas( w, h, fmt );

    auto onParentChanged_print = []( const TNode< ILayer >* iSelf, const TRoot< ILayer >* iParent ) {
        const char* str_parent = iParent ? iParent->Self().Name().Data() : "nullptr";
        ULIS_DEBUG_PRINTF( iSelf->Self().Name().Data() << ": parent changed to: " << str_parent )
    };

    auto onSelfChanged_print = []( const TNode< ILayer >* iSelf ) {
        ULIS_DEBUG_PRINTF( iSelf->Self().Name().Data() << ": trigger on changed" )
    };

    auto onNodeAdded_print = []( const TRoot< ILayer >* iRoot, const TNode< ILayer >* iNode ) {
        ULIS_DEBUG_PRINTF( "Node added to container " << iRoot->Self().Name().Data() << ": " << iNode->Self().Name().Data() );
    };

    auto onNodeRemoved_print = []( const TRoot< ILayer >* iRoot, const TNode< ILayer >* iNode ) {
        // Warning: can't use iRoot->Self().Name().Data() if called during destructor.
        ULIS_DEBUG_PRINTF( "Node removed from container " << ": " << iNode->Self().Name().Data() );
    };

    FLayerStack* stack;
    ULAssignStack( stack, w, h, fmt )
    ULDef( FOnSelfChanged::SetDelegate( onSelfChanged_print ) )
    ULDef( FOnNodeAdded::SetDelegate( onNodeAdded_print ) )
    ULDef( FOnNodeRemoved::SetDelegate( onNodeRemoved_print ) )
    ULDef( AddOrSetUserData( new FTextUserData( "My test layer stack with custom callbacks and user data !" ) ) )
    [
        ULCreateChild( FLayerImage )
        ULDef( SetName( "image0" ) )
        ULDef( FOnParentChanged::SetDelegate( onParentChanged_print ) )
        ULDef( FOnSelfChanged::SetDelegate( onSelfChanged_print ) )
    ][
        ULCreateChild( FLayerFolder )
        ULDef( SetName( "folder0" ) )
        ULDef( FOnParentChanged::SetDelegate( onParentChanged_print ) )
        ULDef( FOnSelfChanged::SetDelegate( onSelfChanged_print ) )
        [
            ULCreateChild( FLayerImage )
            ULDef( SetName( "image0_0" ) )
            ULDef( FOnParentChanged::SetDelegate( onParentChanged_print ) )
            ULDef( FOnSelfChanged::SetDelegate( onSelfChanged_print ) )
        ]
    ];

    /*
    stack->AddChild(
        new FLayerImage(
              "image0"
            , false
            , true
            , FColor::Transparent
            , w
            , h
            , fmt
            , nullptr
            , Blend_Normal
            , Alpha_Normal
            , 1.f
            , false
            , nullptr
            , FOnNameChanged()
            , FOnBoolChanged()
            , FOnBoolChanged()
            , FOnColorChanged()
            , FOnUserDataAdded()
            , FOnUserDataChanged()
            , FOnUserDataRemoved()
            , FOnParentChanged( []( const TNode< ILayer >* iNode, const TRoot< ILayer >* iParent ){
                ULIS_DEBUG_PRINTF( "Parent Changed" )
                auto dummy = 0;
            } )
        )
    );
    */

    FLayerImage& img = stack->Find< FLayerImage >( "image0" );
    {
        ctx.Clear( canvas, canvas.Rect() );
        ctx.Finish();
    }

    auto startTime = std::chrono::steady_clock::now();
    {
        ctx.Finish();
    }
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count();
    std::cout << "ULIS Layer Stack Flatten took: " << delta << "ms." << std::endl;
    QApplication    app( argc, argv );
    QWidget*        widget  = new QWidget();
    QImage*         image   = new QImage( canvas.Bits()
                                        , canvas.Width()
                                        , canvas.Height()
                                        , canvas.BytesPerScanLine()
                                        , QImage::Format_RGBA8888 );
    QPixmap         pixmap  = QPixmap::fromImage( *image );
    QLabel*         label   = new QLabel( widget );
    label->setPixmap( pixmap );
    widget->resize( pixmap.size() );
    widget->show();
    int exit_code = app.exec();
    delete  label;
    delete  image;
    delete  widget;
    delete  stack;
    return  exit_code;
}

