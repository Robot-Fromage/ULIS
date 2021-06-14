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
template< class DelegateType >
class TCallbackCapable
{
protected:
    TCallbackCapable( const DelegateType& iDelegate = DelegateType() )
        : mDelegate( iDelegate )
    {}

    template< typename ... Args >
    ULIS_FORCEINLINE void OnChanged( Args ... args ) const {
        mDelegate.ExecuteIfBound( args ... );
    }

private:
    DelegateType mDelegate;
};

#define ULIS_DECLARE_SIMPLE_DELEGATE( __Name__, __Ret__, ... )          \
    typedef TLambdaCallback< __Ret__, __VA_ARGS__ > __Name__;           \
    template class ULIS_API TLambdaCallback< __Ret__, __VA_ARGS__ >;    \
    template class ULIS_API TCallbackCapable< __Name__ >;

ULIS_NAMESPACE_END

