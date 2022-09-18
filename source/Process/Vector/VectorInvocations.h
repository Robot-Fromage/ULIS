// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         VectorInvocations.h
* @author       Gary Gabriel
* @brief        This file provides the declarations of the invocations for the Raster API.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"

#include "Vector/FVectorObject.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FDrawVectorObjectCommandArgs
class FDrawVectorObjectCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDrawVectorObjectCommandArgs() override
    {
    }

    FDrawVectorObjectCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , FVectorObject& iVecObj
        , BLContext& iBLCtx
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , vecObj ( iVecObj )
        , blCtx ( iBLCtx )
    {
    }

    FVectorObject& vecObj;
    BLContext& blCtx;
};

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleDrawVectorObjectMT_MEM );
ULIS_DECLARE_DISPATCHER( FDispatchedDrawVectorObjectInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO ( FDispatchedDrawVectorObjectInvocationSchedulerSelector
                                          , &ScheduleDrawVectorObjectMT_MEM )


ULIS_NAMESPACE_END

