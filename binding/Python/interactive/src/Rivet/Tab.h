// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Tab.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QColor>
#include <QPoint>
#include <QRect>
#include <String>
#include <QWidget>

//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Forward Declarations
class  FCustomButton;
class  FTab;
class  FTabArea;


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------- Qt Forward Declarations
class  QGraphicsDropShadowEffect;
class  QGraphicsOpacityEffect;
class  QLabel;
class  QPropertyAnimation;
class  QPushButton;


/// @brief      Callback function OnTabDroppedOutCB
/// @param      iSelf    The FTab object for which the callback is called.
/// @param      iSrc     The FTabArea where the FTab was lifted from.
/// @return     void
typedef void (*OnTabDroppedOutCB)( FTab* iSelf, FTabArea* iSrc );

/////////////////////////////////////////////////////
/// @class      FTab
/// @brief      The FTab class provides a Tab widget.
/// @details    It handles drag & drop events along with FTabArea and the Docking Manager.
class  FTab
    : public QWidget
{
    Q_OBJECT

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------------------- Typedefs
    typedef  FTab       tSelf;
    typedef  QWidget    tSuperClass;


////////////////////////////////////////////////////////////////////////////////////////
////                                PUBLIC API                                      ////
////////////////////////////////////////////////////////////////////////////////////////
public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- Public Enums
/// @enum   eTabShape
/// @brief  Enum for Custom Tab Shape
enum class eTabShape
{
    kLine,      ///< Line, The tab has a biseau line shape
    kBezier,    ///< Bezier, The tab has a smooth shape
    kRect,      ///< Rect, The tab has a rectangular shape
    kRect_Line, ///< RectLine, The tab has hybrid shape rect line
};


public:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
    virtual  ~FTab();
    FTab( QWidget* iParent = nullptr );


public:
//--------------------------------------------------------------------------------------
//---------------------------------------------------------- Docking Interface Accessors
    bool            Docked()  const;
    bool            Dragging()  const;
    const  QPoint&  DragShift()  const;
    void            FinishDrag();


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Animation Interface
    void            SetAnimatedMovement( const  QPoint& iDest );
    void            StopAnimatedMovement();
    const  QRect&   TargetGeometry()  const;


public:
//--------------------------------------------------------------------------------------
//--------------------------------------------------- Tab ColorStyle Interface Accessors
    void            SetTitle( const  QString&  iTitle );
    void            SetTabShape( eTabShape iTabStyle );
    void            SetColor( const  QColor&  iColor );
    void            SetFadeColor( const  QColor& iColor );
    QString         GetTitle()  const;
    QString         GetTitleEllided()  const;
    eTabShape       GetTabShape()  const;
    const  QColor&  GetColor()  const;
    const  QColor&  GetFadeColor()  const;


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Tab State accessors
    void            SetActive( bool iValue = true );
    bool            IsHovered()  const;
    bool            IsPressed()  const;
    bool            IsActive()  const;


public:
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------- Behaviour constraints
    void            SetLiftable( bool iValue = true );
    void            SetClosable( bool iValue = true );
    bool            IsLiftable()  const;
    bool            IsClosable()  const;


public:
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------- ID tag Interface
    void             SetTag( const  QString&  iTag );
    const  QString&  GetTag()  const;


public:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- Callback API
    void                SetOnTabDroppedOutCB( OnTabDroppedOutCB iOnTabDroppedOutCB );
    OnTabDroppedOutCB   GetOnTabDroppedOutCB()  const;


public:
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- Link widget API
    void                SetLinkWidget( QWidget* iWidget );
    QWidget*            GetLinkWidget()  const;


////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
private:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Qt Events overrides
    virtual  void   resizeEvent(        QResizeEvent*   event )     override;
    virtual  void   enterEvent(         QEvent*         event )     override;
    virtual  void   leaveEvent(         QEvent*         event )     override;
    virtual  void   mousePressEvent(    QMouseEvent*    event )     override;
    virtual  void   mouseMoveEvent(     QMouseEvent*    event )     override;
    virtual  void   mouseReleaseEvent(  QMouseEvent*    event )     override;
    virtual  void   paintEvent(         QPaintEvent*    event )     override;
    virtual  void   closeEvent(         QCloseEvent*    event )     override;


private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------- Private GUI Processing Functions
    void            Init();
    void            Build();
    void            Compose();
    void            Destroy();


private:
//--------------------------------------------------------------------------------------
//---------------------------------------------------- Internal Tab Utilities on Compose
    void            CheckTitleEllipsis();


private:
//--------------------------------------------------------------------------------------
//------------------------------------------------------ Docking Manager Registering API
    void            Register();
    void            Unregister();


////////////////////////////////////////////////////////////////////////////////////////
////                              SIGNAL SLOTS API                                  ////
////////////////////////////////////////////////////////////////////////////////////////
public slots:
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------- Tab Slots
    void  ProcessCloseClicked();

signals:
//--------------------------------------------------------------------------------------
//------------------------------------------------------------ Docking Interface Signals
    void  Lifted( FTab* );
    void  Dropped( FTab* );
    void  Selected( FTab* );
    void  CloseClicked( FTab* );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE DATA                                    ////
////////////////////////////////////////////////////////////////////////////////////////
private:
//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Private Data Members
    QLabel*                     mTitleLabel;
    FCustomButton*              mCloseButton;
    QWidget*                    mInvisibleBackgroundRect;
    QString                     mTitleText;
    QPoint                      mDragShift;
    bool                        mDragging;
    bool                        mDragEnabled;
    QPropertyAnimation*         mAnimation;
    QRect                       mAnimationTargetGeometry;
    eTabShape                   mTabShape;
    QColor                      mBaseColor;
    QColor                      mFadeColor;
    int                         mSlopePadding;
    QGraphicsDropShadowEffect*  mTitleDropShadowEffect;
    QGraphicsDropShadowEffect*  mGlobalDropShadowEffect;
    QColor                      mTextColor;
    QColor                      mTitleDropShadowColor;
    int                         mDropShadowShift;
    QGraphicsOpacityEffect*     mOpacityEffect;
    bool                        mLiftable;
    bool                        mClosable;
    QString                     mTag;
    bool                        mHovered;
    bool                        mPressed;
    bool                        mActive;
    OnTabDroppedOutCB           mOnTabDroppedOutCB;
    QWidget*                    mLinkWidget;

};

