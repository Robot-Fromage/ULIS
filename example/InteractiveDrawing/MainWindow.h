// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MainWindow.h
* @author       Clement Berthaud
* @brief        InteractiveDrawing application for ULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <QWidget>
class SToolBar;
class FULISLoader;

class SMainWindow
    : public QWidget
{
    Q_OBJECT

public:
    ~SMainWindow();
    SMainWindow( FULISLoader& iHandle );

private:
    FULISLoader& mHandle;
    SToolBar* mToolBar;
};

