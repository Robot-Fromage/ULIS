// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ULISLoader.h
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <ULIS.h>
using namespace ::ULIS;

class FULISLoader
{
public:
    ~FULISLoader();
    FULISLoader();

public:
    eFormat Format() const;
    FContext& Context();
    FFontEngine& FontEngine();
    const FHardwareMetrics& HardwareMetrics() const;

private:
    FThreadPool mPool;
    FCommandQueue mQueue;
    eFormat mFmt;
    FContext mCtx;
    FFontEngine mFontEngine;
    FHardwareMetrics mHW;
};

