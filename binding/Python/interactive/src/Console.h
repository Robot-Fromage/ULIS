// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Console.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QTextEdit>

class SConsole : public QTextEdit
{
    Q_OBJECT

public:
    ~SConsole();
    SConsole( QWidget* iParent = nullptr );
    void LogCPP( const QString& iStr );
    void LogPy( const QString& iStr );
    void Log( const QString& iStr );
    void ScrollBottom();

protected:
    void keyPressEvent( QKeyEvent* e ) override;
};

