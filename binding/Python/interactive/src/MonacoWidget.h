// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MonacoWidget.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma  once

class  QWebEngineView;
class  QWebEnginePage;
#include <QWidget>

#include  "ASyncJS_PPFramework.h"


void SetMonacoPrefix( const  QString&  iPrefix );
QString GetMonacoPrefix();
QString GetFullMonacoPath();


class  MonacoWidget :
    public  QWidget
{
    Q_OBJECT

    typedef  QWidget  tSuperClass;

public:
    // Construction / Destruction
    MonacoWidget( QWidget* iParent = NULL );
    virtual  ~MonacoWidget();

public slots:
    // Editor API Exec
    Q_SLOT void     Layout(); // Recompute size of the editor in embedded page
    Q_SLOT QString  GetText();
    Q_SLOT void     SetText( const  QString&  iStr );
    Q_SLOT void     ClearText();
    Q_SLOT void     AppendText( const  QString&  iStr );

    Q_SLOT QString  GetLanguage();
    Q_SLOT void     SetLanguage( const  QString&  iStr );


protected:
    // Protected Qt events override
    virtual  void  resizeEvent( QResizeEvent*  event )  override;

private:
    // Private Internal GUI Utilitites
    void  Init();
    void  Build();
    void  Compose();
    void  Destroy();

private:
    // Private Data Members
    QWebEngineView* mEmbeddedBrowser;

private:
    // Async JS Framework
    ASyncJS_Decl

};

