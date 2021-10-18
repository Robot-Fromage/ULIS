// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Filter.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Filter API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Misc/Filter.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
void InvokeFilter( const FSimpleBufferJobArgs* jargs, const FFilterCommandArgs* cargs ) {
    uint8* dst = jargs->dst;
    uint8 bpp = cargs->dst.BytesPerPixel();
    for( uint32 i = 0; i < jargs->size; i += bpp ) { //size = size in bytes of src
        cargs->invocation( cargs->dst, dst );
        dst += bpp;
    }
}

void InvokeFilterInPlace( const FSimpleBufferJobArgs* jargs, const FFilterInPlaceCommandArgs* cargs ) {
    uint8* dst = jargs->dst;
    uint8 bpp = cargs->dst.BytesPerPixel();
    for( uint32 i = 0; i < jargs->size; i += bpp ) {
        cargs->invocation( cargs->dst, dst );
        dst += bpp;
    }
}

void InvokeFilterInto( const FDualBufferJobArgs* jargs, const FFilterIntoCommandArgs* cargs ) {
    const uint8* src = jargs->src;
    uint8* dst = jargs->dst;
    uint8 src_bpp = cargs->src.BytesPerPixel();
    uint8 dst_bpp = cargs->dst.BytesPerPixel();
    for( uint32 i = 0; i < jargs->size; i += src_bpp ) {
        cargs->invocation( cargs->src, src, cargs->dst, dst );
        src += src_bpp;
        dst += dst_bpp;
    }
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleFilter_MT_MEM, FSimpleBufferJobArgs, FFilterCommandArgs, &InvokeFilter )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleFilterInPlace_MT_MEM, FSimpleBufferJobArgs, FFilterInPlaceCommandArgs, &InvokeFilterInPlace )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleFilterInto_MT_MEM, FDualBufferJobArgs, FFilterIntoCommandArgs, &InvokeFilterInto )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedFilterInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedFilterInPlaceInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedFilterIntoInvocationSchedulerSelector )

ULIS_NAMESPACE_END

