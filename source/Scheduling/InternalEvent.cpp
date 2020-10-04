// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         InternalEvent.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FInternalEvent class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
FInternalEvent::~FInternalEvent()
{
}

FInternalEvent::FInternalEvent( FEvent* iEvent )
    : mHook( iEvent )
{
}

//static
FSharedInternalEvent
FInternalEvent::MakeShared( FEvent* iEvent )
{
    return  std::make_shared< FInternalEvent >( iEvent );
}

void
FInternalEvent::Untrack()
{
    mHook = nullptr;
}

ULIS_NAMESPACE_END

