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
    mCtxGUI.Finish();
    mCtxCanvas->Finish();
    delete  mCtxCanvas;
}

FULISLoader::FULISLoader()
    : mPool()
    , mQueue( mPool )
    , mFmtGUI( Format_RGBA8 )
    , mFmtCanvas( Format_RGBA8 )
    , mCtxGUI( mQueue, mFmtGUI )
    , mCtxCanvas( new FContext( mQueue, mFmtCanvas ) )
    , mFontEngine()
    , mHW()
{
}

eFormat
FULISLoader::FormatGUI() const
{
    return  mFmtGUI;
}

eFormat
FULISLoader::FormatCanvas() const
{
    return  mFmtCanvas;
}

FContext&
FULISLoader::ContextGUI()
{
    return  mCtxGUI;
}

FContext&
FULISLoader::ContextCanvas()
{
    return  *mCtxCanvas;
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

void
FULISLoader::SetContextCanvasFormat( eFormat iFmt )
{
    if( mCtxCanvas->Format() == iFmt )
        return;

    mCtxCanvas->Finish();
    delete  mCtxCanvas;
    mFmtCanvas = iFmt;
    mCtxCanvas = new FContext( mQueue, mFmtCanvas );
}

