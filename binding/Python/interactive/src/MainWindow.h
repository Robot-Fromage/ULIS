// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainWindow.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Rivet/CustomMainWindow.h"

class SCaption;
class QTableView;
class QTextEdit;
class QPlainTextEdit;
class FPythonSyntaxHighlighter;
class SCode;

class SMainWindow
    : public FCustomMainWindow
{
    Q_OBJECT

public:
    ~SMainWindow() override;
    SMainWindow();

private:
    SCaption* mCaption;
    QWidget* mCenter;
    SCode* mCode;
    QTextEdit* mConsole;
    QTableView* mMetrics;
    QWidget* mViewport;
    FPythonSyntaxHighlighter* mHighlighter;
};

