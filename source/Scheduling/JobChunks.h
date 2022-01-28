// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         JobChunks.h
* @author       Eric Scholl && Eric Scholl
* @brief        This file provides the declaration for the FJobChunks class.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/Job.h"

ULIS_NAMESPACE_BEGIN

#ifdef NEW_JOBSYSTEM

/////////////////////////////////////////////////////
/// @class      FJobChunks
/// @brief      A specification of FJob class providing execution of tasks as chunks of data.

template<
      typename TCommandArgs
    , typename TJobArgs
    , typename TDelegateBuildJobChunks
>
class FJobChunks
    : public FJob<TCommandArgs, TJobArgs>
{
public:
    /*! Destructor */
    virtual ~FJobChunks()
    {}

    /*! Constructor */
    FJobChunks(
        const uint64 iNumJobs
        , const uint64 iNumTaskPerJob
        , const uint64 mNumTasks
        , fpTask iTask
        , const TCommandArgs* iArgs
        , const int64 iChunkSize
        , TDelegateBuildJobChunks iDelegateBuildJobChunks
    )
        : FJob<TCommandArgs, TJobArgs>(iNumJobs, iNumTaskPerJob, mNumTasks, iTask, iArgs)
        , mChunkSize( iChunkSize )
        , mDelegateBuildJobChunks( iDelegateBuildJobChunks )
    {
    }

    /*! explicitly deleted default constructor. */
    FJobChunks() = delete;

    /*! explicitly deleted copy constructor. */
    FJobChunks(const FJobChunks&) = delete;

    /*! explicitly deleted move constructor. */
    FJobChunks(FJobChunks&&) = delete;

    /*! explicitly deleted copy assignment operator. */
    FJobChunks& operator=(const FJobChunks&) = delete;

    /*! explicitly deleted move assignment operator. */
    FJobChunks& operator=(FJobChunks&&) = delete;

public:
    /*! Start exec job tasks. */
    virtual void FillJobArgs(const TCommandArgs* iCommandArgs, TJobArgs& oJobArgs, uint64 iTaskIndex) override
    {
        mDelegateBuildJobChunks( iCommandArgs, mChunkSize, mChunkSize * iTaskIndex, iTaskIndex, oJobArgs );
    }

private:
    uint64 mChunkSize;
    TDelegateBuildJobChunks mDelegateBuildJobChunks;
};

#else

#endif

ULIS_NAMESPACE_END

