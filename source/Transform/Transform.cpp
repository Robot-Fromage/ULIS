// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the transform entry point functions.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Transform/Transform.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Maths/Transform2D.h"
#include "Transform/Dispatch/TransformInfo.h"
#include "Transform/Dispatch/Dispatch.ipp"
#include "Clear/Clear.h"

#include <glm/matrix.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/ext/matrix_transform.hpp>

ULIS2_NAMESPACE_BEGIN
void TransformAffine( FThreadPool*            iThreadPool
                    , bool                    iBlocking
                    , uint32                  iPerfIntent
                    , const FHostDeviceInfo&  iHostDeviceInfo
                    , bool                    iCallCB
                    , const FBlock*           iSource
                    , FBlock*                 iDestination
                    , const FRect&            iSourceRect
                    , const FTransform2D&     iTransform
                    , eResamplingMethod       iMethod )
{
    // Assertions
    ULIS2_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS2_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS2_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    FRect src_fit = iSourceRect & iSource->Rect();
    FRect trans = TransformAffineMetrics( src_fit, iTransform, iMethod );
    FRect dst_fit = trans & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    std::shared_ptr< _FTransformInfoPrivate > forwardTransformParams = std::make_shared< _FTransformInfoPrivate >();
    _FTransformInfoPrivate& alias = *forwardTransformParams;
    alias.pool              = iThreadPool;
    alias.blocking          = iBlocking;
    alias.hostDeviceInfo    = &iHostDeviceInfo;
    alias.perfIntent        = iPerfIntent;
    alias.source            = iSource;
    alias.destination       = iDestination;
    alias.src_roi           = src_fit;
    alias.dst_roi           = dst_fit;
    alias.method            = iMethod;
    alias.inverseTransform  = glm::inverse( iTransform.Matrix() );

    // Query dispatched method
    fpDispatchedTransformFunc fptr = QueryDispatchedTransformAffineFunctionForParameters( alias );
    ULIS2_ASSERT( fptr, "No dispatch function found." );
    fptr( forwardTransformParams );

    // Invalid
    iDestination->Invalidate( dst_fit, iCallCB );
}

void TransformPerspective( FThreadPool*            iThreadPool
                    , bool                    iBlocking
                    , uint32                  iPerfIntent
                    , const FHostDeviceInfo&  iHostDeviceInfo
                    , bool                    iCallCB
                    , const FBlock*           iSource
                    , FBlock*                 iDestination
                    , const FRect&            iSourceRect
                    , const FTransform2D&     iTransform
                    , eResamplingMethod       iMethod )
{
    // Assertions
    ULIS2_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS2_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS2_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    FRect src_fit = iSourceRect & iSource->Rect();
    FRect trans = TransformPerspectiveMetrics( src_fit, iTransform, iMethod );
    FRect dst_fit = trans & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    std::shared_ptr< _FTransformInfoPrivate > forwardTransformParams = std::make_shared< _FTransformInfoPrivate >();
    _FTransformInfoPrivate& alias = *forwardTransformParams;
    alias.pool              = iThreadPool;
    alias.blocking          = iBlocking;
    alias.hostDeviceInfo    = &iHostDeviceInfo;
    alias.perfIntent        = iPerfIntent;
    alias.source            = iSource;
    alias.destination       = iDestination;
    alias.src_roi           = src_fit;
    alias.dst_roi           = dst_fit;
    alias.method            = iMethod;
    alias.inverseTransform  = glm::inverse( iTransform.Matrix() );

    // Query dispatched method
    fpDispatchedTransformFunc fptr = QueryDispatchedTransformPerspectiveFunctionForParameters( alias );
    ULIS2_ASSERT( fptr, "No dispatch function found." );
    fptr( forwardTransformParams );

    // Invalid
    iDestination->Invalidate( dst_fit, iCallCB );
}

void TransformBezier( FThreadPool*                                      iThreadPool
                    , bool                                              iBlocking
                    , uint32                                            iPerfIntent
                    , const FHostDeviceInfo&                            iHostDeviceInfo
                    , bool                                              iCallCB
                    , const FBlock*                                     iSource
                    , FBlock*                                           iDestination
                    , const FRect&                                      iSourceRect
                    , const std::vector< FBezierCubicControlPoint >&    iControlPoints
                    , float                                             iThreshold
                    , int                                               iPlotSize
                    , eResamplingMethod                                 iMethod )
{
    // Assertions
    ULIS2_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS2_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS2_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS2_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS2_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS2_ASSERT( iControlPoints.size() == 4,                   "Bad control points size" );
    FRect src_fit = iSourceRect & iSource->Rect();
    FRect trans = TransformBezierMetrics( src_fit, iControlPoints, iMethod );
    int plotsize = FMaths::Clamp( iPlotSize, 1, 8 );
    trans.w += plotsize;
    trans.h += plotsize;
    FRect dst_fit = trans & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    FVec2F shift( static_cast< float >( trans.x ), static_cast< float >( trans.y ) );
    std::shared_ptr< FBlock > field   = std::make_shared< FBlock >( dst_fit.w, dst_fit.h, ULIS2_FORMAT_GAF );
    std::shared_ptr< FBlock > mask    = std::make_shared< FBlock >( dst_fit.w, dst_fit.h, ULIS2_FORMAT_G8 );
    ClearRaw( mask.get(), ULIS2_NOCB );
    std::vector< FBezierCubicControlPoint > tempPoints;
    tempPoints.reserve( 4 );
    for( auto i : iControlPoints )
        tempPoints.push_back( { i.point - shift, i.ctrlCW - shift, i.ctrlCCW - shift } );
    std::vector< FBezierLUTElement > LUTV0;
    std::vector< FBezierLUTElement > LUTV1;
    CubicBezierGenerateLinearLUT( &LUTV0, tempPoints[0].point, tempPoints[0].ctrlCCW, tempPoints[3].ctrlCW, tempPoints[3].point, iThreshold );
    CubicBezierGenerateLinearLUT( &LUTV1, tempPoints[1].point, tempPoints[1].ctrlCW, tempPoints[2].ctrlCCW, tempPoints[2].point, iThreshold );
    const int max = static_cast< int >( FMaths::Max( LUTV0.size(), LUTV1.size() ) );
    const float maxf = static_cast< float >( max );
    for( int i = 0; i < max; ++i ) {
        float v = i / maxf;
        int index_v0 = static_cast< int >( floor( v * LUTV0.size() ) );
        int index_v1 = static_cast< int >( floor( v * LUTV1.size() ) );
        const FVec2F& V0 = LUTV0[index_v0].point;
        const FVec2F& V1 = LUTV1[index_v1].point;
        FVec2F _v0 = V0 + ( tempPoints[0].ctrlCW  - tempPoints[0].point ) * ( 1.f - v ) + ( tempPoints[3].ctrlCCW - tempPoints[3].point ) * v;
        FVec2F _v1 = V1 + ( tempPoints[1].ctrlCCW - tempPoints[1].point ) * ( 1.f - v ) + ( tempPoints[2].ctrlCW  - tempPoints[2].point ) * v;
        float parametricDistortedV = ( LUTV0[index_v0].param + LUTV1[index_v0].param ) / 2.f;
        std::vector< FBezierLUTElement > lutTemp;
        CubicBezierGenerateLinearLUT( &lutTemp, V0, _v0, _v1, V1, iThreshold );
        for( int i = 0; i < lutTemp.size(); ++i ) {
            float parametricDistortedU = lutTemp[i].param;
            FVec2F P = lutTemp[i].point;
            int x = static_cast< int >( P.x );
            int y = static_cast< int >( P.y );

            for( int i = 0; i < plotsize; ++i ) {
                for( int j = 0; j < plotsize; ++j ) {
                    uint8* maskptr = reinterpret_cast< uint8* >( mask->PixelPtr( x + i, y + j ) );
                    float* fieldptr = reinterpret_cast< float* >( field->PixelPtr( x + i, y + j ) );
                    *maskptr = 0xFF;
                    fieldptr[0] = parametricDistortedU;
                    fieldptr[1] = parametricDistortedV;
                }
            }
        }
    }

    std::shared_ptr< _FTransformInfoPrivate > forwardTransformParams = std::make_shared< _FTransformInfoPrivate >();
    _FTransformInfoPrivate& alias = *forwardTransformParams;
    alias.pool              = iThreadPool;
    alias.blocking          = iBlocking;
    alias.hostDeviceInfo    = &iHostDeviceInfo;
    alias.perfIntent        = iPerfIntent;
    alias.source            = iSource;
    alias.destination       = iDestination;
    alias.src_roi           = src_fit;
    alias.dst_roi           = dst_fit;
    alias.method            = iMethod;
    alias.inverseTransform  = glm::mat3( 1.f );

    // Query dispatched method
    fpDispatchedBezierTransformFunc fptr = QueryDispatchedTransformBezierFunctionForParameters( alias );
    ULIS2_ASSERT( fptr, "No dispatch function found." );
    fptr( forwardTransformParams, field, mask );

    // Invalid
    iDestination->Invalidate( dst_fit, iCallCB );
}

FRect TransformAffineMetrics( const FRect&          iSourceRect
                            , const FTransform2D&   iTransform
                            , eResamplingMethod     iMethod )
{
    FRect trans = iSourceRect.TransformedAffine( iTransform );
    if( iMethod > INTERP_NN ) {
        float tx, ty, r, sx, sy, skx, sky;
        DecomposeMatrix( iTransform.Matrix(), &tx, &ty, &r, &sx, &sy, &skx, &sky );
        trans.x -= static_cast< int >( ceil( sx ) );
        trans.y -= static_cast< int >( ceil( sy ) );
        trans.w += static_cast< int >( ceil( sx ) );
        trans.h += static_cast< int >( ceil( sy ) );
    }
    return  trans;
}


FRect TransformPerspectiveMetrics( const FRect&          iSourceRect
                                 , const FTransform2D&   iTransform
                                 , eResamplingMethod     iMethod )
{
    FRect trans = iSourceRect.TransformedPerspective( iTransform );
    if( iMethod > INTERP_NN ) {
        float tx, ty, r, sx, sy, skx, sky;
        DecomposeMatrix( iTransform.Matrix(), &tx, &ty, &r, &sx, &sy, &skx, &sky );
        trans.x -= static_cast< int >( ceil( sx ) );
        trans.y -= static_cast< int >( ceil( sy ) );
        trans.w += static_cast< int >( ceil( sx ) );
        trans.h += static_cast< int >( ceil( sy ) );
    }
    return  trans;
}

FRect TransformBezierMetrics( const FRect&                                    iSourceRect
                            , const std::vector< FBezierCubicControlPoint >&  iControlPoints
                            , eResamplingMethod                               iMethod )
{
    ULIS2_ASSERT( iControlPoints.size() == 4, "Bad control points size" );
    return  FRect::FromMinMax( static_cast< int >( FMaths::VMin( iControlPoints[0].point.x, iControlPoints[0].ctrlCW.x, iControlPoints[0].ctrlCCW.x
                                                               , iControlPoints[1].point.x, iControlPoints[1].ctrlCW.x, iControlPoints[1].ctrlCCW.x
                                                               , iControlPoints[2].point.x, iControlPoints[2].ctrlCW.x, iControlPoints[2].ctrlCCW.x
                                                               , iControlPoints[3].point.x, iControlPoints[3].ctrlCW.x, iControlPoints[3].ctrlCCW.x ) )
                             , static_cast< int >( FMaths::VMin( iControlPoints[0].point.y, iControlPoints[0].ctrlCW.y, iControlPoints[0].ctrlCCW.y
                                                               , iControlPoints[1].point.y, iControlPoints[1].ctrlCW.y, iControlPoints[1].ctrlCCW.y
                                                               , iControlPoints[2].point.y, iControlPoints[2].ctrlCW.y, iControlPoints[2].ctrlCCW.y
                                                               , iControlPoints[3].point.y, iControlPoints[3].ctrlCW.y, iControlPoints[3].ctrlCCW.y ) )
                             , static_cast< int >( FMaths::VMax( iControlPoints[0].point.x, iControlPoints[0].ctrlCW.x, iControlPoints[0].ctrlCCW.x
                                                               , iControlPoints[1].point.x, iControlPoints[1].ctrlCW.x, iControlPoints[1].ctrlCCW.x
                                                               , iControlPoints[2].point.x, iControlPoints[2].ctrlCW.x, iControlPoints[2].ctrlCCW.x
                                                               , iControlPoints[3].point.x, iControlPoints[3].ctrlCW.x, iControlPoints[3].ctrlCCW.x ) ) 
                             , static_cast< int >( FMaths::VMax( iControlPoints[0].point.y, iControlPoints[0].ctrlCW.y, iControlPoints[0].ctrlCCW.y
                                                               , iControlPoints[1].point.y, iControlPoints[1].ctrlCW.y, iControlPoints[1].ctrlCCW.y
                                                               , iControlPoints[2].point.y, iControlPoints[2].ctrlCW.y, iControlPoints[2].ctrlCCW.y
                                                               , iControlPoints[3].point.y, iControlPoints[3].ctrlCW.y, iControlPoints[3].ctrlCCW.y ) ) );
}

ULIS2_NAMESPACE_END

