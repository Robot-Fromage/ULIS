// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasUserData.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the IHasUserData class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Components/UserData.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      IHasUserData
/// @brief      Simple HasUserData class.
class ULIS_API IHasUserData
{
protected:
    // DTor
    ~IHasUserData();

    // CTor
    IHasUserData();

    // Disable copy
    IHasUserData( const IHasUserData& ) = delete;
    IHasUserData& operator=( const IHasUserData& ) = delete;

public:
    // IHasUserData API
    void ResetUserData();
    TArray< IUserData* >& GetUserDataArray();
    const TArray< IUserData* >& GetUserDataArray() const;
    void AddOrSetUserData( IUserData* iData );
    void RemoveUserData( uint32 iTypeID );
    bool UserDataExists( uint32 iTypeID ) const;
    IUserData* GetUserData( uint32 iTypeID );
    const IUserData* GetUserData( uint32 iTypeID ) const;

    // IHasUserData Template API
    template< class TUserData >
    void RemoveUserData()
    {
        RemoveUserData( TUserData::StaticTypeID() );
    }

    template< class TUserData >
    bool
    HasUserData() const
    {
        return  HasUserData( TUserData::StaticTypeID() );
    }

    template< class TUserData >
    TUserData*
    GetUserData()
    {
        return  dynamic_cast< TUserData* >( GetUserData( TUserData::StaticTypeID() ) );
    }

    template< class TUserData >
    const TUserData*
    GetUserData() const {
        return  dynamic_cast< const TUserData* >( GetUserData( TUserData::StaticTypeID() ) );
    }

private:
    // Private Data Members
    TArray< IUserData* > mUserData;
};


ULIS_NAMESPACE_END

