// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ClearFillCopy.cpp
* @author       Clement Berthaud
* @brief        ClearFillCopy application for ULIS.
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
    eFormat fmt = Format_BGRA8;
    FContext ctx( queue, fmt );

    ::ULIS::FBlock* block1 = new ::ULIS::FBlock( 1, 1, ::ULIS::Format_RGBA8 );
    ::ULIS::FBlock* block2 = new ::ULIS::FBlock( 1024, 1024, ::ULIS::Format_BGRA8 );
    block1->ReallocInternalData( block2->Width(), block2->Height(), block2->Format() );

    ctx.Copy( *block2, *block1 );
    ctx.Finish();
    return  0;
}

