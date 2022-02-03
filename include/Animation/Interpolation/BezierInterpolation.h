// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BezierInterpolation.h
* @author       Thomas Schmitt
* @brief        This file provides declaration for the bezier interpolation
* @license      Please refer to LICENSE.md
*/
#pragma once

#include "Core/Core.h"
#include "Animation/Interpolation/Interpolation.h"
#include "Math/Interpolation/Bezier.h"

ULIS_NAMESPACE_BEGIN

template< typename T >
class TBezierInterpolation : public TInterpolation< T >
{
protected:
    TBezierInterpolation<T>();

    static inline TBezierInterpolation<T>* Instance = nullptr;

public:
    TBezierInterpolation<T>(TBezierInterpolation<T> &other) = delete;
    void operator=(const TBezierInterpolation<T> &) = delete;
    static TBezierInterpolation<T> *GetInstance();
    static void *ReleaseInstance();

public:
    virtual T Interpolate(float iFrame, const FKey<T>& iLeftKey, const FKey<T>& iRightKey) const override;
};

template< typename T >
TBezierInterpolation<T>::TBezierInterpolation() :
    TInterpolation<T>()
{

}

template< typename T >
TBezierInterpolation<T>* TBezierInterpolation<T>::GetInstance()
{
    if( Instance == nullptr )
        Instance = new TBezierInterpolation<T>();
    return Instance;
}

template< typename T >
void * TBezierInterpolation<T>::ReleaseInstance()
{
    if( Instance != nullptr )
    {
        delete Instance;
        Instance = nullptr;
    }
}

template< typename T >
T TBezierInterpolation<T>::Interpolate(float iFrame, const FKey<T>& iLeftKey, const FKey<T>& iRightKey) const
{
    FVec2F leftKey = FVec2F( iLeftKey.Frame, iLeftKey.Value );
    FVec2F rightKey = FVec2F( iRightKey.Frame, iRightKey.Value );

    //Can probably be faster, more correct and flexible(since we're in a template function and we return a float here instead of T) if we solve the cubic equation of the bezier for x = iFrame;
    std::vector<FSplineParametricSample> bezier;
    CubicBezierGenerateLinearLUT( &bezier, leftKey, leftKey + iLeftKey.RightTangent, rightKey + iRightKey.LeftTangent, rightKey, 0.1f );

    //Dichotomy to search for the closest point at iFrame
    int leftKeyIndex = 0;
    int rightKeyIndex = int(bezier.size() - 1);

    while (rightKeyIndex - leftKeyIndex > 1)
    {
        int searchIndex = (leftKeyIndex + rightKeyIndex) / 2;
        if (bezier[searchIndex].point.x > iFrame)
        {
            rightKeyIndex = searchIndex;
        }
        else
        {
            leftKeyIndex = searchIndex;
        }
    }
    
    return bezier[leftKeyIndex].point.y;
}

ULIS_NAMESPACE_END