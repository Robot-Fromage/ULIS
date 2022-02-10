// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedProperty.h
* @author       Thomas Schmitt
* @brief        This file provides declaration for animating a property
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Animation/HasKeys.h"
#include "Animation/Interpolation/AbstractInterpolation.h"

ULIS_NAMESPACE_BEGIN

template< typename T >
class TAnimatedProperty : public THasKeys<T>
{
public:
    TAnimatedProperty();
    TAnimatedProperty( T iDefaultValue );
    ~TAnimatedProperty();

public:
    virtual T GetValueAtFrame( ufloat iFrame ) const;

public:
    T GetDefaultValue() const;
    virtual void SetDefaultValue( T iDefaultValue );

protected:
    T mDefaultValue;
};

template< typename T >
TAnimatedProperty<T>::TAnimatedProperty()
{
}

template< typename T >
TAnimatedProperty<T>::TAnimatedProperty(T iDefaultValue) :
    mDefaultValue(iDefaultValue)
{

}

template< typename T >
TAnimatedProperty<T>::~TAnimatedProperty()
{

}

template< typename T >
T TAnimatedProperty<T>::GetValueAtFrame( ufloat iFrame ) const
{
    if(GetKeys().Size() == 0)
        return  mDefaultValue;

    if( GetKeys()[0].mFrame >= iFrame )
        return  GetKeys()[0].mValue;
    else if( GetKeys()[GetKeys().Size() - 1].mFrame <= iFrame )
        return  GetKeys()[GetKeys().Size() - 1].mValue;

    int leftKeyIndex = 0;
    int rightKeyIndex = GetKeys().Size() - 1;

    //Dichotomy to search for the successive keys to interpolate between them
    while(rightKeyIndex - leftKeyIndex > 1)
    {
        int searchIndex = (leftKeyIndex + rightKeyIndex) / 2;
        if(GetKeys()[searchIndex].mFrame > iFrame)
        {
            rightKeyIndex = searchIndex;
        }
        else
        {
            leftKeyIndex = searchIndex;
        }
    }

    return  GetKeys()[leftKeyIndex].GetInterpolation()->Interpolate( iFrame, GetKeys()[leftKeyIndex], GetKeys()[rightKeyIndex] );
}

template< typename T >
T TAnimatedProperty<T>::GetDefaultValue() const
{
    return  mDefaultValue;
}

template< typename T >
void TAnimatedProperty<T>::SetDefaultValue( T iDefaultValue )
{
    mDefaultValue = iDefaultValue;
}

ULIS_NAMESPACE_END

