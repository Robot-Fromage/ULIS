// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Generic_OP.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for the Generic_OP API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Scheduling/ScheduleArgs.h"
#include "Math/Geometry/Rectangle.h"
#include <functional>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Args
class FGeneric_OPCommandArgs final
    : public ICommandArgs
{
public:
    ~FGeneric_OPCommandArgs() override {};
    FGeneric_OPCommandArgs(
        const std::function< void() >& iInvocation
    )
        : ICommandArgs( FRectI() )
        , invocation( iInvocation )
    {}

    std::function< void() > invocation;
};

class FGeneric_OPJobArgs final
    : public IJobArgs
{
public:
    ~FGeneric_OPJobArgs() override {};
};

/////////////////////////////////////////////////////
// Builders
static void BuildGeneric_OPJob_Scanlines( const FGeneric_OPCommandArgs* iCargs, const int64 iIndex, FGeneric_OPJobArgs& oJargs ) {}
static void BuildGeneric_OPJob_Chunks( const FGeneric_OPCommandArgs* iCargs, const int64 iSize, const int64 iOffset, const int64 iIndex, FGeneric_OPJobArgs& oJargs ) {}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleGeneric_OP );

ULIS_NAMESPACE_END

