// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         CallbackCapable.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TCallbackCapable class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Callback.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TCallbackCapable
/// @brief      The TCallbackCapable class provides a base abstract interface
///             enable callback capability on PIC classes.
template< class DelegateType, int = 0 >
class TCallbackCapable
{
public:
    using Delegate = DelegateType;

public:
    TCallbackCapable( const DelegateType& iDelegate = DelegateType() )
        : mDelegate( iDelegate )
    {}

    template< typename ... Args >
    ULIS_FORCEINLINE void Invoke( Args ... args ) const {
        mDelegate.ExecuteIfBound( args ... );
    }

private:
    DelegateType mDelegate;
};

#define ULIS_DECLARE_SIMPLE_DELEGATE( __Name__, __Ret__, ... )                          \
    typedef TCallbackCapable< TLambdaCallback< __Ret__, __VA_ARGS__ > > __Name__;       \
    template class ULIS_API TLambdaCallback< __Ret__, __VA_ARGS__ >;                    \
    template class ULIS_API TCallbackCapable< TLambdaCallback< __Ret__, __VA_ARGS__ > >;

#define ULIS_DECLARE_SIMPLE_DELEGATE_SPEC( __Name__, __Spec__, __Ret__, ... )                       \
    typedef TCallbackCapable< TLambdaCallback< __Ret__, __VA_ARGS__ >, __Spec__ > __Name__;         \
    template class ULIS_API TLambdaCallback< __Ret__, __VA_ARGS__ >;                                \
    template class ULIS_API TCallbackCapable< TLambdaCallback< __Ret__, __VA_ARGS__ >, __Spec__ >;

ULIS_NAMESPACE_END

