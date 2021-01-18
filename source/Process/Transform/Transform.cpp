// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the transform entry point functions.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Transform/Transform.h"
#include "Transform/TransformArgs.h"
#include "Transform/TransformDispatch.h"
#include "Clear/Clear.h"
#include "Image/Block.h"
#include "Math/Interpolation/Bezier.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"
#include "Misc/SummedAreaTable.h"
#include <glm/matrix.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/ext/matrix_transform.hpp>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// TransformAffine
void TransformAffine( FOldThreadPool*              iOldThreadPool
                    , bool                      iBlocking
                    , uint32                    iPerfIntent
                    , const FHardwareMetrics&    iHostDeviceInfo
                    , bool                      iCallCB
                    , const FBlock*             iSource
                    , FBlock*                   iDestination
                    , const FRectI&              iSourceRect
                    , const FTransformation2D&       iTransform
                    , eResamplingMethod         iMethod )
{
    // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRectI src_fit = iSourceRect & iSource->Rect();
    FRectI trans = TransformAffineMetrics( src_fit, iTransform, iMethod );
    FRectI dst_fit = trans & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    std::shared_ptr< FTransformCommandArgs > commandArgs = std::make_shared< FTransformCommandArgs >();
    FTransformCommandArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool             = iOldThreadPool;
    commandArgsRef.blocking         = iBlocking;
    commandArgsRef.hostDeviceInfo   = &iHostDeviceInfo;
    commandArgsRef.perfIntent       = iPerfIntent;
    commandArgsRef.source           = iSource;
    commandArgsRef.destination      = iDestination;
    commandArgsRef.src_roi          = src_fit;
    commandArgsRef.dst_roi          = dst_fit;
    commandArgsRef.method           = iMethod;
    commandArgsRef.inverseTransform = iTransform.InverseMatrix();

    // Query dispatched method
    fpTransformInvocation dispatchedInvocation = TOldDispatcher< FTransformAffineInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatMetrics(), commandArgsRef );
    dispatchedInvocation( commandArgs );

    // Invalid
    iDestination->Dirty( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// TransformAffineTiled
void TransformAffineTiled( FOldThreadPool*              iOldThreadPool
                         , bool                      iBlocking
                         , uint32                    iPerfIntent
                         , const FHardwareMetrics&    iHostDeviceInfo
                         , bool                      iCallCB
                         , const FBlock*             iSource
                         , FBlock*                   iDestination
                         , const FRectI&              iSourceRect
                         , const FRectI&              iDestRect
                         , const FTransformation2D&       iTransform
                         , eResamplingMethod         iMethod )
{
    // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRectI src_fit = iSourceRect & iSource->Rect();
    FRectI dst_fit = iDestRect & iDestination->Rect();

    if( dst_fit.Area() == 0 || src_fit.Area() == 0 )
        return;

    std::shared_ptr< FTransformCommandArgs > commandArgs = std::make_shared< FTransformCommandArgs >();
    FTransformCommandArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool             = iOldThreadPool;
    commandArgsRef.blocking         = iBlocking;
    commandArgsRef.hostDeviceInfo   = &iHostDeviceInfo;
    commandArgsRef.perfIntent       = iPerfIntent;
    commandArgsRef.source           = iSource;
    commandArgsRef.destination      = iDestination;
    commandArgsRef.src_roi          = src_fit;
    commandArgsRef.dst_roi          = dst_fit;
    commandArgsRef.method           = iMethod;
    commandArgsRef.inverseTransform = iTransform.InverseMatrix();

    // Query dispatched method
    fpTransformInvocation dispatchedInvocation = TOldDispatcher< FTransformAffineTiledInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatMetrics(), commandArgsRef );
    dispatchedInvocation( commandArgs );


    // Invalid
    iDestination->Dirty( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// TransformPerspective
void TransformPerspective( FOldThreadPool*         iOldThreadPool
                    , bool                      iBlocking
                    , uint32                    iPerfIntent
                    , const FHardwareMetrics&    iHostDeviceInfo
                    , bool                      iCallCB
                    , const FBlock*             iSource
                    , FBlock*                   iDestination
                    , const FRectI&              iSourceRect
                    , const FTransformation2D&       iTransform
                    , eResamplingMethod         iMethod )
{
    // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRectI src_fit = iSourceRect & iSource->Rect();
    FRectI trans = TransformPerspectiveMetrics( src_fit, iTransform, iMethod );
    FRectI dst_fit = trans & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    std::shared_ptr< FTransformCommandArgs > commandArgs = std::make_shared< FTransformCommandArgs >();
    FTransformCommandArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool             = iOldThreadPool;
    commandArgsRef.blocking         = iBlocking;
    commandArgsRef.hostDeviceInfo   = &iHostDeviceInfo;
    commandArgsRef.perfIntent       = iPerfIntent;
    commandArgsRef.source           = iSource;
    commandArgsRef.destination      = iDestination;
    commandArgsRef.src_roi          = src_fit;
    commandArgsRef.dst_roi          = dst_fit;
    commandArgsRef.method           = iMethod;
    commandArgsRef.inverseTransform = iTransform.InverseMatrix();

    // Query dispatched method
    fpTransformInvocation dispatchedInvocation = TOldDispatcher< FTransformPerspectiveInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatMetrics(), commandArgsRef );
    dispatchedInvocation( commandArgs );

    // Invalid
    iDestination->Dirty( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// TransformBezier
void TransformBezier( FOldThreadPool*                                      iOldThreadPool
                    , bool                                              iBlocking
                    , uint32                                            iPerfIntent
                    , const FHardwareMetrics&                            iHostDeviceInfo
                    , bool                                              iCallCB
                    , const FBlock*                                     iSource
                    , FBlock*                                           iDestination
                    , const FRectI&                                      iSourceRect
                    , const std::vector< FCubicBezierControlPoint >&    iControlPoints
                    , float                                             iThreshold
                    , int                                               iPlotSize
                    , eResamplingMethod                                 iMethod )
{
    // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS_ASSERT( iControlPoints.size() == 4,                   "Bad control points size" );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRectI src_fit = iSourceRect & iSource->Rect();
    FRectI trans = TransformBezierMetrics( src_fit, iControlPoints, iMethod );
    int plotsize = FMath::Clamp( iPlotSize, 1, 8 );
    trans.w += plotsize;
    trans.h += plotsize;
    FRectI dst_fit = trans & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    FVec2F shift( static_cast< float >( trans.x ), static_cast< float >( trans.y ) );
    std::shared_ptr< FBlock > field   = std::make_shared< FBlock >( dst_fit.w, dst_fit.h, eFormat::Format_GAF );
    std::shared_ptr< FBlock > mask    = std::make_shared< FBlock >( dst_fit.w, dst_fit.h, eFormat::Format_G8 );
    ClearRaw( mask.get(), ULIS_NOCB );
    std::vector< FCubicBezierControlPoint > tempPoints;
    tempPoints.reserve( 4 );
    for( auto i : iControlPoints )
        tempPoints.push_back( { i.point - shift, i.ctrlCW - shift, i.ctrlCCW - shift } );
    std::vector< FSplineParametricSample > LUTV0;
    std::vector< FSplineParametricSample > LUTV1;
    CubicBezierGenerateLinearLUT( &LUTV0, tempPoints[0].point, tempPoints[0].ctrlCCW, tempPoints[3].ctrlCW, tempPoints[3].point, iThreshold );
    CubicBezierGenerateLinearLUT( &LUTV1, tempPoints[1].point, tempPoints[1].ctrlCW, tempPoints[2].ctrlCCW, tempPoints[2].point, iThreshold );
    const int max = static_cast< int >( FMath::Max( LUTV0.size(), LUTV1.size() ) );
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
        std::vector< FSplineParametricSample > lutTemp;
        CubicBezierGenerateLinearLUT( &lutTemp, V0, _v0, _v1, V1, iThreshold );
        for( int i = 0; i < lutTemp.size(); ++i ) {
            float parametricDistortedU = lutTemp[i].param;
            FVec2F P = lutTemp[i].point;
            int x = static_cast< int >( P.x );
            int y = static_cast< int >( P.y );

            for( int i = 0; i < plotsize; ++i ) {
                for( int j = 0; j < plotsize; ++j ) {
                    uint8* maskptr = reinterpret_cast< uint8* >( mask->PixelBits( x + i, y + j ) );
                    float* fieldptr = reinterpret_cast< float* >( field->PixelBits( x + i, y + j ) );
                    *maskptr = 0xFF;
                    fieldptr[0] = parametricDistortedU;
                    fieldptr[1] = parametricDistortedV;
                }
            }
        }
    }

    std::shared_ptr< FTransformCommandArgs > commandArgs = std::make_shared< FTransformCommandArgs >();
    FTransformCommandArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool             = iOldThreadPool;
    commandArgsRef.blocking         = iBlocking;
    commandArgsRef.hostDeviceInfo   = &iHostDeviceInfo;
    commandArgsRef.perfIntent       = iPerfIntent;
    commandArgsRef.source           = iSource;
    commandArgsRef.destination      = iDestination;
    commandArgsRef.src_roi          = src_fit;
    commandArgsRef.dst_roi          = dst_fit;
    commandArgsRef.method           = iMethod;
    commandArgsRef.inverseTransform = FMat3F();

    // Query dispatched method
    fpBezierTransformInvocation dispatchedInvocation = TOldDispatcher< FTransformBezierInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatMetrics(), commandArgsRef );
    dispatchedInvocation( commandArgs, field, mask );


    // Invalid
    iDestination->Dirty( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// Resize
void Resize( FOldThreadPool*             iOldThreadPool
           , bool                     iBlocking
           , uint32                   iPerfIntent
           , const FHardwareMetrics&   iHostDeviceInfo
           , bool                     iCallCB
           , const FBlock*            iSource
           , FBlock*                  iDestination
           , const FRectI&             iSourceRect
           , const FVec2F&            iSize
           , const FVec2F&            iPos
           , eResamplingMethod        iMethod ) {
    // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS_ASSERT( iSize.x > 0.f && iSize.y > 0.f,               "Bad Size." );

    FRectI src_fit = iSourceRect & iSource->Rect();
    bool bNeedFix = ( iMethod == INTERP_BILINEAR || iMethod == INTERP_BICUBIC );
    float src_w = static_cast< float >( src_fit.w );
    float src_h = static_cast< float >( src_fit.h );
    float dst_w = FMath::Max( FMath::kEpsilonf, iSize.x );
    float dst_h = FMath::Max( FMath::kEpsilonf, iSize.y );
    float scale_x = dst_w / src_w;
    float scale_y = dst_h / src_h;
    float fixed_w = bNeedFix && scale_x > 1.f ? dst_w - scale_x : dst_w;
    float fixed_h = bNeedFix && scale_y > 1.f ? dst_h - scale_y : dst_h;
    float fixed_scalex = fixed_w / src_w;
    float fixed_scaley = fixed_h / src_h;
    FVec2F inverseScale = FVec2F( 1.f / fixed_scalex, 1.f / fixed_scaley );
    FVec2F shift = FVec2F( bNeedFix && scale_x > 1.f ? iPos.x + fixed_scalex : iPos.x, bNeedFix && scale_y > 1.f ? iPos.y + fixed_scaley : iPos.y );

    FRectI dst_fit = FRectI( static_cast< int >( FMath::RoundToNegativeInfinity( iPos.x ) )
                         , static_cast< int >( FMath::RoundToNegativeInfinity( iPos.y ) )
                         , static_cast< int >( FMath::RoundToPositiveInfinity( dst_w ) )
                         , static_cast< int >( FMath::RoundToPositiveInfinity( dst_h ) ) )
                    & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    std::shared_ptr< FResizeCommandArgs > commandArgs = std::make_shared< FResizeCommandArgs >();
    FResizeCommandArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool              = iOldThreadPool;
    commandArgsRef.blocking          = iBlocking;
    commandArgsRef.hostDeviceInfo    = &iHostDeviceInfo;
    commandArgsRef.perfIntent        = iPerfIntent;
    commandArgsRef.source            = iSource;
    commandArgsRef.destination       = iDestination;
    commandArgsRef.src_roi           = src_fit;
    commandArgsRef.dst_roi           = dst_fit;
    commandArgsRef.method            = iMethod;
    commandArgsRef.inverseScale      = inverseScale;
    commandArgsRef.shift             = shift;

    if( iMethod == INTERP_AREA ) {
        std::shared_ptr< FBlock > sh( XGetPremultipliedSummedAreaTable( iOldThreadPool, ULIS_BLOCKING, iPerfIntent, iHostDeviceInfo, ULIS_NOCB, iSource ) );
        commandArgsRef.optionalSAT = sh;
    } else {
        commandArgsRef.optionalSAT = nullptr;
    }

    // Query dispatched method
    fpResizeInvocation dispatchedInvocation = TOldDispatcher< FResizeInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatMetrics(), commandArgsRef );
    dispatchedInvocation( commandArgs );

    // Invalid
    iDestination->Dirty( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// XResize
FBlock* XResize( FOldThreadPool*           iOldThreadPool
               , bool                   iBlocking
               , uint32                 iPerfIntent
               , const FHardwareMetrics& iHostDeviceInfo
               , bool                   iCallCB
               , const FBlock*          iSource
               , const FRectI&           iSourceRect
               , const FVec2F&          iSize
               , eResamplingMethod      iMethod ) {
    // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS_ASSERT( iSize.x > 0.f && iSize.y > 0.f,               "Bad Size." );

    if( iSize.x <= 0.f || iSize.y <= 0.f )
        return  nullptr;

    FBlock* dst = new FBlock( static_cast< int >( FMath::RoundToPositiveInfinity( iSize.x ) )
                            , static_cast< int >( FMath::RoundToPositiveInfinity( iSize.y ) ), iSource->Format() );
    Resize( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, iSourceRect, iSize, FVec2F(), iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// XTransformAffine
FBlock* XTransformAffine( FOldThreadPool*              iOldThreadPool
                        , bool                      iBlocking
                        , uint32                    iPerfIntent
                        , const FHardwareMetrics&    iHostDeviceInfo
                        , bool                      iCallCB
                        , const FBlock*             iSource
                        , const FRectI&              iSourceRect
                        , const FTransformation2D&       iTransform
                        , eResamplingMethod         iMethod ) {
    // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRectI src_fit = iSourceRect & iSource->Rect();
    FRectI trans = TransformAffineMetrics( src_fit, iTransform, iMethod );
    if( !trans.Area() )
        return  nullptr;

    FBlock* dst = new FBlock( trans.w, trans.h, iSource->Format() );
    FTransformation2D fixedTransform( FMat3F::MakeTranslationMatrix( static_cast< float >( -trans.x ), static_cast< float >( -trans.y ) ) * iTransform.Matrix() );
    TransformAffine( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, src_fit, fixedTransform, iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// XTransformAffineTiled
FBlock* XTransformAffineTiled( FOldThreadPool*              iOldThreadPool
                             , bool                      iBlocking
                             , uint32                    iPerfIntent
                             , const FHardwareMetrics&    iHostDeviceInfo
                             , bool                      iCallCB
                             , const FBlock*             iSource
                             , const FRectI&              iSourceRect
                             , const FRectI&              iDestRect
                             , const FTransformation2D&       iTransform
                             , eResamplingMethod         iMethod ) {
    // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRectI src_fit = iSourceRect & iSource->Rect();
    FRectI dst_fit = iDestRect;

    if( dst_fit.Area() == 0 || src_fit.Area() == 0 )
        return nullptr;

    FBlock* dst = new FBlock( dst_fit.w, dst_fit.h, iSource->Format() );
    TransformAffineTiled( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, src_fit, dst_fit, iTransform, iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// XMakeTileableTransformedPattern
FBlock* XMakeTileableTransformedPattern( FOldThreadPool*              iOldThreadPool
                                       , bool                      iBlocking
                                       , uint32                    iPerfIntent
                                       , const FHardwareMetrics&    iHostDeviceInfo
                                       , bool                      iCallCB
                                       , const FBlock*             iSource
                                       , const FRectI&              iSourceRect
                                       , const FTransformation2D&       iTransform
                                       , eResamplingMethod         iMethod ) {
        // Assertions
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRectI src_fit = iSourceRect & iSource->Rect();
    FRectI dst_fit = TransformAffineMetrics( src_fit, iTransform, INTERP_NN );;

    if( dst_fit.Area() == 0 || src_fit.Area() == 0 )
        return nullptr;

    FBlock* dst = new FBlock( dst_fit.w, dst_fit.h, iSource->Format() );
    TransformAffineTiled( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, src_fit, dst_fit, iTransform, iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// XTransformPerspective
FBlock* XTransformPerspective( FOldThreadPool*                 iOldThreadPool
                             , bool                         iBlocking
                             , uint32                       iPerfIntent
                             , const FHardwareMetrics&       iHostDeviceInfo
                             , bool                         iCallCB
                             , const FBlock*                iSource
                             , const FRectI&                 iSourceRect
                             , const std::vector< FVec2F >& iDestinationPoints
                             , eResamplingMethod            iMethod ) {
    // Assertions
    ULIS_ASSERT( iSource,                          "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,                      "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,            "Callback flag is specified on non-blocking operation." );
    ULIS_ASSERT( iDestinationPoints.size() == 4,   "Bad destination points"                                );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRectI src_fit = iSourceRect & iSource->Rect();
    std::vector< FVec2F > sourcePoints = { FVec2F( 0, 0 ), FVec2F( src_fit.w, 0 ), FVec2F( src_fit.w, src_fit.h ), FVec2F( 0, src_fit.h ) };
    int minx = INT_MAX;
    int miny = INT_MAX;
    for( auto& it : iDestinationPoints ) {
        if( it.x < minx ) minx = static_cast< int >( it.x );
        if( it.y < miny ) miny = static_cast< int >( it.y );
    }
    std::vector< FVec2F > fixedDestinationPoints;
    for( auto& it : iDestinationPoints )
        fixedDestinationPoints.push_back( FVec2F( it.x - minx, it.y - miny ) );

    FTransformation2D persp( FMat3F::MakeHomography( sourcePoints.data(), fixedDestinationPoints.data() ) );
    FRectI trans = TransformPerspectiveMetrics( src_fit, persp, iMethod );
    if( !trans.Area() )
        return  nullptr;

    FBlock* dst = new FBlock( trans.w, trans.h, iSource->Format() );
    TransformPerspective( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, src_fit, persp, iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// TransformAffineMetrics
FRectI TransformAffineMetrics( const FRectI&          iSourceRect
                            , const FTransformation2D&   iTransform
                            , eResamplingMethod     iMethod )
{
    FRectI trans = iSourceRect.TransformedAffine( iTransform );
    if( iMethod == INTERP_BILINEAR || iMethod == INTERP_BICUBIC || iMethod == INTERP_AREA ) {
        float tx, ty, r, sx, sy, skx, sky;
        iTransform.Matrix().Decompose( &tx, &ty, &r, &sx, &sy, &skx, &sky );
        float angle = FMath::Max( abs( cos( r ) ), abs( sin( r ) ) );
        float scale = FMath::Max( sx, sy );
        int overflow = static_cast< int >( ceil( angle * scale ) );
        trans.x -= overflow;
        trans.y -= overflow;
        trans.w += overflow * 2;
        trans.h += overflow * 2;
    }
    return  trans;
}

/////////////////////////////////////////////////////
// TransformPerspectiveMetrics
FRectI TransformPerspectiveMetrics( const FRectI&          iSourceRect
                                 , const FTransformation2D&   iTransform
                                 , eResamplingMethod     iMethod )
{
    FRectI trans = iSourceRect.TransformedPerspective( iTransform );
    if( iMethod == INTERP_BILINEAR || iMethod == INTERP_BICUBIC || iMethod == INTERP_AREA  ) {
        float tx, ty, r, sx, sy, skx, sky;
        iTransform.Matrix().Decompose( &tx, &ty, &r, &sx, &sy, &skx, &sky );
        trans.x -= static_cast< int >( ceil( sx ) );
        trans.y -= static_cast< int >( ceil( sy ) );
        trans.w += static_cast< int >( ceil( sx ) );
        trans.h += static_cast< int >( ceil( sy ) );
    }
    return  trans;
}

/////////////////////////////////////////////////////
// TransformBezierMetrics
FRectI TransformBezierMetrics( const FRectI&                                    iSourceRect
                            , const std::vector< FCubicBezierControlPoint >&  iControlPoints
                            , eResamplingMethod                               iMethod )
{
    ULIS_ASSERT( iControlPoints.size() == 4, "Bad control points size" );
    return  FRectI::FromMinMax( static_cast< int >( FMath::VMin( iControlPoints[0].point.x, iControlPoints[0].ctrlCW.x, iControlPoints[0].ctrlCCW.x
                                                               , iControlPoints[1].point.x, iControlPoints[1].ctrlCW.x, iControlPoints[1].ctrlCCW.x
                                                               , iControlPoints[2].point.x, iControlPoints[2].ctrlCW.x, iControlPoints[2].ctrlCCW.x
                                                               , iControlPoints[3].point.x, iControlPoints[3].ctrlCW.x, iControlPoints[3].ctrlCCW.x ) )
                             , static_cast< int >( FMath::VMin( iControlPoints[0].point.y, iControlPoints[0].ctrlCW.y, iControlPoints[0].ctrlCCW.y
                                                               , iControlPoints[1].point.y, iControlPoints[1].ctrlCW.y, iControlPoints[1].ctrlCCW.y
                                                               , iControlPoints[2].point.y, iControlPoints[2].ctrlCW.y, iControlPoints[2].ctrlCCW.y
                                                               , iControlPoints[3].point.y, iControlPoints[3].ctrlCW.y, iControlPoints[3].ctrlCCW.y ) )
                             , static_cast< int >( FMath::VMax( iControlPoints[0].point.x, iControlPoints[0].ctrlCW.x, iControlPoints[0].ctrlCCW.x
                                                               , iControlPoints[1].point.x, iControlPoints[1].ctrlCW.x, iControlPoints[1].ctrlCCW.x
                                                               , iControlPoints[2].point.x, iControlPoints[2].ctrlCW.x, iControlPoints[2].ctrlCCW.x
                                                               , iControlPoints[3].point.x, iControlPoints[3].ctrlCW.x, iControlPoints[3].ctrlCCW.x ) ) 
                             , static_cast< int >( FMath::VMax( iControlPoints[0].point.y, iControlPoints[0].ctrlCW.y, iControlPoints[0].ctrlCCW.y
                                                               , iControlPoints[1].point.y, iControlPoints[1].ctrlCW.y, iControlPoints[1].ctrlCCW.y
                                                               , iControlPoints[2].point.y, iControlPoints[2].ctrlCW.y, iControlPoints[2].ctrlCCW.y
                                                               , iControlPoints[3].point.y, iControlPoints[3].ctrlCW.y, iControlPoints[3].ctrlCCW.y ) ) );
}

ULIS_NAMESPACE_END

