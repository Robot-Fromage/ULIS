// IDDN.FR.001.250001.005.S.P.2019.000.00000
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
    {
        std::string path0 = "C:\Users\conta\Documents\work\Interp/0.png";
        std::string path1 = "C:\Users\conta\Documents\work\Interp/0.png";
        // Load from file into blocks
        ulError err;
        err = ctx.XLoadBlockFromDisk( blockBase, pathBase );
        ULIS_ASSERT( !err, "Load failed" );
        err = ctx.XLoadBlockFromDisk( blockOver, pathOver );
        ULIS_ASSERT( !err, "Load failed" );
        ULIS_ASSERT( blockBase.Format() == fmt, "Bad format assumption." );
        ULIS_ASSERT( blockOver.Format() == fmt, "Bad format assumption." );

        // Flush all commands
        ctx.Flush();

        // Wait for completion
        ctx.Fence();

        // Flush() + Fence() is equivalent to Finish()
    }

    FRectI srcRect = blockBase.Rect();
    int w = srcRect.w * 8;
    int h = srcRect.h * 5;

    FBlock* blockCanvas = new  FBlock( w, h, fmt );

    // Return exit code.
    return  exit_code;
}

