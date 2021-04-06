// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.SAT.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the SAT API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/SAT/SAT.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include <vector>

ULIS_NAMESPACE_BEGIN
ulError
FContext::BuildSummedAreaTable(
      const FBlock& iSource
    , FBlock& iDestination
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR( &iSource != &iDestination, "Source and Destination are the same block.", FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA ) );
    ULIS_ASSERT_RETURN_ERROR( iSource.Rect() == iDestination.Rect(), "Source and Destination must be the same size.", FinishEventNo_OP( iEvent, ULIS_ERROR_BAD_INPUT_DATA ) );
    ULIS_ASSERT_RETURN_ERROR( iDestination.Format() == SummedAreaTableMetrics( iSource ), "Cannot build an SAT in this format, use SummedAreaTableMetrics.", FinishEventNo_OP( iEvent, ULIS_ERROR_BAD_INPUT_DATA ) );

    FEvent xpass_event;
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleBuildSATXPass
            , new FDualBufferCommandArgs(
                  iSource
                , iDestination
                , iSource.Rect()
                , iDestination.Rect()
            )
            , iPolicy
            , false
            , false
            , iNumWait
            , iWaitList
            , &xpass_event
            , iDestination.Rect()
        )
    );

    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleBuildSATYPass
            , new FDualBufferCommandArgs(
                  iSource
                , iDestination
                , iSource.Rect()
                , iDestination.Rect()
            )
            , iPolicy
            , false
            , false
            , 1
            , &xpass_event
            , iEvent
            , iDestination.Rect()
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::BuildPremultipliedSummedAreaTable(
      const FBlock& iSource
    , FBlock& iDestination
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR( &iSource != &iDestination, "Source and Destination are the same block.", FinishEventNo_OP( iEvent, ULIS_ERROR_CONCURRENT_DATA ) );
    ULIS_ASSERT_RETURN_ERROR( iSource.Rect() == iDestination.Rect(), "Source and Destination must be the same size.", FinishEventNo_OP( iEvent, ULIS_ERROR_BAD_INPUT_DATA ) );
    ULIS_ASSERT_RETURN_ERROR( iDestination.Format() == SummedAreaTableMetrics( iSource ), "Cannot build an SAT in this format, use SummedAreaTableMetrics.", FinishEventNo_OP( iEvent, ULIS_ERROR_BAD_INPUT_DATA ) );

    FEvent xpass_event;
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleBuildPremultipliedSATXPass
            , new FDualBufferCommandArgs(
                  iSource
                , iDestination
                , iSource.Rect()
                , iDestination.Rect()
            )
            , iPolicy
            , false
            , false
            , iNumWait
            , iWaitList
            , &xpass_event
            , iDestination.Rect()
        )
    );

    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleBuildPremultipliedSATYPass
            , new FDualBufferCommandArgs(
                  iSource
                , iDestination
                , iSource.Rect()
                , iDestination.Rect()
            )
            , iPolicy
            , false
            , false
            , 1
            , &xpass_event
            , iEvent
            , iDestination.Rect()
        )
    );

    return  ULIS_NO_ERROR;
}

//static
eFormat
FContext::SummedAreaTableMetrics(
    const FBlock& iSource
)
{
    return  iSource.FormatMetrics().ReinterpretedType( eType::Type_ufloat ).FMT;
}

ULIS_NAMESPACE_END

