// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         DrawLine.cpp
* @author       Thomas Schmitt
* @brief        This file provides the implementations for the Raster DrawLine API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Raster/DrawLine.h"
#include "Image/Block.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeDrawLineMT_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDrawLineCommandArgs* cargs
)
{
    /*
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                 \
            {                                                                                                                                   \
                TPainterContext< ULIS_REG[ n ] >::DrawLine( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                            \
                                                          , iP0, iP1, iColor, iClippingRect, iPerformanceOptions, iCallInvalidCB );             \
                break;                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }*/
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleDrawLineMT_MEM, FSimpleBufferJobArgs, FDrawLineCommandArgs, &InvokeDrawLineMT_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedDrawLineInvocationSchedulerSelector )

ULIS_NAMESPACE_END

