// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Maths.Geometry.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */
#pragma once
#include "ULIS/Maths/ULIS.Maths.Geometry.h"
#include <algorithm>

namespace ULIS {
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
    int x1 = FMath::Max( x, iOther.x );
    int y1 = FMath::Max( y, iOther.y );
    int x2 = FMath::Min( x + w, iOther.x + iOther.w );
    int y2 = FMath::Min( y + h, iOther.y + iOther.h );
    return  FromMinMax( x1, y1, x2, y2 );
}


FRect
FRect::operator|( const FRect& iOther ) const
{
    int x1 = FMath::Min( x, iOther.x );
    int y1 = FMath::Min( y, iOther.y );
    int x2 = FMath::Max( x + w, iOther.x + iOther.w );
    int y2 = FMath::Max( y + h, iOther.y + iOther.h );
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


/////////////////////////////////////////////////////
// FPoint
FPoint::FPoint()
    : x( 0 )
    , y( 0 )
{}


FPoint::FPoint( int iX, int iY )
    : x( iX )
    , y( iY )
{}


FPoint
FPoint::RotateAround( FPoint pivotPoint, double radianRotation)
{
    //Trigonometric direction
    double sin = -std::sin(radianRotation);
    double cos = -std::cos(radianRotation);
        
    x -= pivotPoint.x;
    y -= pivotPoint.y;
        
    return FPoint( ( x * cos - y * sin ) + pivotPoint.x, ( x * sin + y * cos ) + pivotPoint.y );
}


FPoint
FPoint::AxialSymmetry( float a, float b )
{
    float d = ( x + ( y - b ) * a ) / ( 1 + a * a );
    return FPoint( 2 * d - x,
                    2 * d * a - y + 2 * b );
}


/////////////////////////////////////////////////////
// FPoint64 bits
FPoint64::FPoint64()
    : x( 0 )
    , y( 0 )
{}


FPoint64::FPoint64( int64 iX, int64 iY )
    : x( iX )
    , y( iY )
{}


FPoint64
FPoint64::RotateAround( FPoint pivotPoint, double radianRotation)
{
    //Trigonometric direction
    double sin = -std::sin(radianRotation);
    double cos = -std::cos(radianRotation);
        
    x -= pivotPoint.x;
    y -= pivotPoint.y;
        
    return FPoint64( ( x * cos - y * sin ) + pivotPoint.x, ( x * sin + y * cos ) + pivotPoint.y );
}


FPoint64
FPoint64::AxialSymmetry( float a, float b )
{
    float d = ( x + ( y - b ) * a ) / ( 1 + a * a );
    return FPoint64( 2 * d - x,
                        2 * d * a - y + 2 * b );
}


/////////////////////////////////////////////////////
// FTransformAABB
FTransformAABB::FTransformAABB( float iX1, float iY1, float iX2, float iY2 )
    : x1( iX1 )
    , y1( iY1 )
    , x2( iX2 )
    , y2( iY2 )
{
}


FTransformAABB::FTransformAABB( const  std::vector< glm::vec2 >& iPoints )
{
    SetPoints( iPoints );
}


FTransformAABB::FTransformAABB( const  FTransformOBB& iOBB )
{
    x1 = FMath::Min( iOBB.m00.x, FMath::Min3( iOBB.m01.x, iOBB.m11.x, iOBB.m10.x ) );
    y1 = FMath::Min( iOBB.m00.y, FMath::Min3( iOBB.m01.y, iOBB.m11.y, iOBB.m10.y ) );
    x2 = FMath::Max( iOBB.m00.x, FMath::Max3( iOBB.m01.x, iOBB.m11.x, iOBB.m10.x ) );
    y2 = FMath::Max( iOBB.m00.y, FMath::Max3( iOBB.m01.y, iOBB.m11.y, iOBB.m10.y ) );
}


void
FTransformAABB::GetPoints( std::vector< glm::vec2 >* oPoints ) const
{
    oPoints->clear();
    oPoints->push_back( glm::vec2( x1, y1 ) );
    oPoints->push_back( glm::vec2( x2, y1 ) );
    oPoints->push_back( glm::vec2( x2, y2 ) );
    oPoints->push_back( glm::vec2( x1, y2 ) );
}

void
FTransformAABB::GetPoints( std::vector< glm::vec3 >* opoints ) const
{
    // For glm mat operations and translation operations, points have to be vec3
    // because we multiply with mat3
    // Z is always 1, this is relevant and very important
    // otherwise translation have no effect
    // Pretty much like homogeneous coordinates
    opoints->clear();
    opoints->push_back( glm::vec3( x1, y1, 1.f ) );
    opoints->push_back( glm::vec3( x2, y1, 1.f ) );
    opoints->push_back( glm::vec3( x2, y2, 1.f ) );
    opoints->push_back( glm::vec3( x1, y2, 1.f ) );
}


void
FTransformAABB::SetPoints( const std::vector< glm::vec2 >& points )
{
    assert( points.size() == 4 );
    x1 = FMath::Min( points[0].x, FMath::Min3( points[1].x, points[2].x, points[3].x ) );
    y1 = FMath::Min( points[0].y, FMath::Min3( points[1].y, points[2].y, points[3].y ) );
    x2 = FMath::Max( points[0].x, FMath::Max3( points[1].x, points[2].x, points[3].x ) );
    y2 = FMath::Max( points[0].y, FMath::Max3( points[1].y, points[2].y, points[3].y ) );
}


void
FTransformAABB::SetPoints( const std::vector< glm::vec3 >& points )
{
    assert( points.size() == 4 );
    x1 = FMath::RoundAwayFromZero( FMath::Min( points[0].x, FMath::Min3( points[1].x, points[2].x, points[3].x ) ) );
    y1 = FMath::RoundAwayFromZero( FMath::Min( points[0].y, FMath::Min3( points[1].y, points[2].y, points[3].y ) ) );
    x2 = FMath::RoundAwayFromZero( FMath::Max( points[0].x, FMath::Max3( points[1].x, points[2].x, points[3].x ) ) );
    y2 = FMath::RoundAwayFromZero( FMath::Max( points[0].y, FMath::Max3( points[1].y, points[2].y, points[3].y ) ) );
}


void
FTransformAABB::Transform( const glm::mat3& mat )
{
    std::vector< glm::vec3 > src_points;
    std::vector< glm::vec3 > dst_points;
    GetPoints( &src_points );
    // We compute result points by multiplicating mat with vec3s
    // It is important that mat is on the left-hand side
    // because glm mats are column major.
    dst_points.push_back( mat * src_points[0] );
    dst_points.push_back( mat * src_points[1] );
    dst_points.push_back( mat * src_points[2] );
    dst_points.push_back( mat * src_points[3] );
    SetPoints( dst_points );
}

void
FTransformAABB::Shift( const glm::vec2& iVec )
{
    x1 = 0;
    y1 = 0;
    x2 += iVec.x;
    y2 += iVec.y;
}


glm::vec2
FTransformAABB::GetShift()
{
    return  glm::vec2( -x1, -y1 );
}


float
FTransformAABB::Width() const
{
    return x2 - x1;
}


float
FTransformAABB::Height() const
{
    return y2 - y1;
}


void
FTransformAABB::FitInPositiveRange()
{
    x2 -= x1;
    y2 -= y1;
    x1 = 0;
    y1 = 0;
}


/////////////////////////////////////////////////////
// Sort
bool
SortCompareLesserX( const glm::vec2& iA, const glm::vec2& iB )
{
    return  iA.x < iB.x;
}


bool
SortCompareLesserY( const glm::vec2& iA, const glm::vec2& iB )
{
    return  iA.y < iB.y;
}


/////////////////////////////////////////////////////
// FTransformOBB
// A oriented rectangle used to preview the transformation bounding box result
// Used for precomputation and allocation before actual transforms
FTransformOBB::FTransformOBB( const glm::vec2& iM00
                            , const glm::vec2& iM10
                            , const glm::vec2& iM11
                            , const glm::vec2& iM01 )
    : m00( iM00 )
    , m10( iM10 )
    , m11( iM11 )
    , m01( iM01 )
{
}

FTransformOBB::FTransformOBB( float iX1, float iY1, float iX2, float iY2 )
    : m00( iX1, iY1 )
    , m10( iX2, iY1 )
    , m11( iX2, iY2 )
    , m01( iX1, iY2 )
{
}


FTransformOBB::FTransformOBB( const FTransformAABB& iAABB )
    : m00( iAABB.x1, iAABB.y1 )
    , m10( iAABB.x2, iAABB.y1 )
    , m11( iAABB.x2, iAABB.y2 )
    , m01( iAABB.x1, iAABB.y2 )
{
}


void
FTransformOBB::Transform( const glm::mat3& mat )
{
    m00 = mat * glm::vec3( m00, 1.f );
    m10 = mat * glm::vec3( m10, 1.f );
    m11 = mat * glm::vec3( m11, 1.f );
    m01 = mat * glm::vec3( m01, 1.f );
}

void
FTransformOBB::Shift( const glm::vec2& iVec )
{
    m00 += iVec;
    m10 += iVec;
    m11 += iVec;
    m01 += iVec;
}


void
FTransformOBB::GetPoints( std::vector< glm::vec2 >* oPoints ) const
{
    oPoints->clear();
    oPoints->push_back( m00 );
    oPoints->push_back( m10 );
    oPoints->push_back( m11 );
    oPoints->push_back( m01 );
}

FOBBHullExpression
FTransformOBB::BuildHullExpression() const
{
    std::vector< glm::vec2 > points;
    std::vector< glm::vec2 > pointsLeft;
    std::vector< glm::vec2 > pointsRight;
    points.reserve( 4 );
    pointsLeft.reserve( 3 );
    pointsRight.reserve( 3 );
    GetPoints( &points );
    std::sort( points.begin(), points.end(), SortCompareLesserX );
    pointsLeft.push_back( points[0] );
    pointsLeft.push_back( points[1] );
    pointsRight.push_back( points[2] );
    pointsRight.push_back( points[3] );
    std::sort( pointsLeft.begin(), pointsLeft.end(), SortCompareLesserY );
    std::sort( pointsRight.begin(), pointsRight.end(), SortCompareLesserY );
    if( pointsLeft[0].y > pointsRight[0].y ) pointsLeft.push_back( pointsRight[0] );
    if( pointsLeft[1].y < pointsRight[1].y ) pointsLeft.push_back( pointsRight[1] );
    if( pointsRight[0].y > pointsLeft[0].y ) pointsRight.push_back( pointsLeft[0] );
    if( pointsRight[1].y < pointsLeft[1].y ) pointsRight.push_back( pointsLeft[1] );
    std::sort( pointsLeft.begin(), pointsLeft.end(), SortCompareLesserY );
    std::sort( pointsRight.begin(), pointsRight.end(), SortCompareLesserY );
    assert( pointsLeft.size() >= 2 );
    assert( pointsRight.size() >= 2 );

    FOBBHullExpression hull;
    for( int i = 0; i < pointsLeft.size() - 1; ++i )
        hull.left.lines.push_back( FLinef::FromPointsVertical( pointsLeft[i+1], pointsLeft[i] ) );

    for( int i = 0; i < pointsRight.size() - 1; ++i )
        hull.right.lines.push_back( FLinef::FromPointsVertical( pointsRight[i+1], pointsRight[i] ) );

    hull.left.indexer = pointsLeft[1].y;
    hull.right.indexer = pointsRight[1].y;

    return  hull;
}


/////////////////////////////////////////////////////
// FLinef
FLinef::FLinef( float iA, float iB )
    : a( iA )
    , b( iB )
{
}


//static
FLinef
FLinef::FromPointsVertical( const glm::vec2& iPointA, const glm::vec2& iPointB )
{
    float dx = ( iPointB.x - iPointA.x );
    float slope, add;
    if( dx == 0 )
    {
        slope = 0;
        add = iPointA.x;
    }
    else
    {
        float dy = ( iPointB.y - iPointA.y );
        slope = dx / dy;
        add = iPointA.x - ( slope * iPointA.y );
    }

    return  FLinef( slope, add );
}


float
FLinef::Eval( float iX ) const
{
    return  a * iX + b;
}


/////////////////////////////////////////////////////
// FOBBSlopeExpression
FOBBSlopeExpression::FOBBSlopeExpression()
    : lines()
    , indexer( 0 )
{
}


float
FOBBSlopeExpression::Eval( int iX ) const
{
    int index = indexer == 0 ? 0 : FMath::Min( iX / indexer, 1 );
    return  lines[ index ].Eval( iX );
}



/////////////////////////////////////////////////////
// FOBBHullExpression
FOBBHullExpression::FOBBHullExpression()
    : left()
    , right()
{
}

FOBBHullExpression::FOBBHullExpression( const FOBBSlopeExpression& iLeft, const FOBBSlopeExpression& iRight )
    : left( iLeft )
    , right( iRight )
{
}


} // namespace ULIS

