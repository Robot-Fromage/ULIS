// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Transform.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Transform API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Transform/TransformArgs.h"

// Include MEM Generic Implementation
#include "Process/Transform/Generic/ResizeMT_Area_MEM_Generic.h"
#include "Process/Transform/Generic/ResizeMT_Bicubic_MEM_Generic.h"
#include "Process/Transform/Generic/ResizeMT_Bilinear_MEM_Generic.h"
#include "Process/Transform/Generic/ResizeMT_NN_MEM_Generic.h"
#include "Process/Transform/Generic/TransformAffineMT_Bicubic_MEM_Generic.h"
#include "Process/Transform/Generic/TransformAffineMT_Bilinear_MEM_Generic.h"
#include "Process/Transform/Generic/TransformAffineMT_NN_MEM_Generic.h"
#include "Process/Transform/Generic/TransformAffineTiledMT_Bicubic_MEM_Generic.h"
#include "Process/Transform/Generic/TransformAffineTiledMT_Bilinear_MEM_Generic.h"
#include "Process/Transform/Generic/TransformAffineTiledMT_NN_MEM_Generic.h"
#include "Process/Transform/Generic/TransformBezier_Bicubic_MEM_Generic.h"
#include "Process/Transform/Generic/TransformBezier_Bilinear_MEM_Generic.h"
#include "Process/Transform/Generic/TransformBezier_NN_MEM_Generic.h"
#include "Process/Transform/Generic/TransformPerspectiveMT_Bicubic_MEM_Generic.h"
#include "Process/Transform/Generic/TransformPerspectiveMT_Bilinear_MEM_Generic.h"
#include "Process/Transform/Generic/TransformPerspectiveMT_NN_MEM_Generic.h"

ULIS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
// Dispatchers
ULIS_DECLARE_DISPATCHER( FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector  )
ULIS_DECLARE_DISPATCHER( FDispatchedAlphaBlendSeparableInvocationSchedulerSelector          )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector    )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendNonSeparableInvocationSchedulerSelector            )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector       )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendSeparableInvocationSchedulerSelector               )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector            )
ULIS_DECLARE_DISPATCHER( FDispatchedBlendMiscInvocationSchedulerSelector                    )
ULIS_DECLARE_DISPATCHER( FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector       )
ULIS_DECLARE_DISPATCHER( FDispatchedTiledBlendSeparableInvocationSchedulerSelector          )
ULIS_DECLARE_DISPATCHER( FDispatchedTiledBlendMiscInvocationSchedulerSelector               )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedAlphaBlendSeparableSubpixelInvocationSchedulerSelector,   &ScheduleAlphaBlendMT_Separable_MEM_Generic_Subpixel< T >   )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedAlphaBlendSeparableInvocationSchedulerSelector,           &ScheduleAlphaBlendMT_Separable_MEM_Generic< T >            )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendNonSeparableSubpixelInvocationSchedulerSelector,     &ScheduleBlendMT_NonSeparable_MEM_Generic_Subpixel< T >     )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendNonSeparableInvocationSchedulerSelector,             &ScheduleBlendMT_NonSeparable_MEM_Generic< T >              )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendSeparableSubpixelInvocationSchedulerSelector,        &ScheduleBlendMT_Separable_MEM_Generic_Subpixel< T >        )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendSeparableInvocationSchedulerSelector,                &ScheduleBlendMT_Separable_MEM_Generic< T >                 )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendMiscSubpixelInvocationSchedulerSelector,             &ScheduleBlendMT_Misc_MEM_Generic_Subpixel< T >             )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedBlendMiscInvocationSchedulerSelector,                     &ScheduleBlendMT_Misc_MEM_Generic< T >                      )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTiledBlendNonSeparableInvocationSchedulerSelector,        &ScheduleTiledBlendMT_NonSeparable_MEM_Generic< T >         )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTiledBlendSeparableInvocationSchedulerSelector,           &ScheduleTiledBlendMT_Separable_MEM_Generic< T >            )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTiledBlendMiscInvocationSchedulerSelector,                &ScheduleTiledBlendMT_Misc_MEM_Generic< T >                 )
ULIS_NAMESPACE_END

