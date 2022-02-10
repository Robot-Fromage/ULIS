// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         DefaultValue.h
* @author       Clement Berthaud
* @brief        This file provides declaration for TDefaultValue
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <functional>

ULIS_NAMESPACE_BEGIN
template< typename T >
class TDefaultValue
{
    typedef std::function< T( ufloat ) > tExpression;

public:
    ~TDefaultValue() {}

private:
    TDefaultValue( tExpression iExpression )
        : mExpression( iExpression )
    {}

    TDefaultValue( const TDefaultValue< T >& iOther ) = delete;
    TDefaultValue& operator=( const TDefaultValue< T >& iOther ) = delete;

public:
    static TDefaultValue FromValue( const T& iValue ) {
        return  TDefaultValue( [iValue]( ufloat ){ return  iValue; } );
    }

    static TDefaultValue FromExpression( tExpression iExpression ) {
        return  TDefaultValue( iExpression );
    }

private
    tExpression mExpression;
};

ULIS_NAMESPACE_END

