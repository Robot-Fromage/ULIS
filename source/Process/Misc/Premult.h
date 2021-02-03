// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Premult.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Premult API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Scheduling/Dispatcher.h"
#include "Image/Color.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
template< typename T >
void
InvokePremult(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    T* dst = reinterpret_cast< T* >( jargs->dst );
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    for( int i = 0; i < jargs->size; ++i ) {
        T alpha = fmt.HEA ? *( dst + fmt.AID ) : MaxType< T >();
        for( int j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            *( dst + r ) = ( *( dst + r ) * alpha ) / MaxType< T >();
        }
        dst += fmt.SPP;
    }
}

template< typename T >
void
InvokeUnpremult(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    T* dst = reinterpret_cast< T* >( jargs->dst );
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    for( int i = 0; i < jargs->size; ++i ) {
        T alpha = fmt.HEA ? *( dst + fmt.AID ) : MaxType< T >();
        for( int j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            *( dst + r ) = ( *( dst + r ) * MaxType< T >() ) / alpha;
        }
        dst += fmt.SPP;
    }
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( SchedulePremultMT_MEM_Generic, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokePremultMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleUnpremultMT_MEM_Generic, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeUnpremultMT_MEM_Generic< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedPremultInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedUnpremultInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedPremultInvocationSchedulerSelector, &SchedulePremultMT_MEM_Generic< T > )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedUnpremultInvocationSchedulerSelector, &ScheduleUnpremultMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

