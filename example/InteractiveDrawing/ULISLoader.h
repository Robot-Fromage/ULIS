// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ULISLoader.h
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
    eFormat FormatGUI() const;
    eFormat FormatCanvas() const;
    FContext& ContextGUI();
    FContext& ContextCanvas();
    FFontEngine& FontEngine();
    const FHardwareMetrics& HardwareMetrics() const;
    void SetContextCanvasFormat( eFormat iFmt );

private:
    FThreadPool mPool;
    FCommandQueue mQueue;
    eFormat mFmtGUI;
    eFormat mFmtCanvas;
    FContext mCtxGUI;
    FContext* mCtxCanvas;
    FFontEngine mFontEngine;
    FHardwareMetrics mHW; 
};

