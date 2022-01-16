// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CustomCaption.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#include "CustomCaption.h"
#include <QtWinExtras> // for native event

//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FCustomCaption::~FCustomCaption()
{
}


FCustomCaption::FCustomCaption( QWidget* iParent )
    : tSuperClass( iParent )
{
}


//--------------------------------------------------------------------------------------
//----------------------------------------- Custom NC Handling implementation in caption
bool
FCustomCaption::HitEmptySpace( long iX, long iY )
{
    QPoint local = mapFromParent( QPoint( iX, iY ) );

    // Iterate over children and return false if hit children
    QList<QWidget*> list = this->findChildren<QWidget*>();
    for( QWidget *w : list) {
        if( w->parent() != this )
            continue;

        QRect g = w->geometry();
        if(g.contains( local ))
            return  false;
    }

    // No child was hit, return true.
    return  true;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
bool
FCustomCaption::nativeEvent( const  QByteArray& eventType, void* message, long* result )
{
    // Cast to WINAPI standards
    MSG*    wmsg    = reinterpret_cast< MSG* >( message );
    LPARAM  lparam  = wmsg->lParam;
    WPARAM  wparam  = wmsg->wParam;
    HWND    hwnd    = wmsg->hwnd;
    UINT    msg     = wmsg->message;

    switch( msg )
    {
        case WM_NCHITTEST:
        {
            *result = HTTRANSPARENT;
            return  true;
        }

        default:
        {
            // The case is not handled ? Treat it the normal way & return.
            return  tSuperClass::nativeEvent( eventType, message, result );
        }
    }
}


//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------- Public Qt Slots
void
FCustomCaption::ProcessCloseClicked()
{
    emit  CloseClicked();
}


void
FCustomCaption::ProcessMaximizeClicked()
{
    emit  MaximizeClicked();
}


void
FCustomCaption::ProcessMinimizeClicked()
{
    emit  MinimizeClicked();
}

