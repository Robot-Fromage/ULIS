// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SATMT_MEM_Generic.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the SATMT_MEM_Generic API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations Regular
template< typename T >
void
InvokeBuildSATXPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
}

template< typename T >
void
InvokeBuildSATYPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
}

template< typename T >
void
InvokeBuildPremultSATXPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
}

template< typename T >
void
InvokeBuildPremultSATYPassMT_MEM_Generic(
      const FDualBufferJobArgs* jargs
    , const FDualBufferCommandArgs* cargs
)
{
}

/////////////////////////////////////////////////////
// Scheduling
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildSATXPassMT_MEM_Generic, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildSATXPassMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildSATYPassMT_MEM_Generic, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildSATYPassMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildPremultSATXPassMT_MEM_Generic, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildPremultSATXPassMT_MEM_Generic< T > )
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleBuildPremultSATYPassMT_MEM_Generic, FDualBufferJobArgs, FDualBufferCommandArgs, &InvokeBuildPremultSATYPassMT_MEM_Generic< T > )

ULIS_NAMESPACE_END

