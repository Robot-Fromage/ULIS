// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Event.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FEvent class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/Event.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
FEvent::~FEvent()
{
    if( mHandle )
        mHandle->Untrack();
}

FEvent::FEvent()
    : bFinished( false )
    , mHandle( nullptr )
{
}

bool
FEvent::Finished() const
{
    return  bFinished;
}

void
FEvent::SetFinished()
{
    bFinished = true;
}

void
FEvent::Hook( FInternalEvent* iHandle )
{
    ULIS_ASSERT( iHandle, "Bad handle" );
    mHandle = iHandle;
    mHandle->Track( this );
}

FInternalEvent*
FEvent::Handle() const
{
    return  mHandle;
}

bool
FEvent::Hooked() const
{
    return  static_cast< bool >( mHandle != nullptr );
}

ULIS_NAMESPACE_END

