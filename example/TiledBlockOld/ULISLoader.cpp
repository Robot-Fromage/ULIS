// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ULISLoader.cpp
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include "ULISLoader.h"

FULISLoader::~FULISLoader() {
    mCtx.Finish();
}

FULISLoader::FULISLoader()
    : mPool()
    , mQueue( mPool )
    , mFmt( Format_RGBA8 )
    , mCtx( mQueue, mFmt )
    , mFontEngine()
    , mHW()
{
}

eFormat
FULISLoader::Format() const
{
    return  mFmt;
}

FContext&
FULISLoader::Context()
{
    return  mCtx;
}

FFontEngine&
FULISLoader::FontEngine()
{
    return  mFontEngine;
}

const FHardwareMetrics&
FULISLoader::HardwareMetrics() const
{
    return  mHW;
}

