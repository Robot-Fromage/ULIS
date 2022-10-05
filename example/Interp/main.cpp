// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Interp.cpp
* @author       Clement Berthaud
* @brief        Blend application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QLabel>
using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat fmt = Format_RGBA8;
    FContext ctx( queue, fmt );

    FBlock block0;
    FBlock block1;
    { // Load
        std::string path0 = "C:/Users/conta/Documents/work/Interp/0.png";
        std::string path1 = "C:/Users/conta/Documents/work/Interp/1.png";
        ulError err;
        err = ctx.XLoadBlockFromDisk( block0, path0 );
        ULIS_ASSERT( !err, "Load failed" );
        ULIS_ASSERT( block0.Format() == fmt, "Bad format assumption." );
        err = ctx.XLoadBlockFromDisk( block1, path1 );
        ULIS_ASSERT( !err, "Load failed" );
        ULIS_ASSERT( block1.Format() == fmt, "Bad format assumption." );
        ctx.Flush();
        ctx.Fence();
    }

    const FRectI roi = block0.Rect();
    FBlock* blockCanvas = new  FBlock( roi.w, roi.h, fmt );

    return  0;
}

