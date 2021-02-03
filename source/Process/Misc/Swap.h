// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Swap.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for the Swap API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FSwapCommandArgs
class FSwapCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FSwapCommandArgs() override
    {
    }

    FSwapCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , uint8 iChannel1
        , uint8 iChannel2
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , channel1( iChannel1 )
        , channel2( iChannel2 )
    {}

    uint8 channel1;
    uint8 channel2;
};

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleSwapMT_MEM );
ULIS_DECLARE_DISPATCHER( FDispatchedSwapInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedSwapInvocationSchedulerSelector, &ScheduleSwapMT_MEM )
ULIS_NAMESPACE_END

