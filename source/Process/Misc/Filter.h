// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Filter.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for the Filter API.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"
#include "Scheduling/DualBufferArgs.h"
#include <functional>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// CommandArgs

typedef std::function< void( const FBlock& iBlock, const uint8* iPtr ) > fpFilterOPFunc;
typedef std::function< void( FBlock& iBlock, uint8* iPtr ) > fpFilterOPInPlaceFunc;
typedef std::function< void( const FBlock& iSrcBlock, const uint8* iSrcPtr, FBlock& iDstBlock, uint8* iDstPtr ) > fpFilterOPInto;
/////////////////////////////////////////////////////
// CommandArgs
class FFilterCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FFilterCommandArgs() override {}
    FFilterCommandArgs(
          FBlock& iSrc
        , const FRectI& iSrcRect
        , fpFilterOPFunc iInvocation
    )
        : FSimpleBufferCommandArgs( iSrc, iSrcRect )
        , invocation( iInvocation )
        {}

    fpFilterOPFunc invocation;
};

class FFilterInPlaceCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FFilterInPlaceCommandArgs() override {}
    FFilterInPlaceCommandArgs(
          FBlock& iSrc
        , const FRectI& iSrcRect
        , fpFilterOPInPlaceFunc iInvocation
    )
        : FSimpleBufferCommandArgs( iSrc, iSrcRect )
        , invocation( iInvocation )
        {}

    fpFilterOPInPlaceFunc invocation;
};

class FFilterIntoCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FFilterIntoCommandArgs() override {}
    FFilterIntoCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , fpFilterOPInto iInvocation
    )
        : FDualBufferCommandArgs( iSrc, iDst, iSrcRect, iDstRect )
        , invocation( iInvocation )
        {}

    fpFilterOPInto invocation;
};

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFilter_MT_MEM );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFilterInPlace_MT_MEM );
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleFilterInto_MT_MEM );
ULIS_DECLARE_DISPATCHER( FDispatchedFilterInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedFilterInPlaceInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedFilterIntoInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedFilterInvocationSchedulerSelector, &ScheduleFilter_MT_MEM )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedFilterInPlaceInvocationSchedulerSelector, &ScheduleFilterInPlace_MT_MEM )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedFilterIntoInvocationSchedulerSelector, &ScheduleFilterInto_MT_MEM )

ULIS_NAMESPACE_END

