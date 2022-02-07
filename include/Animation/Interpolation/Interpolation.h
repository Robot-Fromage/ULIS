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

#include "Core/Core.h"


ULIS_NAMESPACE_BEGIN

template<typename T>
struct FKey;

template< typename T >
class TInterpolation
{
protected:
    TInterpolation();
    ~TInterpolation();

public:
    virtual T Interpolate( float iFrame, const FKey<T>& iLeftKey, const FKey<T>& iRightKey ) const = 0;
};

template< typename T >
TInterpolation<T>::TInterpolation()
{

}

template< typename T >
TInterpolation<T>::~TInterpolation()
{

}

ULIS_NAMESPACE_END