// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         GeometryUtils.cpp
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#include "GeometryUtils.h"
#include <QWidget>

QRect
MapRectToGlobal( QWidget* iFrom, const  QRect&  iRect )
{
    if( !iFrom )
        return  iRect;

    return  QRect(  iFrom->mapToGlobal( iRect.topLeft() ),
                    iFrom->mapToGlobal( iRect.bottomRight() ) );
}


QRect
MapRectFromGlobal( QWidget* iTo, const  QRect&  iRect )
{
    if( !iTo )
        return  iRect;
    return  QRect(  iTo->mapFromGlobal( iRect.topLeft() ),
                    iTo->mapFromGlobal( iRect.bottomRight() ) );
}


QPoint
PointFromSize( const  QSize&  iFrom )
{
    return  QPoint( iFrom.width(), iFrom.height() );
}


QSize
SizeFromPoint( const  QPoint&  iFrom )
{
    return  QSize( iFrom.x(), iFrom.y() );
}

