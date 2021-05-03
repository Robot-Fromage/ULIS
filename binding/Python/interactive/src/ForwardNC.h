// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ForwardNC.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <windows.h>

static void DebugForward( char* iStr )
{
    auto dummy = 0;
}

#define  ForwardNC_Def virtual  bool  nativeEvent( const  QByteArray& eventType, void* message, long* result )  override;
#define  ForwardNC_Imp( iClass )                                                            \
    bool                                                                                    \
    iClass::nativeEvent( const  QByteArray&  eventType, void* message, long* result )       \
    {                                                                                       \
        MSG*    wmsg    = reinterpret_cast< MSG* >( message );                              \
        LPARAM  lparam  = wmsg->lParam;                                                     \
        WPARAM  wparam  = wmsg->wParam;                                                     \
        HWND    hwnd    = wmsg->hwnd;                                                       \
        UINT    msg     = wmsg->message;                                                    \
                                                                                            \
        switch( msg )                                                                       \
        {                                                                                   \
            case WM_NCHITTEST:                                                              \
            {                                                                               \
                DebugForward( #iClass );                                                    \
                *result = HTTRANSPARENT;                                                    \
                return  true;                                                               \
            }                                                                               \
                                                                                            \
            default:                                                                        \
            {                                                                               \
                return  QWidget::nativeEvent( eventType, message, result );                 \
            }                                                                               \
        }                                                                                   \
    }

