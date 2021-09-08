// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ClearFillCopy.cpp
* @author       Clement Berthaud
* @brief        ClearFillCopy application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
    FContext ctx( queue, fmt, PerformanceIntent_AVX );
    int blockSize = 1024;
    int tileSize = 64;
    int numTiles = 1;
    FBlock* blockA = new FBlock( blockSize, blockSize, fmt );
    FBlock* blockB = new FBlock( blockSize, blockSize, fmt );

    auto startTime = std::chrono::steady_clock::now();
    int numRepeat = 2000;
    for( int x = 0; x < numRepeat; ++x ) {
        for( int i = 0; i < numTiles; ++i ) {
            for( int j = 0; j < numTiles; ++j ) {
                FRectI rect( i * tileSize, j * tileSize, tileSize, tileSize );
                ctx.Copy( *blockA, *blockB, rect, rect.Position(), FSchedulePolicy::MonoScanlines );
                ctx.Flush();
            }
        }
        ctx.Finish();
    }
    auto endTime = std::chrono::steady_clock::now();
    auto delta   = std::chrono::duration_cast< std::chrono::milliseconds >( endTime - startTime ).count() / float( numRepeat );
    std::cout << delta << "ms." << std::endl;

    delete  blockA;
    delete  blockB;
    return  0;
}

