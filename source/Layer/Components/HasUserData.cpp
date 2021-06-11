// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasUserData.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasUserData class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasUserData.h"

ULIS_NAMESPACE_BEGIN
IHasUserData::~IHasUserData() {
    ResetUserData();
}

IHasUserData::IHasUserData()
    : mUserData()
{}

void
IHasUserData::ResetUserData() {
    const uint64 size = mUserData.Size();
    for( uint64 i = 0; i < size; ++i )
        delete  mUserData[i];
    mUserData.Clear();
}

TArray< IUserData* >&
IHasUserData::GetUserDataArray()
{
    return  mUserData;
}

const TArray< IUserData* >&
IHasUserData::GetUserDataArray() const
{
    return  mUserData;
}

void
IHasUserData::AddOrSetUserData( IUserData* iData )
{
    uint32 id = iData->TypeID();
    const uint64 size = mUserData.Size();
    for( uint64 i = 0; i < size; ++i ) {
        if( id == mUserData[i]->TypeID() ) {
            delete mUserData[i];
            mUserData[i] = iData;
            return;
        }
    }

    mUserData.PushBack( iData );
}

bool
IHasUserData::UserDataExists( uint32 iTypeID ) const
{
    return  static_cast< bool >( GetUserData( iTypeID ) );
}

IUserData*
IHasUserData::GetUserData( uint32 iTypeID )
{
    const uint64 size = mUserData.Size();
    for( uint64 i = 0; i < size; ++i )
        if( mUserData[i]->TypeID() == iTypeID )
            return  mUserData[i];
    return  nullptr;
}

const IUserData*
IHasUserData::GetUserData( uint32 iTypeID ) const
{
    const uint64 size = mUserData.Size();
    for( uint64 i = 0; i < size; ++i )
        if( mUserData[i]->TypeID() == iTypeID )
            return  mUserData[i];
    return  nullptr;
}

ULIS_NAMESPACE_END

