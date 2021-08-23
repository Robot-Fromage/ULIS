// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Code.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QPlainTextEdit>
class FPythonSyntaxHighlighter;

class SCode : public QPlainTextEdit
{
    Q_OBJECT

public:
    ~SCode();
    SCode( QWidget* iParent = nullptr );

protected:
    void keyPressEvent( QKeyEvent* e ) override;

private:
    FPythonSyntaxHighlighter* mHighlighter;
};

