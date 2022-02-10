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

#include "Core/Core.h"
#include "Animation/Interpolation/Interpolation.h"

ULIS_NAMESPACE_BEGIN

template< typename T >
class TLinearInterpolation : public TInterpolation< T >
{
protected:
    TLinearInterpolation<T>();

    static inline TLinearInterpolation<T>* Instance = nullptr;

public:
    TLinearInterpolation<T>(TLinearInterpolation<T> &other) = delete;
    void operator=(const TLinearInterpolation<T> &) = delete;
    static TLinearInterpolation<T> *GetInstance();
    static void *ReleaseInstance();

public:
    virtual T Interpolate(float iFrame, const FKey<T>& iLeftKey, const FKey<T>& iRightKey) const override;
};

template< typename T >
TLinearInterpolation<T>::TLinearInterpolation() :
TInterpolation<T>()
{

}

template< typename T >
TLinearInterpolation<T>* TLinearInterpolation<T>::GetInstance()
{
    if( Instance == nullptr )
        Instance = new TLinearInterpolation<T>();
    return Instance;
}

template< typename T >
void * TLinearInterpolation<T>::ReleaseInstance()
{
    if( Instance != nullptr )
    {
        delete Instance;
        Instance = nullptr;
    }
}

template< typename T >
T TLinearInterpolation<T>::Interpolate(float iFrame, const FKey<T>& iLeftKey, const FKey<T>& iRightKey) const
{
    float t = (iFrame - iLeftKey.Frame) / iRightKey.Frame - iLeftKey.Frame;

    return iLeftKey.Value + t * (iRightKey.Value - iLeftKey.Value);
}

ULIS_NAMESPACE_END

