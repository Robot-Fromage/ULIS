// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CustomButton.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QObject>
#include <QPushButton>

class  FCustomButton
    : public  QPushButton
{
    Q_OBJECT

private:
    typedef  FCustomButton  tSelf;
    typedef  QPushButton    tSuperClass;

public:
enum  class  eButtonIconShape
{
      kNone         ///< None, No shape, this is a valid state.
    , kClose        ///< Close, Cross shape for close window button.
    , kMaximize     ///< Maximize, Square shape for maximize window button.
    , kMinimize     ///< Minimize, Minus shape for minimize window button.
    , kLeftArrow    ///< LeftArrow, Triangle shape pointing left for TabArea.
    , kRightArrow   ///< RightArrow, Triangle shape pointing right for TabArea.
};

enum  class  eButtonBackgroundShape
{
      kSquare       ///< Square, full size square or rectangle background.
    , kDisk         ///< Disk, disk or ellipse shaped background.
};

private:
enum  class  eButtonState
{
    kNone           = 0,    ///< None, The button is in idle state.
    kHovered        = 1,    ///< Hovered, The button is in hovered by the mouse.
    kPressed        = 2,    ///< Pressed, The button is in pressed but the mouse does not hover.
    kPressedHovered = 3,    ///< PressedHovered, The button is in pressed and hovered by the mouse.
};

public:
    virtual  ~FCustomButton();
    FCustomButton( QWidget* iParent = nullptr );

public:
    void    SetBackgroundColor( const  QColor&  iColor );
    void    SetHoveredBackgroundColor( const  QColor&  iColor );
    void    SetPressedBackgroundColor( const  QColor&  iColor );
    const   QColor&  GetBackgroundColor()  const;
    const   QColor&  GetHoveredBackgroundColor()  const;
    const   QColor&  GetPressedBackgroundColor()  const;
    void    SetColor( const  QColor&  iColor );
    void    SetHoveredColor( const  QColor&  iColor );
    void    SetPressedColor( const  QColor&  iColor );
    const   QColor&  GetColor()  const;
    const   QColor&  GetHoveredColor()  const;
    const   QColor&  GetPressedColor()  const;
    void    SetIconShape( const  eButtonIconShape&  iShape );
    const   eButtonIconShape&  GetShape()  const;
    void    SetBackgroundShape( const  eButtonBackgroundShape&  iShape );
    const   eButtonBackgroundShape&  GetBackgroundShape()  const;
    void    SetIconSize( int iSize );
    int     GetIconSize()  const;

public:
    bool IsHovered();
    bool IsPressed();


public slots:signals:
    void  DoubleClicked();

protected:
    virtual  void  enterEvent(              QEvent*         event )  override;
    virtual  void  leaveEvent(              QEvent*         event )  override;
    virtual  void  mousePressEvent(         QMouseEvent*    event )  override;
    virtual  void  mouseReleaseEvent(       QMouseEvent*    event )  override;
    virtual  void  mouseDoubleClickEvent(   QMouseEvent*    event )  override;
    virtual  void  paintEvent(              QPaintEvent*    event )  override;

private:
    bool                    mHovered;               ///< Boolean flag, wether the button is hovered or not.
    bool                    mPressed;               ///< Boolean flag, wether the button is pressed or not.
    QColor                  mBgColor;               ///< The idle background color.
    QColor                  mHoveredBgColor;        ///< The hovered background color.
    QColor                  mPressedBgColor;        ///< The pressed background color.
    QColor                  mPressedHoveredBgColor; ///< The pressed-hovered background color.
    QColor                  mColor;                 ///< The idle color.
    QColor                  mHoveredColor;          ///< The hovered color.
    QColor                  mPressedColor;          ///< The pressed color.
    QColor                  mPressedHoveredColor;   ///< The pressed-hovered color.
    eButtonIconShape        mIconShape;             ///< The icon shape.
    eButtonBackgroundShape  mBackgroundShape;       ///< The background shape.
    int                     mIconSize;              ///< The icon size.
};

