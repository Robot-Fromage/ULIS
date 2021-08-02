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
ULIS_DECLARE_DISPATCHER( FDispatchedResizeAreaInvocationSchedulerSelector                   )
ULIS_DECLARE_DISPATCHER( FDispatchedResizeBicubicInvocationSchedulerSelector                )
ULIS_DECLARE_DISPATCHER( FDispatchedResizeBilinearInvocationSchedulerSelector               )
ULIS_DECLARE_DISPATCHER( FDispatchedResizeNNInvocationSchedulerSelector                     )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformAffineBicubicInvocationSchedulerSelector       )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformAffineBilinearInvocationSchedulerSelector      )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformAffineNNInvocationSchedulerSelector            )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformAffineTiledBicubicInvocationSchedulerSelector  )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformAffineTiledBilinearInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformAffineTiledNNInvocationSchedulerSelector       )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformBezierBicubicInvocationSchedulerSelector       )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformBezierBilinearInvocationSchedulerSelector      )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformBezierNNInvocationSchedulerSelector            )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformPerspectiveBicubicInvocationSchedulerSelector  )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformPerspectiveBilinearInvocationSchedulerSelector )
ULIS_DECLARE_DISPATCHER( FDispatchedTransformPerspectiveNNInvocationSchedulerSelector       )

ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedResizeAreaInvocationSchedulerSelector,                    &ScheduleResizeMT_Area_MEM_Generic< T >                     )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedResizeBicubicInvocationSchedulerSelector,                 &ScheduleResizeMT_Bicubic_MEM_Generic< T >                  )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedResizeBilinearInvocationSchedulerSelector,                &ScheduleResizeMT_Bilinear_MEM_Generic< T >                 )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedResizeNNInvocationSchedulerSelector,                      &ScheduleResizeMT_NN_MEM_Generic< T >                       )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformAffineBicubicInvocationSchedulerSelector,        &ScheduleTransformAffineMT_Bicubic_MEM_Generic< T >         )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformAffineBilinearInvocationSchedulerSelector,       &ScheduleTransformAffineMT_Bilinear_MEM_Generic< T >        )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformAffineNNInvocationSchedulerSelector,             &ScheduleTransformAffineMT_NN_MEM_Generic< T >              )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformAffineTiledBicubicInvocationSchedulerSelector,   &ScheduleTransformAffineTiledMT_Bicubic_MEM_Generic< T >    )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformAffineTiledBilinearInvocationSchedulerSelector,  &ScheduleTransformAffineTiledMT_Bilinear_MEM_Generic< T >   )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformAffineTiledNNInvocationSchedulerSelector,        &ScheduleTransformAffineTiledMT_NN_MEM_Generic< T >         )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformBezierBicubicInvocationSchedulerSelector,        &ScheduleTransformBezierMT_Bicubic_MEM_Generic< T >         )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformBezierBilinearInvocationSchedulerSelector,       &ScheduleTransformBezierMT_Bilinear_MEM_Generic< T >        )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformBezierNNInvocationSchedulerSelector,             &ScheduleTransformBezierMT_NN_MEM_Generic< T >              )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformPerspectiveBicubicInvocationSchedulerSelector,   &ScheduleTransformPerspectiveMT_Bicubic_MEM_Generic< T >    )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformPerspectiveBilinearInvocationSchedulerSelector,  &ScheduleTransformPerspectiveMT_Bilinear_MEM_Generic< T >   )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedTransformPerspectiveNNInvocationSchedulerSelector,        &ScheduleTransformPerspectiveMT_NN_MEM_Generic< T >         )

ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleProcessBezierDeformField )
ULIS_DECLARE_DISPATCHER( FDispatchedProcessBezierDeformFieldSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO( FDispatchedProcessBezierDeformFieldSchedulerSelector, &ScheduleProcessBezierDeformField )

ULIS_NAMESPACE_END

