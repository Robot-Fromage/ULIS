// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedProperty.h
* @author       Thomas Schmitt
* @brief        This file provides declaration for animating a property in the time
* @license      Please refer to LICENSE.md
*/
#pragma once

#include "Core/Core.h"
#include "Memory/Array.h"
#include "Animation/AnimatedProperty.h"

ULIS_NAMESPACE_BEGIN

template< typename T >
class TBoundedAnimatedProperty : public TAnimatedProperty<T>
{
public:
    TBoundedAnimatedProperty( T iDefaultValue, T iMinValue, T iMaxValue );
    ~TBoundedAnimatedProperty();

public:
    //We can add keys that have a value outside the bounds of MinValue;MaxValue. We just clamp the value we get at frame iFrame
    virtual T GetValueAtFrame(float iFrame) const override;

    virtual void AddOrReplaceKey(FKey<T>& iKey) override;

public:
    virtual void SetDefaultValue(T iDefaultValue) override;
    T GetMinValue() const;
    T GetMaxValue() const;

protected:
    T MinValue;
    T MaxValue;
};

template< typename T >
TBoundedAnimatedProperty<T>::TBoundedAnimatedProperty(T iDefaultValue, T iMinValue, T iMaxValue)
{
    if (iMinValue > iMaxValue)
    {
        MinValue = iMaxValue;
        MaxValue = iMinValue;
    }
    else
    {
        MinValue = iMinValue;
        MaxValue = iMaxValue;
    }

    if( iDefaultValue < MinValue )
        DefaultValue = MinValue;
    else if( iDefaultValue > MaxValue )
        DefaultValue = MaxValue;
    else
        DefaultValue = iDefaultValue;
        
}

template< typename T >
TBoundedAnimatedProperty<T>::~TBoundedAnimatedProperty()
{

}

template< typename T >
T TBoundedAnimatedProperty<T>::GetValueAtFrame(float iFrame) const
{
    T value = TAnimatedProperty<T>::GetValueAtFrame( iFrame );
    return FMath::Clamp( value, MinValue, MaxValue );
}

template< typename T >
void TBoundedAnimatedProperty<T>::AddOrReplaceKey(FKey<T>& iKey)
{
    iKey.Value = FMath::Clamp( iKey.Value, MinValue, MaxValue );
    THasKeys<T>::AddOrReplaceKey( iKey );
}

template< typename T >
void TBoundedAnimatedProperty<T>::SetDefaultValue(T iDefaultValue)
{
    DefaultValue = FMath::Clamp( iDefaultValue, MinValue, MaxValue );
}

template< typename T >
T TBoundedAnimatedProperty<T>::GetMinValue() const
{
    return MinValue;
}

template< typename T >
T TBoundedAnimatedProperty<T>::GetMaxValue() const
{
    return MaxValue;
}

ULIS_NAMESPACE_END