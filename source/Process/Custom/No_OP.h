// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         No_OP.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for the No_OP API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Scheduling/ScheduleArgs.h"
#include "Math/Geometry/Rectangle.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Args
class FNo_OPCommandArgs final
    : public ICommandArgs
{
public:
    ~FNo_OPCommandArgs() override {};
    FNo_OPCommandArgs()
        : ICommandArgs( FRectI() )
    {}
};

class FNo_OPJobArgs final
    : public IJobArgs
{
public:
    ~FNo_OPJobArgs() override {};
};

/////////////////////////////////////////////////////
// Builders
static void BuildNo_OPJob_Scanlines( const FNo_OPCommandArgs* iCargs, const int64 iNumJobs, const int64 iNumTasksPerJob, const int64 iIndex, FNo_OPJobArgs& oJargs ) {}
static void BuildNo_OPJob_Chunks( const FNo_OPCommandArgs* iCargs, const int64 iSize, const int64 iCount, const int64 iOffset, const int64 iIndex, FNo_OPJobArgs& oJargs ) {}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleNo_OP );

ULIS_NAMESPACE_END

