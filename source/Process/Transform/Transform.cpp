// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for Transform API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Transform/Transform.h"
#include "Math/Math.h"
#include "Math/Interpolation/Bezier.h"
#include "Math/Interpolation/Spline.h"

// Include SSE RGBA8 Implementation
#ifdef ULIS_COMPILETIME_SSE_SUPPORT
#include "Process/Transform/RGBA8/ResizeMT_NN_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/ResizeMT_Bilinear_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/ResizeMT_Bicubic_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/ResizeMT_Area_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformAffineMT_NN_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformAffineMT_Bilinear_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformAffineMT_Bicubic_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformAffineTiledMT_NN_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformAffineTiledMT_Bilinear_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformAffineTiledMT_Bicubic_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformPerspectiveMT_NN_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformPerspectiveMT_Bilinear_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformPerspectiveMT_Bicubic_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformBezier_NN_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformBezier_Bilinear_SSE_RGBA8.h"
#include "Process/Transform/RGBA8/TransformBezier_Bicubic_SSE_RGBA8.h"
#endif // ULIS_COMPILETIME_SSE_SUPPORT

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Resize Area
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedResizeAreaInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleResizeMT_Area_SSE_RGBA8
        , &ScheduleResizeMT_Area_SSE_RGBA8
        , &ScheduleResizeMT_Area_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedResizeAreaInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// Resize Bicubic
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedResizeBicubicInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleResizeMT_Bicubic_SSE_RGBA8
        , &ScheduleResizeMT_Bicubic_SSE_RGBA8
        , &ScheduleResizeMT_Bicubic_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedResizeBicubicInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// Resize Bilinear
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedResizeBilinearInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleResizeMT_Bilinear_SSE_RGBA8
        , &ScheduleResizeMT_Bilinear_SSE_RGBA8
        , &ScheduleResizeMT_Bilinear_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedResizeBilinearInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// Resize NN
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedResizeNNInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleResizeMT_NN_SSE_RGBA8
        , &ScheduleResizeMT_NN_SSE_RGBA8
        , &ScheduleResizeMT_NN_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedResizeNNInvocationSchedulerSelector )
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
// TransformAffine Bicubic
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineBicubicInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformAffineMT_Bicubic_SSE_RGBA8
        , &ScheduleTransformAffineMT_Bicubic_SSE_RGBA8
        , &ScheduleTransformAffineMT_Bicubic_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineBicubicInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// TransformAffine Bilinear
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineBilinearInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformAffineMT_Bilinear_SSE_RGBA8
        , &ScheduleTransformAffineMT_Bilinear_SSE_RGBA8
        , &ScheduleTransformAffineMT_Bilinear_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineBilinearInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// TransformAffine NN
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineNNInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformAffineMT_NN_SSE_RGBA8
        , &ScheduleTransformAffineMT_NN_SSE_RGBA8
        , &ScheduleTransformAffineMT_NN_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineNNInvocationSchedulerSelector )
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
// TransformAffineTiled Bicubic
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineTiledBicubicInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformAffineTiledMT_Bicubic_SSE_RGBA8
        , &ScheduleTransformAffineTiledMT_Bicubic_SSE_RGBA8
        , &ScheduleTransformAffineTiledMT_Bicubic_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineTiledBicubicInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// TransformAffineTiled Bilinear
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineTiledBilinearInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformAffineTiledMT_Bilinear_SSE_RGBA8
        , &ScheduleTransformAffineTiledMT_Bilinear_SSE_RGBA8
        , &ScheduleTransformAffineTiledMT_Bilinear_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineTiledBilinearInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// TransformAffineTiled NN
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineTiledNNInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformAffineTiledMT_NN_SSE_RGBA8
        , &ScheduleTransformAffineTiledMT_NN_SSE_RGBA8
        , &ScheduleTransformAffineTiledMT_NN_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformAffineTiledNNInvocationSchedulerSelector )
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
// TransformBezier Bicubic
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformBezierBicubicInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformBezierMT_Bicubic_SSE_RGBA8
        , &ScheduleTransformBezierMT_Bicubic_SSE_RGBA8
        , &ScheduleTransformBezierMT_Bicubic_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformBezierBicubicInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// TransformBezier Bilinear
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformBezierBilinearInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformBezierMT_Bilinear_SSE_RGBA8
        , &ScheduleTransformBezierMT_Bilinear_SSE_RGBA8
        , &ScheduleTransformBezierMT_Bilinear_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformBezierBilinearInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// TransformBezier NN
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformBezierNNInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformBezierMT_NN_SSE_RGBA8
        , &ScheduleTransformBezierMT_NN_SSE_RGBA8
        , &ScheduleTransformBezierMT_NN_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformBezierNNInvocationSchedulerSelector )
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
// TransformPerspective Bicubic
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformPerspectiveBicubicInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformPerspectiveMT_Bicubic_SSE_RGBA8
        , &ScheduleTransformPerspectiveMT_Bicubic_SSE_RGBA8
        , &ScheduleTransformPerspectiveMT_Bicubic_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformPerspectiveBicubicInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// TransformPerspective Bilinear
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformPerspectiveBilinearInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformPerspectiveMT_Bilinear_SSE_RGBA8
        , &ScheduleTransformPerspectiveMT_Bilinear_SSE_RGBA8
        , &ScheduleTransformPerspectiveMT_Bilinear_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformPerspectiveBilinearInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// TransformPerspective NN
ULIS_BEGIN_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformPerspectiveNNInvocationSchedulerSelector )
    ULIS_DEFINE_DISPATCHER_SPECIALIZATION(
          &DispatchTestIsUnorderedRGBA8
        , &ScheduleTransformPerspectiveMT_NN_SSE_RGBA8
        , &ScheduleTransformPerspectiveMT_NN_SSE_RGBA8
        , &ScheduleTransformPerspectiveMT_NN_MEM_Generic< uint8 > )
ULIS_END_DISPATCHER_SPECIALIZATION_DEFINITION( FDispatchedTransformPerspectiveNNInvocationSchedulerSelector )
/////////////////////////////////////////////////////
// Bezier Deform Field
void
InvokeProcessBezierDeformField(
      const FSimpleBufferJobArgs* jargs
    , const FProcessBezierDeformFieldArgs* cargs
)
{
    FBlock& field = cargs->dst;
    FBlock& mask = cargs->mask;
    std::vector< FSplineParametricSample > LUTV0;
    std::vector< FSplineParametricSample > LUTV1;
    CubicBezierGenerateLinearLUT( &LUTV0, cargs->points[0].point, cargs->points[0].ctrlCCW, cargs->points[3].ctrlCW, cargs->points[3].point, cargs->threshold );
    CubicBezierGenerateLinearLUT( &LUTV1, cargs->points[1].point, cargs->points[1].ctrlCW, cargs->points[2].ctrlCCW, cargs->points[2].point, cargs->threshold );
    const int max = static_cast< int >( FMath::Max( LUTV0.size(), LUTV1.size() ) );
    const float maxf = static_cast< float >( max );

    for( int i = 0; i < max; ++i ) {
        float v = i / maxf;
        int index_v0 = static_cast< int >( floor( v * LUTV0.size() ) );
        int index_v1 = static_cast< int >( floor( v * LUTV1.size() ) );
        const FVec2F& V0 = LUTV0[index_v0].point;
        const FVec2F& V1 = LUTV1[index_v1].point;
        FVec2F _v0 = V0 + ( cargs->points[0].ctrlCW  - cargs->points[0].point ) * ( 1.f - v ) + ( cargs->points[3].ctrlCCW - cargs->points[3].point ) * v;
        FVec2F _v1 = V1 + ( cargs->points[1].ctrlCCW - cargs->points[1].point ) * ( 1.f - v ) + ( cargs->points[2].ctrlCW  - cargs->points[2].point ) * v;
        float parametricDistortedV = ( LUTV0[index_v0].param + LUTV1[index_v1].param ) / 2.f;
        std::vector< FSplineParametricSample > lutTemp;
        CubicBezierGenerateLinearLUT( &lutTemp, V0, _v0, _v1, V1, cargs->threshold );
        for( int i = 0; i < lutTemp.size(); ++i ) {
            float parametricDistortedU = lutTemp[i].param;
            FVec2F P = lutTemp[i].point;
            int x = static_cast< int >( P.x );
            int y = static_cast< int >( P.y );

            for( uint32 i = 0; i < cargs->plotSize; ++i ) {
                int _u = x + i;
                if( _u < 0 || _u >= mask.Width() )
                    continue;
                for( uint32 j = 0; j < cargs->plotSize; ++j ) {
                    int _v = y + j;
                    if( _v < 0 || _v >= mask.Height() )
                        continue;
                    uint8* maskptr = reinterpret_cast< uint8* >( mask.PixelBits( _u, _v ) );
                    float* fieldptr = reinterpret_cast< float* >( field.PixelBits( _u, _v ) );
                    *maskptr = 0xFF;
                    fieldptr[0] = parametricDistortedU;
                    fieldptr[1] = parametricDistortedV;
                }
            }
        }
    }
}

ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleProcessBezierDeformField, FSimpleBufferJobArgs, FProcessBezierDeformFieldArgs, &InvokeProcessBezierDeformField );
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedProcessBezierDeformFieldSchedulerSelector )

ULIS_NAMESPACE_END

