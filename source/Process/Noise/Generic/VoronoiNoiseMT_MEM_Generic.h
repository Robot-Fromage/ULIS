// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         VoronoiNoiseMT_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the implementation of a Noise type.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/Constants.h"
#include "Image/Block.h"
#include "Process/Noise/NoiseArgs.h"

ULIS_NAMESPACE_BEGIN
template< typename T >
void
InvokeVoronoiNoiseMT_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FVoronoiNoiseCommandArgs* cargs
)
{
    // Gather basic data for image traversal
    const FFormatMetrics& fmt = cargs->dst.FormatMetrics();
    T* ULIS_RESTRICT dst = reinterpret_cast< T* >( jargs->dst );
}

ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleVoronoiNoiseMT_MEM_Generic, FSimpleBufferJobArgs, FVoronoiNoiseCommandArgs, &InvokeVoronoiNoiseMT_MEM_Generic< T > )
ULIS_NAMESPACE_END

