// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Geometry.cpp
* @author       Clement Berthaud
* @brief        This file provides geometry class definitions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Maths/Transform2D.h"
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FRect
FRect::FRect()
    : x( 0 )
    , y( 0 )
    , w( 0 )
    , h( 0 )
{}


FRect::FRect( int iX, int iY, int iW, int iH )
    : x( iX )
    , y( iY )
    , w( iW )
    , h( iH )
{}

//static
FRect
FRect::FromXYWH( int iX, int iY, int iW, int iH )
{
    return  FRect( iX, iY, iW, iH );
}


//static
FRect
FRect::FromMinMax( int iXMin, int iYMin, int iXMax, int iYMax )
{
    return  FRect( iXMin, iYMin, iXMax - iXMin, iYMax - iYMin );
}


FRect
FRect::operator&( const FRect& iOther ) const
{
    int x1 = FMaths::Max( x, iOther.x );
    int y1 = FMaths::Max( y, iOther.y );
    int x2 = FMaths::Min( x + w, iOther.x + iOther.w );
    int y2 = FMaths::Min( y + h, iOther.y + iOther.h );
    return  FromMinMax( x1, y1, x2, y2 );
}


FRect
FRect::operator|( const FRect& iOther ) const
{
    int x1 = FMaths::Min( x, iOther.x );
    int y1 = FMaths::Min( y, iOther.y );
    int x2 = FMaths::Max( x + w, iOther.x + iOther.w );
    int y2 = FMaths::Max( y + h, iOther.y + iOther.h );
    return  FromMinMax( x1, y1, x2, y2 );
}
    
bool
FRect::operator==( const FRect& iOther ) const
{
    return  ( x == iOther.x && y == iOther.y && w == iOther.w && h == iOther.h );
}


int
FRect::Area() const
{
    return  w * h;
}


void
FRect::TransformAffine( const FTransform2D& iTransform ) {
    float src_x2 = static_cast< float >( x + w );
    float src_y2 = static_cast< float >( y + h );
    const glm::mat3& mat = iTransform.Matrix();
    glm::vec3 m00 = mat * glm::vec3( x, y, 1 );
    glm::vec3 m10 = mat * glm::vec3( src_x2, y, 1 );
    glm::vec3 m11 = mat * glm::vec3( src_x2, src_y2, 1 );
    glm::vec3 m01 = mat * glm::vec3( x, src_y2, 1 );
    x = static_cast< int >( FMaths::RoundToNegativeInfinity( FMaths::Min4( m00.x, m10.x, m11.x, m01.x ) ) );
    y = static_cast< int >( FMaths::RoundToNegativeInfinity( FMaths::Min4( m00.y, m10.y, m11.y, m01.y ) ) );
    w = static_cast< int >( FMaths::RoundToPositiveInfinity( FMaths::Max4( m00.x, m10.x, m11.x, m01.x ) ) ) - x;
    h = static_cast< int >( FMaths::RoundToPositiveInfinity( FMaths::Max4( m00.y, m10.y, m11.y, m01.y ) ) ) - y;
}

void
FRect::TransformPerspective( const FTransform2D& iTransform ) {
    float x1 = static_cast< float >( x );
    float y1 = static_cast< float >( y );
    float x2 = static_cast< float >( x + w );
    float y2 = static_cast< float >( y + h );
    const glm::mat3& mat = iTransform.Matrix();
    FVec2F A = HomographyTransform( FVec2F( x1, y1 ),   mat );
    FVec2F B = HomographyTransform( FVec2F( x2, y1 ),   mat );
    FVec2F C = HomographyTransform( FVec2F( x2, y2 ),   mat );
    FVec2F D = HomographyTransform( FVec2F( x1, y2 ),   mat );
    x = static_cast< int >( FMaths::RoundToNegativeInfinity( FMaths::Min4( A.x, B.x, C.x, D.x ) ) );
    y = static_cast< int >( FMaths::RoundToNegativeInfinity( FMaths::Min4( A.y, B.y, C.y, D.y ) ) );
    w = static_cast< int >( FMaths::RoundToPositiveInfinity( FMaths::Max4( A.x, B.x, C.x, D.x ) ) ) - x;
    h = static_cast< int >( FMaths::RoundToPositiveInfinity( FMaths::Max4( A.y, B.y, C.y, D.y ) ) ) - y;
}


FRect
FRect::TransformedAffine( const FTransform2D& iTransform ) const {
    FRect ret = *this;
    ret.TransformAffine( iTransform );
    return  ret;
}


FRect
FRect::TransformedPerspective( const FTransform2D& iTransform ) const {
    FRect ret = *this;
    ret.TransformPerspective( iTransform );
    return  ret;
}


void
FRect::FitInPositiveRange() {
    x = 0;
    y = 0;
}


void
FRect::Shift( const FVec2I& iVec ) {
    x += iVec.x;
    y += iVec.y;
}


FVec2I
FRect::GetShift() const {
    return  FVec2I( -x, -y );
}

ULIS2_NAMESPACE_END

