// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SchedulePolicy.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FSchedulePolicy class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Scheduling/SchedulePolicy.h"
#include "System/Device.h"

ULIS_NAMESPACE_BEGIN
//static
static const FHardwareMetrics sHW;
const FSchedulePolicy FSchedulePolicy::AsyncCacheEfficient( ScheduleTime_Async, ScheduleRun_Multi,ScheduleMode_Chunks, ScheduleParameter_Length, sHW.L1CacheSize() );
const FSchedulePolicy FSchedulePolicy::CacheEfficient( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, sHW.L1CacheSize() );
const FSchedulePolicy FSchedulePolicy::AsyncMonoChunk( ScheduleTime_Async, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );
const FSchedulePolicy FSchedulePolicy::MonoChunk( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Chunks, ScheduleParameter_Count, 1 );
const FSchedulePolicy FSchedulePolicy::AsyncMultiScanlines( ScheduleTime_Async, ScheduleRun_Multi, ScheduleMode_Scanlines );
const FSchedulePolicy FSchedulePolicy::MultiScanlines( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Scanlines );
const FSchedulePolicy FSchedulePolicy::MonoScanlines( ScheduleTime_Sync, ScheduleRun_Mono, ScheduleMode_Scanlines );
//

FSchedulePolicy::~FSchedulePolicy()
{
}

FSchedulePolicy::FSchedulePolicy(
      eScheduleTimePolicy iTime
    , eScheduleRunPolicy iRun
    , eScheduleModePolicy iMode
    , eScheduleParameterPolicy iParam
    , int64 iValue
)
    : mTime( iTime )
    , mRun( iRun )
    , mMode( iMode )
    , mParameter( iParam )
    , mValue( iValue )
{
}

eScheduleTimePolicy
FSchedulePolicy::TimePolicy() const
{
    return  mTime;
}

eScheduleRunPolicy
FSchedulePolicy::RunPolicy() const
{
    return  mRun;
}

eScheduleModePolicy
FSchedulePolicy::ModePolicy() const
{
    return  mMode;
}

eScheduleParameterPolicy
FSchedulePolicy::ParameterPolicy() const
{
    return  mParameter;
}

int64
FSchedulePolicy::Value() const
{
    return  mValue;
}

ULIS_NAMESPACE_END

