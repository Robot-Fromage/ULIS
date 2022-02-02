// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HoldInterpolation.h
* @author       Thomas Schmitt
* @brief        This file provides declaration for the hold interpolation
* @license      Please refer to LICENSE.md
*/
#pragma once

#include "Core/Core.h"
#include "Animation/Interpolation/Interpolation.h"

ULIS_NAMESPACE_BEGIN

template< typename T >
class THoldInterpolation : public TInterpolation< T >
{
protected:
    THoldInterpolation<T>();

    static inline THoldInterpolation<T>* Instance = nullptr;

public:
    THoldInterpolation<T>(THoldInterpolation<T> &other) = delete;
    void operator=(const THoldInterpolation<T> &) = delete;
    static THoldInterpolation<T> *GetInstance();
    static void *ReleaseInstance();

public:
    virtual T Interpolate(float iT, T iLeftValue, T iRightValue) override;
};

template< typename T >
THoldInterpolation<T>::THoldInterpolation() :
    TInterpolation<T>()
{

}

template< typename T >
THoldInterpolation<T>* THoldInterpolation<T>::GetInstance()
{
    if( Instance == nullptr )
        Instance = new THoldInterpolation<T>();
    return Instance;
}

template< typename T >
void * THoldInterpolation<T>::ReleaseInstance()
{
    if( Instance != nullptr )
    {
        delete Instance;
        Instance = nullptr;
    }
}

template< typename T >
T THoldInterpolation<T>::Interpolate(float iT, T iLeftValue, T iRightValue)
{
    return iLeftValue;
}

ULIS_NAMESPACE_END