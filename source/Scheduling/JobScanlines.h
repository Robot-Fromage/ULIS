// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         JobScanlines.h
* @author       Eric Scholl
* @brief        This file provides the declaration for the FJobScanlines class.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Scheduling/Job.h"

ULIS_NAMESPACE_BEGIN

#ifdef NEW_JOBSYSTEM

/////////////////////////////////////////////////////
/// @class      FJobScanlines
/// @brief      A specification of FJob class providing execution of tasks as Scanlines of data.

template<
      typename TCommandArgs
    , typename TJobArgs
    , typename TDelegateBuildJobScanlines
>
class FJobScanlines
    : public FJob<TCommandArgs, TJobArgs>
{
public:
    /*! Destructor */
    virtual ~FJobScanlines()
    {
    }

    /*! Constructor */
    FJobScanlines(
        const uint64 iNumJobs
        , const uint64 iNumTaskPerJob
        , const uint64 iNumTasks
        , fpTask iTask
        , const TCommandArgs* iArgs
        , TDelegateBuildJobScanlines iDelegateBuildJobScanlines
    )
        : FJob<TCommandArgs, TJobArgs>(iNumJobs, iNumTaskPerJob, iNumTasks, iTask, iArgs)
        , mDelegateBuildJobScanlines( iDelegateBuildJobScanlines )
    {
    }

    /*! explicitly deleted default constructor. */
    FJobScanlines() = delete;

    /*! explicitly deleted copy constructor. */
    FJobScanlines(const FJobScanlines&) = delete;

    /*! explicitly deleted move constructor. */
    FJobScanlines(FJobScanlines&&) = delete;

    /*! explicitly deleted copy assignment operator. */
    FJobScanlines& operator=(const FJobScanlines&) = delete;

    /*! explicitly deleted move assignment operator. */
    FJobScanlines& operator=(FJobScanlines&&) = delete;

public:
    /*! Start exec job tasks. */
    virtual void FillJobArgs(const TCommandArgs* iCommandArgs, TJobArgs& oJobArgs, uint64 iTaskIndex) override
    {
        mDelegateBuildJobScanlines( iCommandArgs, iTaskIndex, oJobArgs );
    }

private:
    TDelegateBuildJobScanlines mDelegateBuildJobScanlines;
};

#else
#endif

ULIS_NAMESPACE_END

