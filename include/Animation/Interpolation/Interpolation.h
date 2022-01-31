// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Interpolation.h
* @author       Thomas Schmitt
* @brief        This file provides declaration for the basics of interpolation between animation keys
* @license      Please refer to LICENSE.md
*/
#pragma once

template< typename Scalar >
class TInterpolation
{
protected:
    TInterpolation();
    ~TInterpolation();

public:
    virtual Scalar Interpolate( float iT, Scalar iLeftValue, Scalar iRightValue ) = 0;
};

template< typename Scalar >
TInterpolation<Scalar>::TInterpolation()
{

}

template< typename Scalar >
TInterpolation<Scalar>::~TInterpolation()
{

}
