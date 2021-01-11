// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Job.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FJob class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/Job.h"

ULIS_NAMESPACE_BEGIN

FJob::~FJob()
{
    // Cleanup assumes args were allocated as a raw buffer,
    // and IJobArgs object were constructed in place.
    // This requires to call the destructors manually,
    // and delete the buffer through its original type.
    for( uint32 i = 0; i < mNumTasks; ++i )
        delete  mArgs[i];

    delete [] mArgs;
}

FJob::FJob(
      uint32 iNumTasks
    , fpTask iTask
    , IJobArgs** iArgs
    , const FCommand* iParent
)
    : mNumTasks( iNumTasks )
    , mTask( iTask )
    , mArgs( iArgs )
    , mParent( iParent )
{
}

void
FJob::Execute() const
{
    for( uint32 i = 0; i < mNumTasks; ++i )
        mTask( mArgs[i], mParent->Args() );
}

const FCommand*
FJob::Parent() const
{
    return  mParent;
}

ULIS_NAMESPACE_END

