// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainWindow.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Rivet/CustomMainWindow.h"

class SCaption;
class FPythonSyntaxHighlighter;
class SCode;
class SConsole;
class SHardwareMetrics;
class SViewport;

class SMainWindow
    : public FCustomMainWindow
{
    Q_OBJECT

public:
    ~SMainWindow() override;
    SMainWindow();

private:
    QWidget* BuildTabSection( const QString& iName, QWidget* iWidget );

private:
    SCaption* mCaption;
    QWidget* mCenter;
    SCode* mCode;
    SConsole* mConsole;
    SHardwareMetrics* mMetrics;
    SViewport* mViewport;
    FPythonSyntaxHighlighter* mHighlighter;
};

