// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ULISLoader.cpp
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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

