// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         DockingManager.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once

#include <QObject>
#include <QRegion>
#include <QVector>
#include <functional>


// Forward Declarations
class  FTab;
class  FTabArea;

class FDockingManager :
    public  QObject
{
    Q_OBJECT

////////////////////////////////////////////////////////////////////////////////////////
////                            PRIVATE CONSTRUCTION                                ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Construction / Destruction
    virtual  ~FDockingManager();
    FDockingManager();

////////////////////////////////////////////////////////////////////////////////////////
////                            PUBLIC SINGLETON API                                ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    // Singleton Accessor
    static  FDockingManager*  DockingManager();


////////////////////////////////////////////////////////////////////////////////////////
////                                INFO API                                        ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    FTab*        CurrentDraggingTab()  const;
    FTabArea*    CurrentTargetArea()  const;
    void        SetLastLiftedFrom( FTabArea* iValue );
    FTabArea*    GetLastLiftedFrom()  const;

////////////////////////////////////////////////////////////////////////////////////////
////                            REGISTER API                                        ////
////////////////////////////////////////////////////////////////////////////////////////
public:
    // Public Register API for Tabs & TabAreas
    void  RegisterTabArea(      FTabArea* iTabArea );
    void  UnregisterTabArea(    FTabArea* iTabArea );
    void  RegisterTab(          FTab* iTab );
    void  UnregisterTab(        FTab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                            PRIVATE SIGNAL SLOTS API                            ////
////////////////////////////////////////////////////////////////////////////////////////
private slots:
    // Docking Interface Slots
    void  TabLifted( FTab* iTab );
    void  TabDropped( FTab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE API                                     ////
////////////////////////////////////////////////////////////////////////////////////////
protected:
    // Qt Events overrides
    // Intercepting Tab Events
    virtual  bool  eventFilter( QObject* obj, QEvent* event)    override;

private:
    // Private Connection Interface
    void  InitConnectionsForTab( FTab* iTab );
    void  DestroyConnectionsForTab( FTab* iTab );

////////////////////////////////////////////////////////////////////////////////////////
////                                PRIVATE DATA                                    ////
////////////////////////////////////////////////////////////////////////////////////////
private:
    // Private Data Members
    QVector< FTabArea* >             mTabAreaList;
    FTab*                            mCurrentDraggingTab;
    FTabArea*                        mCurrentTargetArea;
    FTabArea*                        mLastLiftedFrom;
};

////////////////////////////////////////////////////////////////////////////////////////
////                            PUBLIC SINGLETON API                                ////
////////////////////////////////////////////////////////////////////////////////////////
// External Conveniency Singleton Accessor
FDockingManager*  DockingManager();


