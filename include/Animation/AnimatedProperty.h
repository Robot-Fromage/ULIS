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
#include "Animation/Interpolation/Interpolation.h"
#include "Animation/Interpolation/LinearInterpolation.h"

template< typename Scalar >
struct FKey
{
    FKey( float iTime, Scalar iValue, TInterpolation<Scalar>* iInterpolation );

    float Time;
    Scalar Value;
    TInterpolation<Scalar>* Interpolation;
};

template< typename Scalar >
FKey<Scalar>::FKey(float iTime, Scalar iValue, TInterpolation<Scalar>* iInterpolation):
    Time(iTime),
    Value(iValue),
    Interpolation(iInterpolation)
{
}

template< typename Scalar >
class TAnimatedProperty
{
public:
    TAnimatedProperty();
    TAnimatedProperty( Scalar iDefaultValue );
    ~TAnimatedProperty();

public:
    virtual Scalar GetValueAtTime(float iTime) const;
    void AddKey( FKey<Scalar>& iKey );
    bool RemoveKeyAt(float iTime);

public:
    Scalar GetDefaultValue() const;
    virtual void SetDefaultValue( Scalar iDefaultValue );

protected:
    ::ULIS::TArray<FKey<Scalar>> Keys;
    Scalar DefaultValue;
};

template< typename Scalar >
TAnimatedProperty<Scalar>::TAnimatedProperty()
{
}

template< typename Scalar >
TAnimatedProperty<Scalar>::TAnimatedProperty(Scalar iDefaultValue) :
    DefaultValue(iDefaultValue)
{

}

template< typename Scalar >
TAnimatedProperty<Scalar>::~TAnimatedProperty()
{

}

template< typename Scalar >
Scalar TAnimatedProperty<Scalar>::GetValueAtTime(float iTime) const
{
    if (Keys.Size() == 0)
        return DefaultValue;

    if( Keys[0].Time >= iTime )
        return Keys[0].Value;
    else if ( Keys[Keys.Size() - 1].Time <= iTime )
        return Keys[Keys.Size() - 1].Value;

    int leftKeyIndex = 0;
    int rightKeyIndex = Keys.Size() - 1;

    //Dichotomy to search for the successive keys to interpolate between them
    while (rightKeyIndex - leftKeyIndex > 1)
    {
        int searchIndex = (leftKeyIndex + rightKeyIndex) / 2;
        if (Keys[searchIndex].Time > iTime)
        {
            rightKeyIndex = searchIndex;
        }
        else
        {
            leftKeyIndex = searchIndex;
        }
    }

    float t = ( iTime - Keys[leftKeyIndex].Time ) / ( Keys[rightKeyIndex].Time - Keys[leftKeyIndex].Time );

    return Keys[leftKeyIndex].Interpolation->Interpolate( t, Keys[leftKeyIndex].Value, Keys[rightKeyIndex].Value );
}

template< typename Scalar >
void TAnimatedProperty<Scalar>::AddKey(FKey<Scalar>& iKey)
{
    if (Keys.Size() == 0)
        Keys.PushBack(iKey);

    if (Keys[0].Time >= iKey.Time)
        Keys.Insert(0, iKey);
    else if (Keys[Keys.Size() - 1].Time <= iKey.Time)
        Keys.PushBack(iKey);

    //Dichotomy to search for the index at which we want to insert our key
    int leftKeyIndex = 0;
    int rightKeyIndex = Keys.Size() - 1;

    while (rightKeyIndex - leftKeyIndex > 1)
    {
        int searchIndex = (leftKeyIndex + rightKeyIndex) / 2;
        if (Keys[searchIndex].Time > iKey.Time)
        {
            rightKeyIndex = searchIndex;
        }
        else
        {
            leftKeyIndex = searchIndex;
        }
    }

    Keys.Insert( leftKeyIndex, iKey );
}

template< typename Scalar >
bool TAnimatedProperty<Scalar>::RemoveKeyAt(float iTime)
{
    if (Keys.Size() == 0)
        return false;

    if (Keys[0].Time == iTime)
    {
        Keys.Erase( 0 );
        return true;
    }
    else if (Keys[Keys.Size() - 1].Time == iTime)
    {
        Keys.Erase(Keys.Size() - 1);
    }

    //Dichotomy to search for the index at which we want to remove our key
    int leftKeyIndex = 0;
    int rightKeyIndex = Keys.Size() - 1;

    while (rightKeyIndex - leftKeyIndex > 1)
    {
        int searchIndex = (leftKeyIndex + rightKeyIndex) / 2;
        
        if (Keys[searchIndex].Time == iTime)
        {
            Keys.Erase( searchIndex );
            return true;
        }

        if (Keys[searchIndex].Time > iTime)
        {
            rightKeyIndex = searchIndex;
        }
        else
        {
            leftKeyIndex = searchIndex;
        }
    }

    return false;
}

template< typename Scalar >
Scalar TAnimatedProperty<Scalar>::GetDefaultValue() const
{
    return DefaultValue;
}

template< typename Scalar >
void TAnimatedProperty<Scalar>::SetDefaultValue(Scalar iDefaultValue)
{
    DefaultValue = iDefaultValue;
}

