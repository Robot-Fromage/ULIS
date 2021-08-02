// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         DockingCallbackLibrary.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "DockingCallbackLibrary.h"
#include "TabArea.h"

void
OnAreaBecomesEmptyCB_CloseTopLevel( FTabArea* iArea )
{
    auto tw = iArea->topLevelWidget();
    tw->close();
    tw->deleteLater();
}


void
OnAreaBecomesEmptyCB_DoNothing( FTabArea* iArea )
{
}


void
OnTabDroppedOutCB_RevertBack( FTab* iTab, FTabArea* iSrc )
{
    iSrc->DockHere( iTab );
}


void
OnTabDroppedOutCB_Open( FTab* iTab, FTabArea* iSrc )
{
    auto  w = new  FTabArea();

    w->SetOnAreaBecomesEmptyCB( OnAreaBecomesEmptyCB_CloseTopLevel );
    w->SetOnTabDroppedOutCB( OnTabDroppedOutCB_RevertBack );

    w->setAttribute( Qt::WA_DeleteOnClose );
    w->resize( 800, 30 );
    w->move(iTab->pos());
    w->setFocus();
    w->show();
    w->raise();
    w->DockHere(iTab);

    // Now this is optional if we want to activate the window once the tab has been docked
    // the set focus method just doesn't work
    // This does activate & raise the window but doesn't grab the focus
    w->raise();  // for MacOS
    w->activateWindow(); // for Windows
}

