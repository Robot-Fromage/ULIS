// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Alloc.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Alloc API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// AllocArgs
class FAllocCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FAllocCommandArgs() override {}
    FAllocCommandArgs(
          FBlock& iSrc
        , const FRectI& iSrcRect
        , FVec2I iSize
        , eFormat iFormat
        , const FColorSpace* iColorSpace
        , const FOnInvalidBlock& iOnInvalid
        , const FOnCleanupData& iOnCleanup
    )
        : FSimpleBufferCommandArgs( iSrc, iSrcRect )
        , size( iSize )
        , format( iFormat )
        , colorspace( iColorSpace )
        , onInvalid( iOnInvalid )
        , onCleanup( iOnCleanup )
        {}

    FVec2I size;
    eFormat format;
    const FColorSpace* colorspace;
    FOnInvalidBlock onInvalid;
    FOnCleanupData onCleanup;
};

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleAlloc )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDealloc )

ULIS_NAMESPACE_END

