// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Extract.h
* @author       Clement Berthaud
* @brief        This file provides the declarations for the Extract API.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Typedefs
typedef void (*fpExtract)( const uint8* iSrc, uint8* iDst, uint8* iStridesSrc, uint8* iStridesDst, uint8 iSRCSPP, uint8 iDSTSPP, uint8 iNumExtract, uint32 iLen );
ULIS_API fpExtract QueryDispatchedExtractInvocation( eType iSrcType, eType iDstType );

/////////////////////////////////////////////////////
// FDiskIOCommandArgs
class FExtractCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FExtractCommandArgs() override
    {
        delete [] sourceExtractMask;
        delete [] destinationExtractMask;
    }

    FExtractCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , uint8* iSourceExtractMask
        , uint8* iDestinationExtractMask
        , uint8 iNumExtract
        , fpExtract iInvocation
    )
        : FDualBufferCommandArgs(
              iSrc
            , iDst
            , iSrcRect
            , iDstRect
            )
        , sourceExtractMask( iSourceExtractMask )
        , destinationExtractMask( iDestinationExtractMask )
        , numExtract( iNumExtract )
        , invocation( iInvocation )
        {}

    uint8* sourceExtractMask;
    uint8* destinationExtractMask;
    uint8 numExtract;
    fpExtract invocation;
};

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleExtract );
ULIS_DECLARE_DISPATCHER( FDispatchedExtractInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedExtractInvocationSchedulerSelector
    , &ScheduleExtract
)

ULIS_NAMESPACE_END

