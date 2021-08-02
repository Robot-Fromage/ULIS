// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TabArea.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QWidget>
#include "Tab.h"


// Forward declarations
class  FCustomButton;


// Qt Forward declarations
class  QGraphicsDropShadowEffect;
class  QScrollArea;
class  QScroller;
class  QTimer;
class  QStackedWidget;

typedef void (*OnAreaBecomesEmptyCB)( FTabArea* self );

class FTabArea :
    public QWidget
{
    Q_OBJECT

    typedef  QWidget  tSuperClass;

////////////////////////////////////////////////////////////////////////////////////////
////                                PUBLIC API                                      ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    // Construction / Destruction
    virtual         ~FTabArea();
                    FTabArea( QWidget *parent = Q_NULLPTR );

public:
    // Custom Hit Test Handling implementation in TabArea
    bool            HitEmptySpace( long iX, long iY );

public:
    // Tab Control Interface
    void            ManualAddNewTab(    FTab* iTab );
    void            DockHere(           FTab* iTab);
    void            SetCurrentTab(      FTab* iTab );
    int             NTabs()  const;
    void            SetCandidateTab( FTab* iTab );
    void            SetNoCandidateTab();

    void            SetLinkedStack( QStackedWidget* iStack );
    QStackedWidget* GetLinkedStack()  const;

public:
    // Tab-Specific Geometry Ordering
    void            Reorder();
    void            Recompose();

public:
    // Overlap Size Style accessors
    void            SetOverlap( int iValue );
    void            SetMaximumTabWidth( int iWidth );
    void            SetMinimumTabWidth( int iWidth );
    void            SetTabsShapeStyle( FTab::eTabShape iValue );
    void            SetTabsClosable( bool iValue );
    void            SetTabsLiftable( bool iValue );

    void            SetOnAreaBecomesEmptyCB( OnAreaBecomesEmptyCB iOnAreaBecomesEmptyCB );
    void            SetOnTabDroppedOutCB( OnTabDroppedOutCB iOnTabDroppedOutCB );

    int                 GetOverlap()            const;
    int                 GetMaximumTabWidth()    const;
    int                 GetMinimumTabWidth()    const;
    int                 GetTabWidth()           const;
    FTab::eTabShape     GetTabsShapeStyle()     const;
    bool                GetTabsClosable()       const;
    bool                GetTabsLiftable()       const;

    OnAreaBecomesEmptyCB    GetOnAreaBecomesEmptyCB()  const;
    OnTabDroppedOutCB       GetOnTabDroppedOutCB()  const;

    void  SetBlackControls();
    void  SetWhiteControls();

    void  EnableBlurEffectControls( bool iValue );

public:
    // ID tag Interface
    void             SetTag( const  QString&  iTag );
    const  QString&  GetTag()  const;


////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Private Tab Composing & Style utilities
    void            SetTabWidth( int iWidth );
    int             ComputeSmartWidthWithOverlap()  const;
    void            RecomputeCurrentTabOnImminentChange();
    void            TakeActionOnEmpty();

protected:
    // Qt Events overrides
    virtual  void   resizeEvent( QResizeEvent* event )          override;
    virtual  void   wheelEvent( QWheelEvent* event )            override;

private:
    // Private GUI Processing Functions
    void  Init();
    void  Build();
    void  Compose();
    void  Destroy();

private:
    // Internal Private Utilities on Compose
    void  ComposeScrollArea();

private:
    // Docking Manager Registering API
    void  Register();
    void  Unregister();

////////////////////////////////////////////////////////////////////////////////////////
////                              SIGNAL SLOTS API                                  ////
////////////////////////////////////////////////////////////////////////////////////////
public slots:
    // Debug Slots
    //void  _NewTab();        // Debug

private slots:
    // Dragging Slots
    void  DomesticTabLifted(     FTab* iTab );
    void  DomesticTabDropped(    FTab* iTab );
    void  ForeignTabDropped(    FTab* iTab );

    // Navigation Slots
    void  NavDelta( int iDelta, int iTimeMS );

    void  NavLeftDoubleClick();
    void  NavLeftDown();
    void  NavLeftWork();
    void  NavLeftUp();

    void  NavRightDoubleClick();
    void  NavRightDown();
    void  NavRightWork();
    void  NavRightUp();

    void  ProcessCandidateDragHover();

    // Tab Control Slots
    void  DomesticTabSelected(   FTab* iTab );
    void  CloseCurrentTab();
    void  SwitchToNextTab();
    void  CloseTab( FTab* iTab );

public slots:signals:
    void  AreaEmpty( FTabArea* iArea );
    void  CurrentTabChanged( FTab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE DATA                                    ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Private Data Members
    // Owned Tabs OBjects Data
    QVector< FTab* >            mDomesticTabs;

    // Scroll Related Widgets Data
    QScrollArea*                mScrollArea;
    QWidget*                    mScrollWidgetWrapper;
    QScroller*                  mScroller;
    FCustomButton*              mLeftButton;
    FCustomButton*              mRightButton;

    // Size data
    int mTabWidth;
    int mMaximumTabWidth;
    int mMinimumTabWidth;
    int mOverlap;

    // Style Data
    QGraphicsDropShadowEffect*  mLeftDropShadowEffect;
    QGraphicsDropShadowEffect*  mRightDropShadowEffect;
    FTab::eTabShape            mTabsShapeStyle;
    bool                        mTabsClosable;
    bool                        mTabsLiftable;

    // State Data
    FTab*                        mCurrentTab; // Active
    FTab*                        mCandidateTab; // Dragging

    // Nav Data
    QTimer*                     mNavTimerLeft;
    QTimer*                     mNavTimerRight;
    QTimer*                     mProcessCandidateTimer;

    // ID Data
    QString                     mTag;

    OnAreaBecomesEmptyCB        mOnAreaBecomesEmptyCB;
    OnTabDroppedOutCB           mOnTabDroppedOutCB;
    QStackedWidget*             mLinkedStack;
};

