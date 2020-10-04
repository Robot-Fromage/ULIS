// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Event_Private.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FEvent_Private class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include <memory>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FEvent::FEvent_Private
FEvent::FEvent_Private::FEvent_Private( FEvent* iUserEvent )
    : mHandle( FInternalEvent::MakeShared( iUserEvent ) )
{}

ULIS_NAMESPACE_END

