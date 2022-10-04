// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Job.cpp
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the definition for the FJob class.
* @license      Please refer to LICENSE.md
*/
#include "Scheduling/Job.h"

ULIS_NAMESPACE_BEGIN

#ifdef NEW_JOBSYSTEM

#else
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
    // Gather event
    //FSharedInternalEvent evt = Parent()->Event();

    for( uint32 i = 0; i < mNumTasks; ++i )
        mTask( mArgs[i], mParent->Args() );

    // Notify job's done
    //evt->NotifyOneJobFinished();
}

const FCommand*
FJob::Parent() const
{
    return  mParent;
}
#endif

ULIS_NAMESPACE_END

