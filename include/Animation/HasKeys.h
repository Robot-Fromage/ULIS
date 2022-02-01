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
#include "Core/CallbackCapable.h"

ULIS_NAMESPACE_BEGIN

template< typename T >
struct FKey
{
    FKey( int iFrame, T iValue, TInterpolation<T>* iInterpolation );

    int Frame;
    T Value;
    TInterpolation<T>* Interpolation;
};

template< typename T >
FKey<T>::FKey(int iFrame, T iValue, TInterpolation<T>* iInterpolation):
    Frame(iFrame),
    Value(iValue),
    Interpolation(iInterpolation)
{
}

/*template<typename T>
ULIS_DECLARE_SIMPLE_DELEGATE(FOnKeyAdded, void, const FKey<T>&)

ULIS_DECLARE_SIMPLE_DELEGATE(FOnKeyRemoved, void, const FKey<T>&)
ULIS_DECLARE_SIMPLE_DELEGATE(FOnKeyChanged, void, const FKey<T>&)
template< typename Scalar >
class IHasKeys:
    public FOnKeyAdded,
    public FOnKeyRemoved,
    public FOnKeyChanged
{
public:
    IHasKeys();
    ~IHasKeys();

public:
    void AddKey( FKey<Scalar>& iKey );
    bool RemoveKeyAt(float iTime);
    ::ULIS::TArray<FKey<Scalar>>& GetKeys();

private:
    ::ULIS::TArray<FKey<Scalar>> Keys;

    FKey LastAddedKey;
    FKey LastRemovedKey;
    FKey LastChangedKey;
};

template< typename Scalar >
IHasKeys<Scalar>::IHasKeys()
{
}

template< typename Scalar >
IHasKeys<Scalar>::~IHasKeys()
{

}

template< typename Scalar >
void IHasKeys<Scalar>::AddKey(FKey<Scalar>& iKey)
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
bool IHasKeys<Scalar>::RemoveKeyAt(float iTime)
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
TArray<FKey<Scalar>>& IHasKeys<Scalar>::GetKeys()
{
    return Keys;
}*/

ULIS_NAMESPACE_END
