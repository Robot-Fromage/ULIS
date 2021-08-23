// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CustomMainWindowBase.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "FramelessTranslucentBlurBehindWindow.h"

class FCustomMainWindowBase
    : public IFramelessTranslucentBlurBehindWindow
{
private:
    typedef  FCustomMainWindowBase                  tSelf;
    typedef  IFramelessTranslucentBlurBehindWindow  tSuperClass;


public:
    virtual  ~FCustomMainWindowBase();
    FCustomMainWindowBase( QWidget* iParent = nullptr );

public:
    void SetCaptionHeight( int iValue );
    int GetCaptionHeight()  const;
    QRect CaptionGeometry()  const;
    QRect ContentsGeometry()  const;

public:
    void Restore();
    bool CheckCustomWindowMaximizedState()  const;
    bool CheckCustomWindowResizingState()  const;

protected:
    virtual  bool  NCHitCaption( const  QRect&  iRect, const  long iBorderWidth, long iX, long iY )  override;

protected:
    virtual  void  resizeEvent( QResizeEvent*   event )  override;
    virtual  void  moveEvent(   QMoveEvent*     event )  override;
    virtual  void  changeEvent( QEvent*         event )  override;

private:
    void  FixWindowOverlapOnMove();
    void  FixWindowOverlapOnResize( const  QSize&  iOldSize );
    void  AdjustWindowOnMaximize();
    void  AdjustWindowOnRestore();

private:
    int         mCaptionHeight;             ///< The caption Height.
    QMargins    mPaddings;                  ///< The internal content paddings
    QRect       mRestoreGeometry;           ///< The backup restore geometry
    bool        mMaximized;                 ///< The maximized state stored as a boolean flag
    QPoint      mLatestOldPos;              ///< The latest safe position
    QSize       mLatestOldSize;             ///< The latest safe sizee
    bool        mIsResizing;                ///< The resizing state stored as a boolean flag
    QMargins    mOverrideContentsMargins;   ///< The contents margins used to control the OS borders

};

