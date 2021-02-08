// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         GammaCompress.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the GammaCompress API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Image/Color.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Process/Conv/srgb2linear.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
template< typename T >
void
InvokeApplysRGB2LinearMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    T* dst = reinterpret_cast< T* >( jargs->dst );
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    for( int i = 0; i < jargs->size; ++i ) {
        for( int j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            *( dst + r ) = sel_srgb2linearT< T >( *( dst + r ) );
        }
        dst += fmt.SPP;
    }
}

template< typename T >
void
InvokeApplyLinear2sRGBMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    T* dst = reinterpret_cast< T* >( jargs->dst );
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    for( int i = 0; i < jargs->size; ++i ) {
        for( int j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            *( dst + r ) = sel_linear2srgbT< T >( *( dst + r ) );
        }
        dst += fmt.SPP;
    }
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleApplysRGB2LinearMT_MEM_Generic, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeApplysRGB2LinearMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleApplyLinear2sRGBMT_MEM_Generic, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeApplyLinear2sRGBMT_MEM_Generic< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedApplysRGB2LinearInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedApplyLinear2sRGBInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedApplysRGB2LinearInvocationSchedulerSelector, &ScheduleApplysRGB2LinearMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedApplyLinear2sRGBInvocationSchedulerSelector, &ScheduleApplyLinear2sRGBMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

