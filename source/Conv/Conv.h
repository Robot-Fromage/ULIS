// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Conv.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Conv entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/Conv.h"
#include "Dispatch/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Typedefs
typedef void (*fpConvertFormat)( FPixel iSrc, FPixel iDst, uint32 iLen );
ULIS_API fpConvertFormat QueryDispatchedConvertFormatInvocation( eFormat iSrcFormat, eFormat iDstFormat );

/////////////////////////////////////////////////////
// FConvCommandArgs
class FConvCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FConvCommandArgs() override {}

    FConvCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , const fpConvertFormat iInvocation
    )
        : FDualBufferCommandArgs(
              iSrc
            , iDst
            , iSrcRect
            , iDstRect
            )
        , invocation( iInvocation )
        {}

    fpConvertFormat invocation;
};

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleConvertFormat_MEM_Generic );
ULIS_DECLARE_DISPATCHER( FDispatchedConvertFormatInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedConvertFormatInvocationSchedulerSelector
    , &ScheduleConvertFormat_MEM_Generic
)

/////////////////////////////////////////////////////
// Convert Format Invocations Declarations
#define X_DECLARE_CONVERT_FORMAT_INVOCATION( A, B ) template< typename T1, typename T2 > void ConvertFormat ## A ## To ## B ## ( FPixel iSrc, FPixel iDst, uint32 iLen );
ULIS_FOR_ALL_COLOR_MODELS_COMBINATIONS_DO( X_DECLARE_CONVERT_FORMAT_INVOCATION )
#undef X_DECLARE_CONVERT_FORMAT_INVOCATION

ULIS_NAMESPACE_END

