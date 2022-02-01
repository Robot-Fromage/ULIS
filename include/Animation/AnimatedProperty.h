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
#include "Memory/Array.h"
#include "Animation/HasKeys.h"
#include "Animation/Interpolation/Interpolation.h"
#include "Animation/Interpolation/LinearInterpolation.h"

ULIS_NAMESPACE_BEGIN

template< typename T >
class TAnimatedProperty
{
public:
    TAnimatedProperty();
    TAnimatedProperty( T iDefaultValue );
    ~TAnimatedProperty();

public:
    virtual T GetValueAtFrame(int iFrame) const;
    void AddKey( FKey<T>& iKey );
    bool RemoveKeyAtFrame(int iFrame);

public:
    T GetDefaultValue() const;
    virtual void SetDefaultValue( T iDefaultValue );

protected:
    TArray<FKey<T>> Keys;
    T DefaultValue;
};

template< typename T >
TAnimatedProperty<T>::TAnimatedProperty()
{
}

template< typename T >
TAnimatedProperty<T>::TAnimatedProperty(T iDefaultValue) :
    DefaultValue(iDefaultValue)
{

}

template< typename T >
TAnimatedProperty<T>::~TAnimatedProperty()
{

}

template< typename T >
T TAnimatedProperty<T>::GetValueAtFrame(int iFrame) const
{
    if (Keys.Size() == 0)
        return DefaultValue;

    if( Keys[0].Frame >= iFrame )
        return Keys[0].Value;
    else if ( Keys[Keys.Size() - 1].Frame <= iFrame )
        return Keys[Keys.Size() - 1].Value;

    int leftKeyIndex = 0;
    int rightKeyIndex = Keys.Size() - 1;

    //Dichotomy to search for the successive keys to interpolate between them
    while (rightKeyIndex - leftKeyIndex > 1)
    {
        int searchIndex = (leftKeyIndex + rightKeyIndex) / 2;
        if (Keys[searchIndex].Frame > iFrame)
        {
            rightKeyIndex = searchIndex;
        }
        else
        {
            leftKeyIndex = searchIndex;
        }
    }

    float t = float( iFrame - Keys[leftKeyIndex].Frame ) / float( Keys[rightKeyIndex].Frame - Keys[leftKeyIndex].Frame );

    return Keys[leftKeyIndex].Interpolation->Interpolate( t, Keys[leftKeyIndex].Value, Keys[rightKeyIndex].Value );
}

template< typename T >
void TAnimatedProperty<T>::AddKey(FKey<T>& iKey)
{
    if (Keys.Size() == 0)
    {
        Keys.PushBack(iKey);
        return;
    }

    if (Keys[0].Frame > iKey.Frame)
    {
        Keys.Insert(0, iKey);
        return;
    }
    else if (Keys[Keys.Size() - 1].Frame < iKey.Frame)
    {
        Keys.PushBack(iKey);
        return;
    }

    //Dichotomy to search for the index at which we want to insert our key
    int leftKeyIndex = 0;
    int rightKeyIndex = Keys.Size() - 1;

    while (rightKeyIndex - leftKeyIndex > 1)
    {
        int searchIndex = (leftKeyIndex + rightKeyIndex) / 2;
        if (Keys[searchIndex].Frame > iKey.Frame)
        {
            rightKeyIndex = searchIndex;
        }
        else
        {
            leftKeyIndex = searchIndex;
        }
    }

    Keys.Insert( rightKeyIndex, iKey );
}

template< typename T >
bool TAnimatedProperty<T>::RemoveKeyAtFrame(int iFrame)
{
    if (Keys.Size() == 0)
        return false;

    if (Keys[0].Frame == iFrame)
    {
        Keys.Erase( 0 );
        return true;
    }
    else if (Keys[Keys.Size() - 1].Frame == iFrame)
    {
        Keys.PopBack();
        return true;
    }

    //Dichotomy to search for the index at which we want to remove our key
    int leftKeyIndex = 0;
    int rightKeyIndex = Keys.Size() - 1;

    while (rightKeyIndex - leftKeyIndex > 1)
    {
        int searchIndex = (leftKeyIndex + rightKeyIndex) / 2;
        
        if (Keys[searchIndex].Frame == iFrame)
        {
            Keys.Erase( searchIndex );
            return true;
        }

        if (Keys[searchIndex].Frame > iFrame)
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

template< typename T >
T TAnimatedProperty<T>::GetDefaultValue() const
{
    return DefaultValue;
}

template< typename T >
void TAnimatedProperty<T>::SetDefaultValue(T iDefaultValue)
{
    DefaultValue = iDefaultValue;
}

ULIS_NAMESPACE_END