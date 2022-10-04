// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Sanitize.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Sanitize API.
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
InvokeSanitizeZeroAlphaMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FSimpleBufferCommandArgs* cargs
)
{
    T* dst = reinterpret_cast< T* >( jargs->dst );
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    const T zero = MinType< T >();
    for( int i = 0; i < jargs->size; ++i ) {
        const T alpha = fmt.HEA ? *( dst + fmt.AID ) : MaxType< T >();
        for( int j = 0; j < fmt.NCC; ++j ) {
            uint8 r = fmt.IDT[j];
            *( dst + r ) = alpha == zero ? zero : *( dst + r );
        }
        dst += fmt.SPP;
    }
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleSanitizeZeroAlphaMT_MEM_Generic, FSimpleBufferJobArgs, FSimpleBufferCommandArgs, &InvokeSanitizeZeroAlphaMT_MEM_Generic< T > )
ULIS_DECLARE_DISPATCHER( FDispatchedSanitizeZeroAlphaInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedSanitizeZeroAlphaInvocationSchedulerSelector, &ScheduleSanitizeZeroAlphaMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

