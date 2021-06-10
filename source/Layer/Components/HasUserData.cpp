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

void
IHasUserData::ResetUserData() {
    const uint64 size = mUserData.Size();
    for( uint64 i = 0; i < size; ++i )
        delete  mUserData[i];
    mUserData.Clear();
}
ULIS_NAMESPACE_END

