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
#pragma once
#include "Scheduling/SchedulePolicy.h"

ULIS_NAMESPACE_BEGIN
FSchedulePolicy::~FSchedulePolicy()
{
}

FSchedulePolicy::FSchedulePolicy(
      eScheduleRunPolicy iRun
    , eScheduleModePolicy iMode
    , eScheduleParameterPolicy iParam
    , int64 iValue
)
    : mRun( iRun )
    , mMode( iMode )
    , mParameter( iParam )
    , mValue( iValue )
{
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

