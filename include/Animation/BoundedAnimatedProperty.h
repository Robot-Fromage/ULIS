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

#include "Memory/Array.h"
#include "Animation/AnimatedProperty.h"

template< typename Scalar >
class TBoundedAnimatedProperty : public TAnimatedProperty<Scalar>
{
public:
    TBoundedAnimatedProperty( Scalar iDefaultValue, Scalar iMinValue, Scalar iMaxValue );
    ~TBoundedAnimatedProperty();

public:
    //We can add keys that have a value outside the bounds of MinValue;MaxValue. We just clamp the value we get at time iTime
    virtual Scalar GetValueAtTime(float iTime) const override;

public:
    virtual void SetDefaultValue(Scalar iDefaultValue) override;
    void SetMinValue( Scalar iNewMinValue );
    void SetMaxValue( Scalar iNewMaxValue );
    Scalar GetMinValue() const;
    Scalar GetMaxValue() const;

protected:
    Scalar MinValue;
    Scalar MaxValue;
};

template< typename Scalar >
TBoundedAnimatedProperty<Scalar>::TBoundedAnimatedProperty(Scalar iDefaultValue, Scalar iMinValue, Scalar iMaxValue)
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

template< typename Scalar >
TBoundedAnimatedProperty<Scalar>::~TBoundedAnimatedProperty()
{

}

template< typename Scalar >
Scalar TBoundedAnimatedProperty<Scalar>::GetValueAtTime(float iTime) const
{
    Scalar value = TAnimatedProperty<Scalar>::GetValueAtTime( iTime );
    return FMath::Clamp( value, MinValue, MaxValue );
}

template< typename Scalar >
void TBoundedAnimatedProperty<Scalar>::SetDefaultValue(Scalar iDefaultValue)
{
    DefaultValue = FMath::Clamp( iDefaultValue, MinValue, MaxValue );
}

template< typename Scalar >
void TBoundedAnimatedProperty<Scalar>::SetMinValue(Scalar iNewMinValue)
{
    if (iNewMinValue > MaxValue)
        MinValue = MaxValue;
    else
        MinValue = iNewMinValue;
}

template< typename Scalar >
void TBoundedAnimatedProperty<Scalar>::SetMaxValue(Scalar iNewMaxValue)
{
    if( iNewMaxValue < MinValue )
        MaxValue = MinValue;
    else
        MaxValue = iNewMaxValue;
}

template< typename Scalar >
Scalar TBoundedAnimatedProperty<Scalar>::GetMinValue() const
{
    return MinValue;
}

template< typename Scalar >
Scalar TBoundedAnimatedProperty<Scalar>::GetMaxValue() const
{
    return MaxValue;
}
