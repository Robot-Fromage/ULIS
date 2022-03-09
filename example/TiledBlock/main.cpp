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
    ctx.Fill( blockA, FColor::Red, FRectI( 0, 0, 128, 128 ) );
    ctx.Finish();
    tilePool.PrintDiagnosis();
    system("cls");
    blockA.SanitizeNow();
    tilePool.PrintDiagnosis();
    system("cls");
    ctx.Clear( blockA, FRectI( 32, 32, 64, 64 ) );
    ctx.Finish();
    tilePool.PrintDiagnosis();
    system("cls");
    blockA.SanitizeNow();
    tilePool.PrintDiagnosis();
    system("cls");

    while( true ) {
        system("cls");
        tilePool.PrintDiagnosis();
        std::this_thread::sleep_for( std::chrono::duration< double, std::milli >( 1000 ) );
        blockA.SanitizeNow();
    }
    return  0;
}

