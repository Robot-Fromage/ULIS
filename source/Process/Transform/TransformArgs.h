// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         TransformArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for Transform and Resize Args.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Conv/Conv.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Geometry/Matrix.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FTransformArgs
class FTransformArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FTransformArgs() override
    {
    };

    FTransformArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , eResamplingMethod iResamplingMethod
        , eBorderMode iBorderMode
        , const FColor& iBorderValue
        , const FMat3F& iInverseMatrix
        , bool iTiled = false
    )
        : FDualBufferCommandArgs(
              iSrc
            , iDst
            , iSrcRect
            , iDstRect
            )
        , resamplingMethod( iResamplingMethod )
        , borderMode( iBorderMode )
        , borderValue( iBorderValue )
        , inverseMatrix( iInverseMatrix )
        , tiled( iTiled )
        {}

    eResamplingMethod resamplingMethod;
    eBorderMode borderMode;
    FColor borderValue;
    FMat3F inverseMatrix;
    bool tiled;
};

/////////////////////////////////////////////////////
// FResizeArgs
class FResizeArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FResizeArgs() override
    {
    };

    FResizeArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , eResamplingMethod iResamplingMethod
        , eBorderMode iBorderMode
        , const FColor& iBorderValue
        , const FVec2F& iInverseScale
        , const FVec2F& iShift
        , const FBlock* iOptionalSAT
        , bool iTiled = false
    )
        : FDualBufferCommandArgs(
              iSrc
            , iDst
            , iSrcRect
            , iDstRect
            )
        , resamplingMethod( iResamplingMethod )
        , borderMode( iBorderMode )
        , borderValue( iBorderValue )
        , inverseScale( iInverseScale )
        , shift( iShift )
        , optionalSAT( iOptionalSAT )
        , tiled( iTiled )
        {}

    eResamplingMethod resamplingMethod;
    eBorderMode borderMode;
    FColor borderValue;
    FVec2F inverseScale;
    FVec2F shift;
    const FBlock* optionalSAT;
    bool tiled;

};

/////////////////////////////////////////////////////
// Schedulers
#define ULIS_DEFINE_TRANSFORM_COMMAND_GENERIC( iName )      \
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(  \
    Schedule ## iName                                       \
    , FDualBufferJobArgs                                    \
    , FTransformArgs                                        \
    , &Invoke ## iName ## < T >                             \
    , nullptr                                               \
    , nullptr                                               \
)
#define ULIS_DEFINE_TRANSFORM_COMMAND_SPECIALIZATION( iName )   \
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(              \
    Schedule ## iName                                           \
    , FDualBufferJobArgs                                        \
    , FTransformArgs                                            \
    , &Invoke ## iName                                          \
    , nullptr                                                   \
    , nullptr                                                   \
)

ULIS_NAMESPACE_END

