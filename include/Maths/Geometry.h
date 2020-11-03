// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Geometry.h
* @author       Clement Berthaud
* @brief        This file provides geometry class declarations.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// TVec2
template< typename T >
struct ULIS2_API TVec2
{
    T x;
    T y;

    TVec2() : x( 0 ), y( 0 ) {}
    TVec2( T iV ) : x( iV ), y( iV ) {};
    TVec2( T ix, T iy ) : x( ix ), y( iy ) {};
    template< typename U > TVec2( U ix, U iy ) : x( static_cast< T >( ix ) ), y( static_cast< T >( iy ) ) {};
    template< typename U > TVec2( const TVec2< U >& iOther ) : x( static_cast< T >( iOther.x ) ), y( static_cast< T >( iOther.y ) ) {};

    ULIS2_FORCEINLINE T Distance() const {                              return  sqrt( x * x + y *y ); }
    ULIS2_FORCEINLINE T DistanceSquared() const {                       return  x * x + y * y; }
    ULIS2_FORCEINLINE T ManhattanDistance() const {                     return  x + y; }
    ULIS2_FORCEINLINE T operator|( const TVec2& iOther ) const {        return  x * iOther.x + y * iOther.y; }
    ULIS2_FORCEINLINE T DotProduct( const TVec2& iOther ) {             return  ( *this | iOther ); }
    ULIS2_FORCEINLINE TVec2 operator+( const TVec2& iOther ) const {    return  TVec2( x + iOther.x, y + iOther.y ); }
    ULIS2_FORCEINLINE TVec2 operator-(const TVec2& iOther ) const {     return  TVec2( x - iOther.x, y - iOther.y ); }
    ULIS2_FORCEINLINE TVec2 operator-( T iValue ) const {               return  TVec2( x - iValue, y - iValue ); }
    ULIS2_FORCEINLINE TVec2 operator+( T iValue ) const {               return  TVec2( x + iValue, y + iValue ); }
    ULIS2_FORCEINLINE TVec2 operator*( T iValue ) const {               return  TVec2( x * iValue, y * iValue ); }
    ULIS2_FORCEINLINE TVec2 operator/( T iValue ) const {               return  TVec2( x / iValue, y / iValue ); }
    ULIS2_FORCEINLINE TVec2 operator*(const TVec2& iOther ) const {     return  TVec2( x * iOther.x, y * iOther.y ); }
    ULIS2_FORCEINLINE TVec2 operator/(const TVec2& iOther ) const {     return  TVec2( x / iOther.x, y / iOther.y ); }
    ULIS2_FORCEINLINE TVec2 operator%(const TVec2& iOther ) const {     return  TVec2( x % iOther.x, y % iOther.y ); }
    ULIS2_FORCEINLINE bool operator==(const TVec2& iOther ) const {     return  x == iOther.x && y == iOther.y; }
    ULIS2_FORCEINLINE bool operator!=(const TVec2& iOther ) const {     return  x != iOther.x || y != iOther.y; }
    ULIS2_FORCEINLINE TVec2 operator-() const {                         return  TVec2( -x, -y ); }
    ULIS2_FORCEINLINE TVec2 operator+=(const TVec2& iOther ) {          x += iOther.x; y += iOther.y; return  *this; }
    ULIS2_FORCEINLINE TVec2 operator-=(const TVec2& iOther ) {          x -= iOther.x; y -= iOther.y; return  *this; }
    ULIS2_FORCEINLINE TVec2 operator*=(T iValue) {                      x *= iValue; y *= iValue; return  *this; }
    ULIS2_FORCEINLINE TVec2 operator/=(T iValue ) {                     x /= iValue; y /= iValue; return  *this; }
    ULIS2_FORCEINLINE TVec2 operator*=(const TVec2& iOther ) {          x *= iOther.x; y *= iOther.y; return  *this; }
    ULIS2_FORCEINLINE TVec2 operator/=(const TVec2& iOther ) {          x /= iOther.x; y /= iOther.y; return  *this; }
    ULIS2_FORCEINLINE TVec2 operator%=(const TVec2& iOther ) {          x %= iOther.x; y %= iOther.y; return  *this; }
    ULIS2_FORCEINLINE T& operator[](int32 iIndex ) { ULIS2_ASSERT( iIndex >= 0 && iIndex < 3, "Bad Index" ); return  ( &x )[ iIndex ]; }
    ULIS2_FORCEINLINE T operator[]( int32 iIndex)const { ULIS2_ASSERT( iIndex >= 0 && iIndex < 3, "Bad Index" ); return  ( &x )[ iIndex ]; }
    ULIS2_FORCEINLINE void Normalize() { T dist = Distance(); x /= dist; y /= dist; }
};

/////////////////////////////////////////////////////
// FRect
struct ULIS2_API FRect
{
    int x;
    int y;
    int w;
    int h;

    FRect();
    FRect( int ix, int iy, int iW, int iH );
    static FRect FromXYWH( int ix, int iy, int iW, int iH );
    static FRect FromMinMax( int ixMin, int iyMin, int ixMax, int iyMax );

    template< class T >
    bool HitTest( const TVec2< T >& iV ) {
        return  iV.x >= static_cast< T >( x )
            &&  iV.x <  static_cast< T >( x + h )
            &&  iV.y >= static_cast< T >( y )
            &&  iV.y <  static_cast< T >( y + h );
    }

    template< class T >
    bool InVerticalRange( T iV ) {
        return  iV >= static_cast< T >( y )
            &&  iV <  static_cast< T >( y + h );
    }

    template< class T >
    bool InHorizontalRange( T iV ) {
        return  iV >= static_cast< T >( x )
            &&  iV <  static_cast< T >( x + w );
    }

    FRect operator&( const FRect& iOther ) const;
    FRect operator|( const FRect& iOther ) const;
    bool operator==( const FRect& iOther ) const;
    int Area() const;
    void TransformAffine( const FTransform2D& iTransform );
    void TransformPerspective( const FTransform2D& iTransform );
    FRect TransformedAffine( const FTransform2D& iTransform ) const;
    FRect TransformedPerspective( const FTransform2D& iTransform ) const;
    void FitInPositiveRange();
    void Shift( const FVec2I& iVec );
    FVec2I GetShift() const;
};

ULIS2_NAMESPACE_END

