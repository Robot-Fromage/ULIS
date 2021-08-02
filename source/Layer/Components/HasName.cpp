// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasName.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasName class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasName.h"

ULIS_NAMESPACE_BEGIN
IHasName::IHasName(
      const FString& iValue
    , const FOnNameChanged& iDelegate
)
    : FOnNameChanged( iDelegate )
    , mStr( iValue )
{
    Invoke( mStr );
}

const FString&
IHasName::Name() const {
    return  mStr;
}

void
IHasName::SetName( const FString& iValue ) {
    mStr = iValue;
    Invoke( mStr );
}

ULIS_NAMESPACE_END

