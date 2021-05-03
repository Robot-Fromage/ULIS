// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FramelessTranslucentBlurBehindWindow.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QWidget>

class IFramelessTranslucentBlurBehindWindow
    : public QWidget
{
private:
    typedef  IFramelessTranslucentBlurBehindWindow  tSelf;
    typedef  QWidget                                tSuperClass;

public:
    virtual  ~IFramelessTranslucentBlurBehindWindow() = 0;
    IFramelessTranslucentBlurBehindWindow( QWidget* iParent = nullptr );

protected:
    int BorderWidth()  const;
    int DefaultBorderWidth()  const;
    void SetBorderWidth( int iValue );
    void ResetBorderWidth();

public:
    void EnableBlurBehind();
    void DisableBlurBehind();

private:
    void InitNativeFrameless();

private:
    void  WM_NCHITTEST_Event_Handler( int iX, int iY, long *oResult );
    bool  NCHitLeftBorder(      const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitRightBorder(     const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitTopBorder(       const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitBottomBorder(    const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitTopLeftCorner(   const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitTopRightCorner(  const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitBotRightCorner(  const  QRect&  iRect, const long iBorderWidth, long iX, long iY );
    bool  NCHitBotLeftCorner(   const  QRect&  iRect, const long iBorderWidth, long iX, long iY );

protected:
    virtual  bool  NCHitCaption( const  QRect&  iRect, const long iBorderWidth, long iX, long iY );

protected:
    virtual  bool  nativeEvent( const  QByteArray& eventType, void* message, long* result )  override;

private:
    int  mBorderWidth;
};

