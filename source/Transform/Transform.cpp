// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Transform.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the transform entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Transform/Transform.h"
#include "Transform/TransformArgs.h"
#include "Transform/TransformDispatch.ipp"
#include "Clear/Clear.h"
#include "Data/Block.h"
#include "Maths/Bezier.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Maths/Transform2D_Private.h"
#include "Misc/SummedAreaTable.h"
#include <glm/matrix.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/ext/matrix_transform.hpp>

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// TransformAffine
void TransformAffine( FThreadPool*              iThreadPool
                    , bool                      iBlocking
                    , uint32                    iPerfIntent
                    , const FHostDeviceInfo&    iHostDeviceInfo
                    , bool                      iCallCB
                    , const FBlock*             iSource
                    , FBlock*                   iDestination
                    , const FRect&              iSourceRect
                    , const FTransform2D&       iTransform
                    , eResamplingMethod         iMethod )
{
    // Assertions
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS3_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRect src_fit = iSourceRect & iSource->Rect();
    FRect trans = TransformAffineMetrics( src_fit, iTransform, iMethod );
    FRect dst_fit = trans & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    std::shared_ptr< FTransformArgs > commandArgs = std::make_shared< FTransformArgs >();
    FTransformArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool             = iThreadPool;
    commandArgsRef.blocking         = iBlocking;
    commandArgsRef.hostDeviceInfo   = &iHostDeviceInfo;
    commandArgsRef.perfIntent       = iPerfIntent;
    commandArgsRef.source           = iSource;
    commandArgsRef.destination      = iDestination;
    commandArgsRef.src_roi          = src_fit;
    commandArgsRef.dst_roi          = dst_fit;
    commandArgsRef.method           = iMethod;
    commandArgsRef.inverseTransform = glm::inverse( iTransform.GetImp().Matrix() );

    // Query dispatched method
    fpTransformInvocation dispatchedInvocation = TDispatcher< FTransformAffineInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo(), commandArgsRef );
    dispatchedInvocation( commandArgs );

    // Invalid
    iDestination->Invalidate( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// TransformAffineTiled
void TransformAffineTiled( FThreadPool*              iThreadPool
                         , bool                      iBlocking
                         , uint32                    iPerfIntent
                         , const FHostDeviceInfo&    iHostDeviceInfo
                         , bool                      iCallCB
                         , const FBlock*             iSource
                         , FBlock*                   iDestination
                         , const FRect&              iSourceRect
                         , const FRect&              iDestRect
                         , const FTransform2D&       iTransform
                         , eResamplingMethod         iMethod )
{
    // Assertions
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS3_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRect src_fit = iSourceRect & iSource->Rect();
    FRect dst_fit = iDestRect & iDestination->Rect();

    if( dst_fit.Area() == 0 || src_fit.Area() == 0 )
        return;

    std::shared_ptr< FTransformArgs > commandArgs = std::make_shared< FTransformArgs >();
    FTransformArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool             = iThreadPool;
    commandArgsRef.blocking         = iBlocking;
    commandArgsRef.hostDeviceInfo   = &iHostDeviceInfo;
    commandArgsRef.perfIntent       = iPerfIntent;
    commandArgsRef.source           = iSource;
    commandArgsRef.destination      = iDestination;
    commandArgsRef.src_roi          = src_fit;
    commandArgsRef.dst_roi          = dst_fit;
    commandArgsRef.method           = iMethod;
    commandArgsRef.inverseTransform = glm::inverse( iTransform.GetImp().Matrix() );

    // Query dispatched method
    fpTransformInvocation dispatchedInvocation = TDispatcher< FTransformAffineTiledInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo(), commandArgsRef );
    dispatchedInvocation( commandArgs );


    // Invalid
    iDestination->Invalidate( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// TransformPerspective
void TransformPerspective( FThreadPool*         iThreadPool
                    , bool                      iBlocking
                    , uint32                    iPerfIntent
                    , const FHostDeviceInfo&    iHostDeviceInfo
                    , bool                      iCallCB
                    , const FBlock*             iSource
                    , FBlock*                   iDestination
                    , const FRect&              iSourceRect
                    , const FTransform2D&       iTransform
                    , eResamplingMethod         iMethod )
{
    // Assertions
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS3_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRect src_fit = iSourceRect & iSource->Rect();
    FRect trans = TransformPerspectiveMetrics( src_fit, iTransform, iMethod );
    FRect dst_fit = trans & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    std::shared_ptr< FTransformArgs > commandArgs = std::make_shared< FTransformArgs >();
    FTransformArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool             = iThreadPool;
    commandArgsRef.blocking         = iBlocking;
    commandArgsRef.hostDeviceInfo   = &iHostDeviceInfo;
    commandArgsRef.perfIntent       = iPerfIntent;
    commandArgsRef.source           = iSource;
    commandArgsRef.destination      = iDestination;
    commandArgsRef.src_roi          = src_fit;
    commandArgsRef.dst_roi          = dst_fit;
    commandArgsRef.method           = iMethod;
    commandArgsRef.inverseTransform = glm::inverse( iTransform.GetImp().Matrix() );

    // Query dispatched method
    fpTransformInvocation dispatchedInvocation = TDispatcher< FTransformPerspectiveInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo(), commandArgsRef );
    dispatchedInvocation( commandArgs );

    // Invalid
    iDestination->Invalidate( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// TransformBezier
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
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS3_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iControlPoints.size() == 4,                   "Bad control points size" );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRect src_fit = iSourceRect & iSource->Rect();
    FRect trans = TransformBezierMetrics( src_fit, iControlPoints, iMethod );
    int plotsize = FMaths::Clamp( iPlotSize, 1, 8 );
    trans.w += plotsize;
    trans.h += plotsize;
    FRect dst_fit = trans & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    FVec2F shift( static_cast< float >( trans.x ), static_cast< float >( trans.y ) );
    std::shared_ptr< FBlock > field   = std::make_shared< FBlock >( dst_fit.w, dst_fit.h, ULIS3_FORMAT_GAF );
    std::shared_ptr< FBlock > mask    = std::make_shared< FBlock >( dst_fit.w, dst_fit.h, ULIS3_FORMAT_G8 );
    ClearRaw( mask.get(), ULIS3_NOCB );
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

    std::shared_ptr< FTransformArgs > commandArgs = std::make_shared< FTransformArgs >();
    FTransformArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool             = iThreadPool;
    commandArgsRef.blocking         = iBlocking;
    commandArgsRef.hostDeviceInfo   = &iHostDeviceInfo;
    commandArgsRef.perfIntent       = iPerfIntent;
    commandArgsRef.source           = iSource;
    commandArgsRef.destination      = iDestination;
    commandArgsRef.src_roi          = src_fit;
    commandArgsRef.dst_roi          = dst_fit;
    commandArgsRef.method           = iMethod;
    commandArgsRef.inverseTransform = glm::mat3( 1.f );

    // Query dispatched method
    fpBezierTransformInvocation dispatchedInvocation = TDispatcher< FTransformBezierInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo(), commandArgsRef );
    dispatchedInvocation( commandArgs, field, mask );


    // Invalid
    iDestination->Invalidate( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// Resize
void Resize( FThreadPool*             iThreadPool
           , bool                     iBlocking
           , uint32                   iPerfIntent
           , const FHostDeviceInfo&   iHostDeviceInfo
           , bool                     iCallCB
           , const FBlock*            iSource
           , FBlock*                  iDestination
           , const FRect&             iSourceRect
           , const FVec2F&            iSize
           , const FVec2F&            iPos
           , eResamplingMethod        iMethod ) {
    // Assertions
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS3_ASSERT( iSource->Format() == iDestination->Format(),  "Formats do not match."                                 );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iSize.x > 0.f && iSize.y > 0.f,               "Bad Size." );

    FRect src_fit = iSourceRect & iSource->Rect();
    bool bNeedFix = ( iMethod == INTERP_BILINEAR || iMethod == INTERP_BICUBIC );
    float src_w = static_cast< float >( src_fit.w );
    float src_h = static_cast< float >( src_fit.h );
    float dst_w = FMaths::Max( FMaths::kEpsilonf, iSize.x );
    float dst_h = FMaths::Max( FMaths::kEpsilonf, iSize.y );
    float scale_x = dst_w / src_w;
    float scale_y = dst_h / src_h;
    float fixed_w = bNeedFix && scale_x > 1.f ? dst_w - scale_x : dst_w;
    float fixed_h = bNeedFix && scale_y > 1.f ? dst_h - scale_y : dst_h;
    float fixed_scalex = fixed_w / src_w;
    float fixed_scaley = fixed_h / src_h;
    FVec2F inverseScale = FVec2F( 1.f / fixed_scalex, 1.f / fixed_scaley );
    FVec2F shift = FVec2F( bNeedFix && scale_x > 1.f ? iPos.x + fixed_scalex : iPos.x, bNeedFix && scale_y > 1.f ? iPos.y + fixed_scaley : iPos.y );

    FRect dst_fit = FRect( static_cast< int >( FMaths::RoundToNegativeInfinity( iPos.x ) )
                         , static_cast< int >( FMaths::RoundToNegativeInfinity( iPos.y ) )
                         , static_cast< int >( FMaths::RoundToPositiveInfinity( dst_w ) )
                         , static_cast< int >( FMaths::RoundToPositiveInfinity( dst_h ) ) )
                    & iDestination->Rect();

    if( !dst_fit.Area() )
        return;

    std::shared_ptr< FResizeArgs > commandArgs = std::make_shared< FResizeArgs >();
    FResizeArgs& commandArgsRef = *commandArgs;
    commandArgsRef.pool              = iThreadPool;
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
        std::shared_ptr< FBlock > sh( XGetPremultipliedSummedAreaTable( iThreadPool, ULIS3_BLOCKING, iPerfIntent, iHostDeviceInfo, ULIS3_NOCB, iSource ) );
        commandArgsRef.optionalSAT = sh;
    } else {
        commandArgsRef.optionalSAT = nullptr;
    }

    // Query dispatched method
    fpResizeInvocation dispatchedInvocation = TDispatcher< FResizeInvocationSelector >::Query( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo(), commandArgsRef );
    dispatchedInvocation( commandArgs );

    // Invalid
    iDestination->Invalidate( dst_fit, iCallCB );
}

/////////////////////////////////////////////////////
// XResize
FBlock* XResize( FThreadPool*           iThreadPool
               , bool                   iBlocking
               , uint32                 iPerfIntent
               , const FHostDeviceInfo& iHostDeviceInfo
               , bool                   iCallCB
               , const FBlock*          iSource
               , const FRect&           iSourceRect
               , const FVec2F&          iSize
               , eResamplingMethod      iMethod ) {
    // Assertions
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iSize.x > 0.f && iSize.y > 0.f,               "Bad Size." );

    if( iSize.x <= 0.f || iSize.y <= 0.f )
        return  nullptr;

    FBlock* dst = new FBlock( static_cast< int >( FMaths::RoundToPositiveInfinity( iSize.x ) )
                            , static_cast< int >( FMaths::RoundToPositiveInfinity( iSize.y ) ), iSource->Format() );
    Resize( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, iSourceRect, iSize, FVec2F(), iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// XTransformAffine
FBlock* XTransformAffine( FThreadPool*              iThreadPool
                        , bool                      iBlocking
                        , uint32                    iPerfIntent
                        , const FHostDeviceInfo&    iHostDeviceInfo
                        , bool                      iCallCB
                        , const FBlock*             iSource
                        , const FRect&              iSourceRect
                        , const FTransform2D&       iTransform
                        , eResamplingMethod         iMethod ) {
    // Assertions
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRect src_fit = iSourceRect & iSource->Rect();
    FRect trans = TransformAffineMetrics( src_fit, iTransform, iMethod );
    if( !trans.Area() )
        return  nullptr;

    FBlock* dst = new FBlock( trans.w, trans.h, iSource->Format() );
    FTransform2D fixedTransform( FTransform2D::ComposeTransforms( FTransform2D::MakeTranslationTransform( static_cast< float >( -trans.x ), static_cast< float >( -trans.y ) ), iTransform ) );
    TransformAffine( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, src_fit, fixedTransform, iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// XTransformAffineTiled
FBlock* XTransformAffineTiled( FThreadPool*              iThreadPool
                             , bool                      iBlocking
                             , uint32                    iPerfIntent
                             , const FHostDeviceInfo&    iHostDeviceInfo
                             , bool                      iCallCB
                             , const FBlock*             iSource
                             , const FRect&              iSourceRect
                             , const FRect&              iDestRect
                             , const FTransform2D&       iTransform
                             , eResamplingMethod         iMethod ) {
    // Assertions
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRect src_fit = iSourceRect & iSource->Rect();
    FRect dst_fit = iDestRect;

    if( dst_fit.Area() == 0 || src_fit.Area() == 0 )
        return nullptr;

    FBlock* dst = new FBlock( dst_fit.w, dst_fit.h, iSource->Format() );
    TransformAffineTiled( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, src_fit, dst_fit, iTransform, iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// XMakeTileableTransformedPattern
FBlock* XMakeTileableTransformedPattern( FThreadPool*              iThreadPool
                                       , bool                      iBlocking
                                       , uint32                    iPerfIntent
                                       , const FHostDeviceInfo&    iHostDeviceInfo
                                       , bool                      iCallCB
                                       , const FBlock*             iSource
                                       , const FRect&              iSourceRect
                                       , const FTransform2D&       iTransform
                                       , eResamplingMethod         iMethod ) {
        // Assertions
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRect src_fit = iSourceRect & iSource->Rect();
    FRect dst_fit = TransformAffineMetrics( src_fit, iTransform, INTERP_NN );;

    if( dst_fit.Area() == 0 || src_fit.Area() == 0 )
        return nullptr;

    FBlock* dst = new FBlock( dst_fit.w, dst_fit.h, iSource->Format() );
    TransformAffineTiled( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, src_fit, dst_fit, iTransform, iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// XTransformPerspective
FBlock* XTransformPerspective( FThreadPool*                 iThreadPool
                             , bool                         iBlocking
                             , uint32                       iPerfIntent
                             , const FHostDeviceInfo&       iHostDeviceInfo
                             , bool                         iCallCB
                             , const FBlock*                iSource
                             , const FRect&                 iSourceRect
                             , const std::vector< FVec2F >& iDestinationPoints
                             , eResamplingMethod            iMethod ) {
    // Assertions
    ULIS3_ASSERT( iSource,                          "Bad source."                                           );
    ULIS3_ASSERT( iThreadPool,                      "Bad pool."                                             );
    ULIS3_ASSERT( !iCallCB || iBlocking,            "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iDestinationPoints.size() == 4,   "Bad destination points"                                );

    // Fix AREA not available here
    iMethod = iMethod == INTERP_AREA ? INTERP_BILINEAR : iMethod;

    FRect src_fit = iSourceRect & iSource->Rect();
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

    FTransform2D persp( FTransform2D::GetPerspectiveTransform( sourcePoints.data(), fixedDestinationPoints.data() ) );
    FRect trans = TransformPerspectiveMetrics( src_fit, persp, iMethod );
    if( !trans.Area() )
        return  nullptr;

    FBlock* dst = new FBlock( trans.w, trans.h, iSource->Format() );
    TransformPerspective( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dst, src_fit, persp, iMethod );
    return  dst;
}

/////////////////////////////////////////////////////
// TransformAffineMetrics
FRect TransformAffineMetrics( const FRect&          iSourceRect
                            , const FTransform2D&   iTransform
                            , eResamplingMethod     iMethod )
{
    FRect trans = iSourceRect.TransformedAffine( iTransform );
    if( iMethod == INTERP_BILINEAR || iMethod == INTERP_BICUBIC || iMethod == INTERP_AREA ) {
        float tx, ty, r, sx, sy, skx, sky;
        DecomposeMatrix( iTransform.GetImp().Matrix(), &tx, &ty, &r, &sx, &sy, &skx, &sky );
        float angle = FMaths::Max( abs( cos( r ) ), abs( sin( r ) ) );
        float scale = FMaths::Max( sx, sy );
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
FRect TransformPerspectiveMetrics( const FRect&          iSourceRect
                                 , const FTransform2D&   iTransform
                                 , eResamplingMethod     iMethod )
{
    FRect trans = iSourceRect.TransformedPerspective( iTransform );
    if( iMethod == INTERP_BILINEAR || iMethod == INTERP_BICUBIC || iMethod == INTERP_AREA  ) {
        float tx, ty, r, sx, sy, skx, sky;
        DecomposeMatrix( iTransform.GetImp().Matrix(), &tx, &ty, &r, &sx, &sy, &skx, &sky );
        trans.x -= static_cast< int >( ceil( sx ) );
        trans.y -= static_cast< int >( ceil( sy ) );
        trans.w += static_cast< int >( ceil( sx ) );
        trans.h += static_cast< int >( ceil( sy ) );
    }
    return  trans;
}

/////////////////////////////////////////////////////
// TransformBezierMetrics
FRect TransformBezierMetrics( const FRect&                                    iSourceRect
                            , const std::vector< FBezierCubicControlPoint >&  iControlPoints
                            , eResamplingMethod                               iMethod )
{
    ULIS3_ASSERT( iControlPoints.size() == 4, "Bad control points size" );
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

ULIS3_NAMESPACE_END

