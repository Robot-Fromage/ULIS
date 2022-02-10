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
    FKey( float iFrame, T iValue, TInterpolation<T>* iInterpolation, FVec2F iLeftTangent = FVec2F(-1.f, 0.f), FVec2F iRightTangent = FVec2F(1.f, 0.f) );

    float Frame;
    T Value;
    TInterpolation<T>* Interpolation;
    FVec2F LeftTangent;
    FVec2F RightTangent;
};

template< typename T >
FKey<T>::FKey(float iFrame, T iValue, TInterpolation<T>* iInterpolation, FVec2F iLeftTangent, FVec2F iRightTangent ):
    Frame(iFrame),
    Value(iValue),
    Interpolation(iInterpolation),
    LeftTangent( iLeftTangent ), 
    RightTangent( iRightTangent )
{
}

template< class Type > using TKeyAddedDelegate = TLambdaCallback< void, FKey<Type> >;
template< class Type > using TKeyRemovedDelegate = TLambdaCallback< void, FKey<Type> >;
template< class Type > using TKeyChangedDelegate = TLambdaCallback< void, FKey<Type> >;

template< class Type > using TOnKeyAdded = TCallbackCapable< TKeyAddedDelegate< Type >, 0 >;
template< class Type > using TOnKeyRemoved = TCallbackCapable< TKeyRemovedDelegate< Type >, 1 >;
template< class Type > using TOnKeyChanged = TCallbackCapable< TKeyAddedDelegate< Type >, 2 >;

template< typename T >
class THasKeys:
    public TOnKeyAdded<T>,
    public TOnKeyRemoved<T>,
    public TOnKeyChanged<T>
{
public:
    THasKeys();
    ~THasKeys();

public:

    /** Adds a key in Keys Array, replaces existing key if there was already one at the same frame*/
    virtual void AddOrReplaceKey( FKey<T>& iKey );

    /** Removes a key at a certain frame. Return true if there was one at the frame passed in parameter */
    virtual bool RemoveKeyAtFrame(float iFrame);

    /** Const getter, useful to check or display the keys, but no modification is allowed */
    const ::ULIS::TArray<FKey<T>>& GetKeys() const;

private:
    ::ULIS::TArray<FKey<T>> Keys;
};

template< typename T >
THasKeys<T>::THasKeys()
{
}

template< typename T >
THasKeys<T>::~THasKeys()
{

}

template< typename T >
void THasKeys<T>::AddOrReplaceKey(FKey<T>& iKey)
{
    if (Keys.Size() == 0)
    {
        Keys.PushBack(iKey);
        TOnKeyAdded<T>::Invoke( iKey );
        return;
    }

    if (Keys[0].Frame == iKey.Frame)
    {
        Keys[0] = iKey;
        TOnKeyChanged<T>::Invoke(iKey);
        return;
    }
    else if (Keys[Keys.Size() - 1].Frame == iKey.Frame)
    {
        Keys[Keys.Size() - 1] = iKey;
        TOnKeyChanged<T>::Invoke(iKey);
        return;
    }

    if (Keys[0].Frame > iKey.Frame)
    {
        Keys.Insert(0, iKey);
        TOnKeyAdded<T>::Invoke( iKey );
        return;
    }
    else if (Keys[Keys.Size() - 1].Frame < iKey.Frame)
    {
        Keys.PushBack(iKey);
        TOnKeyAdded<T>::Invoke( iKey );
        return;
    }

    //Dichotomy to search for the index at which we want to insert our key
    int leftKeyIndex = 0;
    int rightKeyIndex = Keys.Size() - 1;

    while (rightKeyIndex - leftKeyIndex > 1)
    {
        int searchIndex = (leftKeyIndex + rightKeyIndex) / 2;
        
        if (Keys[searchIndex].Frame == iKey.Frame)
        {
            Keys[searchIndex] = iKey;
            TOnKeyChanged<T>::Invoke(iKey);
            return;
        }

        if (Keys[searchIndex].Frame > iKey.Frame)
        {
            rightKeyIndex = searchIndex;
        }
        else
        {
            leftKeyIndex = searchIndex;
        }
    }

    Keys.Insert(rightKeyIndex, iKey);
    TOnKeyAdded<T>::Invoke( iKey );
}

template< typename T >
bool THasKeys<T>::RemoveKeyAtFrame(float iFrame)
{
    if (Keys.Size() == 0)
        return false;

    if (Keys[0].Frame == iFrame)
    {
        TOnKeyRemoved<T>::Invoke( Keys[0] );
        Keys.Erase(0);
        return true;
    }
    else if (Keys[Keys.Size() - 1].Frame == iFrame)
    {
        TOnKeyRemoved<T>::Invoke( Keys[Keys.Size() - 1] );
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
            TOnKeyRemoved<T>::Invoke( Keys[searchIndex] );
            Keys.Erase(searchIndex);
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
const TArray<FKey<T>>& THasKeys<T>::GetKeys() const
{
    return Keys;
}

ULIS_NAMESPACE_END
