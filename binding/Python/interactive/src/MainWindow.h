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
class FPythonSyntaxHighlighter;
class SCode;
class SConsole;
class SHardwareMetrics;

class SMainWindow
    : public FCustomMainWindow
{
    Q_OBJECT

public:
    ~SMainWindow() override;
    SMainWindow();

private:
    QWidget* BuildTabSection( const QString& iName, QWidget* iWidget );
    QWidget* BuildCodeSection();
    QWidget* BuildConsoleSection();
    QWidget* BuildViewportSection();
    QWidget* BuildHardwareMetricsSection();

private:
    SCaption* mCaption;
    QWidget* mCenter;
    SCode* mCode;
    SConsole* mConsole;
    SHardwareMetrics* mMetrics;
    QWidget* mViewport;
    FPythonSyntaxHighlighter* mHighlighter;
};

