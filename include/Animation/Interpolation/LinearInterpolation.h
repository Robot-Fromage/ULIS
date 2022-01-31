// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LinearInterpolation.h
* @author       Thomas Schmitt
* @brief        This file provides declaration for the linear interpolation
* @license      Please refer to LICENSE.md
*/
#pragma once

#include "Animation/Interpolation/Interpolation.h"

template< typename Scalar >
class TLinearInterpolation : public TInterpolation< Scalar >
{
protected:
    TLinearInterpolation<Scalar>();

    static inline TLinearInterpolation<Scalar>* Instance = nullptr;

public:
    TLinearInterpolation<Scalar>(TLinearInterpolation<Scalar> &other) = delete;
    void operator=(const TLinearInterpolation<Scalar> &) = delete;
    static TLinearInterpolation<Scalar> *GetInstance();
    static void *ReleaseInstance();

public:
    virtual Scalar Interpolate(float iT, Scalar iLeftValue, Scalar iRightValue) override;
};

template< typename Scalar >
TLinearInterpolation<Scalar>::TLinearInterpolation() :
TInterpolation<Scalar>()
{

}

template< typename Scalar >
TLinearInterpolation<Scalar>* TLinearInterpolation<Scalar>::GetInstance()
{
    if( Instance == nullptr )
        Instance = new TLinearInterpolation<Scalar>();
    return Instance;
}

template< typename Scalar >
void * TLinearInterpolation<Scalar>::ReleaseInstance()
{
    if( Instance != nullptr )
    {
        delete Instance;
        Instance = nullptr;
    }
}

template< typename Scalar >
Scalar TLinearInterpolation<Scalar>::Interpolate(float iT, Scalar iLeftValue, Scalar iRightValue)
{
    return iLeftValue + iT * (iRightValue - iLeftValue);
}
