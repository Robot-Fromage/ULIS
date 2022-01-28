// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ASyncJS_PPFramework.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma  once
#include  <QEventLoop>
#include  <QVariant>

#define ASyncJS_RFlag_Name                      ASyncJS_ReadyFlag
#define ASyncJS_VRet_Name                       ASyncJS_VRet
#define ASyncJS_SetReadySlot_Name               ASyncJS_SetReady
#define ASyncJS_UnlockFct_Name                  ASyncJS_Unlock_fct
#define ASyncJS_UnlockSignal_Name               ASyncJS_Unlock_sig
#define ASyncJS_Decl_RFlag                      const  bool  ASyncJS_RFlag_Name = false;
#define ASyncJS_Decl_VRet                       QVariant  ASyncJS_VRet_Name;
#define ASyncJS_Decl_SetReadySlot               void  ASyncJS_SetReadySlot_Name( bool );
#define ASyncJS_Decl_UnlockFct                  void  ASyncJS_UnlockFct_Name();
#define ASyncJS_Decl_UnlockSignal               void  ASyncJS_UnlockSignal_Name();
#define ASyncJS_Set_RFlag                       bool* t = const_cast< bool* >( &ASyncJS_RFlag_Name ); *t = true;
#define ASyncJS_Impl_SetReadySlot( iClass )     void  iClass::ASyncJS_SetReadySlot_Name( bool ) { ASyncJS_Set_RFlag; };
#define ASyncJS_Impl_UnlockFct( iClass )        void  iClass::ASyncJS_UnlockFct_Name() { emit  ASyncJS_UnlockSignal_Name(); }
#define ASyncJS_IsReady                         ASyncJS_RFlag_Name
#define ASyncJS_Decl                            private: ASyncJS_Decl_UnlockFct                         \
                                                        private: ASyncJS_Decl_RFlag ASyncJS_Decl_VRet   \
                                                        Q_SLOT ASyncJS_Decl_SetReadySlot                \
                                                        Q_SIGNAL ASyncJS_Decl_UnlockSignal

#define  ASyncJS_Impl( iClass )                 ASyncJS_Impl_SetReadySlot( iClass ) ASyncJS_Impl_UnlockFct( iClass )
#define  ASyncJS_Run( iContext, iCmd, iResult )                                                                             \
    QEventLoop loop;                                                                                                        \
    QObject::connect( this, SIGNAL( ASyncJS_Unlock_sig() ), &loop, SLOT( quit() ) );                                        \
    iContext->runJavaScript( iCmd, [this]( const  QVariant&  v ) { ASyncJS_VRet_Name =  v; ASyncJS_UnlockFct_Name(); } );   \
    loop.exec();                                                                                                            \
    iResult = ASyncJS_VRet_Name;

#define  ASyncJS_ReadyCond( iObj, iSignal ) \
    QObject::connect( iObj, SIGNAL( iSignal( bool ) ), this, SLOT( ASyncJS_SetReady_slot( bool ) ) );

