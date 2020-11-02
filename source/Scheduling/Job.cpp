// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         Job.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FJob class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/Job.h"

ULIS_NAMESPACE_BEGIN

FJob::~FJob()
{
}

FJob::FJob(
      uint32 iNumTasks
    , fpTask iTask
    , const IJobArgs* iArgs
)
    : mNumTasks( iNumTasks )
    , mTask( iTask )
    , mArgs( iArgs )
{
}

void
FJob::Execute()
{
    for( uint32 i = 0; i < mNumTasks; ++i )
        mTask( &mArgs[i], mParent->Args() );
}

const FCommand*
FJob::Parent() const
{
    return  mParent;
}

ULIS_NAMESPACE_END

