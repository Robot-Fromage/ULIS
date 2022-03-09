// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         main.cpp
* @author       Clement Berthaud
* @brief        TiledBlock application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include <QApplication>
#include <cstdlib>
#include <ctime>
using namespace ::ULIS;

int
main( int argc, char *argv[] ) {
    srand( time( NULL ) );

    FThreadPool pool;
    FCommandQueue queue( pool );
    eFormat format = Format_RGBA8;
    FContext ctx( queue, format );

    FTilePool tilePool( format );
    FTiledBlock blockA( tilePool );
    ctx.Fill( blockA, FColor::Red, FRectI( -128, -128, 64, 64 ) );
    ctx.Finish();
    ctx.Finish();

    //while( true ) {
    //    system("cls");
    //    tilePool.PrintDiagnosis();
    //    std::this_thread::sleep_for( std::chrono::duration< double, std::milli >( 1000 ) );
    //    blockA.SanitizeNow();
    //}
    blockA.RecomputeLeafGeometry();
    blockA.RecomputeRootGeometry();
    FRectI r0 = blockA.LeafGeometry();
    FRectI r1 = blockA.RootGeometry();
    FRectI r2 = blockA.OperativeGeometry();

    return  0;
}

