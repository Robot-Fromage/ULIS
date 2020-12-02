// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         FillPreserveAlpha.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for FillPreserveAlpha API.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

#pragma once
#include "Core/Core.h"
#include "Dispatch/Dispatcher.h"
#include "Image/Color.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FFillPreserveAlphaCommandArgs
class FFillPreserveAlphaCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FFillPreserveAlphaCommandArgs() override {}
    FFillPreserveAlphaCommandArgs(
          FBlock& iBlock
        , const FColor& iColor
        , const FRectI& iRect
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , color( iColor )
    {}

    const FColor color;
};

/////////////////////////////////////////////////////
// Scheduler
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFillPreserveAlphaMT_MEM );

/////////////////////////////////////////////////////
// Dispatch
ULIS_DECLARE_DISPATCHER( FDispatchedFillPreserveAlphaInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedFillPreserveAlphaInvocationSchedulerSelector
    , &ScheduleFillPreserveAlphaMT_MEM< T >
)

ULIS_NAMESPACE_END

