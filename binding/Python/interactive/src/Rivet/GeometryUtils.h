// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         GeometryUtils.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include <QRect>
#include <QSize>
#include <QPoint>

class  QWidget;

QRect   MapRectToGlobal( QWidget* iFrom, const  QRect&  iRect );
QRect   MapRectFromGlobal( QWidget* iTo, const  QRect&  iRect );
QPoint  PointFromSize( const  QSize&  iFrom );
QSize   SizeFromPoint( const  QPoint&  iFrom );

