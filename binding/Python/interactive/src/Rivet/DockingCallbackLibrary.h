// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         DockingCallbackLibrary.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once

class FTab;
class FTabArea;

void  OnAreaBecomesEmptyCB_CloseTopLevel( FTabArea* iArea );
void  OnAreaBecomesEmptyCB_DoNothing( FTabArea* iArea );
void  OnTabDroppedOutCB_RevertBack( FTab* iTab, FTabArea* iSrc );
void  OnTabDroppedOutCB_Open( FTab* iTab, FTabArea* iSrc );

