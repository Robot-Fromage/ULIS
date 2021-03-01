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
#include "Math/Interpolation/Spline.h"
#include "Math/Interpolation/Bezier.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FTransformCommandArgs
class FTransformCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FTransformCommandArgs() override
    {
    };

    FTransformCommandArgs(
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
// FResizeCommandArgs
class FResizeCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FResizeCommandArgs() override
    {
    };

    FResizeCommandArgs(
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
// FTransformBezierCommandArgs
class FTransformBezierCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FTransformBezierCommandArgs() override
    {
    };

    FTransformBezierCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , const FBlock& iField
        , const FBlock& iMask
        , eResamplingMethod iResamplingMethod
        , eBorderMode iBorderMode
        , const FColor& iBorderValue
    )
        : FDualBufferCommandArgs(
              iSrc
            , iDst
            , iSrcRect
            , iDstRect
            )
        , field( iField )
        , mask( iMask )
        , resamplingMethod( iResamplingMethod )
        , borderMode( iBorderMode )
        , borderValue( iBorderValue )
    {}

    const FBlock& field;
    const FBlock& mask;
    eResamplingMethod resamplingMethod;
    eBorderMode borderMode;
    FColor borderValue;
};

/////////////////////////////////////////////////////
// FTransformBezierCommandArgs
class FProcessBezierDeformFieldArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FProcessBezierDeformFieldArgs() override
    {
    };

    FProcessBezierDeformFieldArgs(
          FBlock& iField
        , FBlock& iMask
        , const FRectI& iRect
        , eResamplingMethod iResamplingMethod
        , eBorderMode iBorderMode
        , const FColor& iBorderValue
    )
        : FSimpleBufferCommandArgs(
              iField
            , iRect
        )
        , mask( iMask )
        , resamplingMethod( iResamplingMethod )
        , borderMode( iBorderMode )
        , borderValue( iBorderValue )
    {}

    FBlock& mask;
    eResamplingMethod resamplingMethod;
    eBorderMode borderMode;
    FColor borderValue;
    TArray< FCubicBezierControlPoint > points;
};

/////////////////////////////////////////////////////
// FTransformJobArgs
class FTransformJobArgs final
    : public IJobArgs
{
public:

    ~FTransformJobArgs() override {};

    const uint8* ULIS_RESTRICT src;
    uint8* ULIS_RESTRICT dst;
    uint32 line;
};

/////////////////////////////////////////////////////
// Builders
static
void
BuildTransformJob_Scanlines(
      const FTransformCommandArgs* iCargs
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
    , const int64 iIndex
    , FTransformJobArgs& oJargs
)
{
    const FFormatMetrics& fmt               = iCargs->src.FormatMetrics();
    const uint8* const ULIS_RESTRICT src    = iCargs->src.Bits();
    uint8* const ULIS_RESTRICT dst          = iCargs->dst.Bits();
    const uint32 src_bps                    = static_cast< uint32 >( iCargs->src.BytesPerScanLine() );
    const uint32 dst_bps                    = static_cast< uint32 >( iCargs->dst.BytesPerScanLine() );
    const uint32 src_decal_x                = iCargs->srcRect.x * iCargs->src.BytesPerPixel();
    const uint32 bdp_decal_x                = iCargs->dstRect.x * fmt.BPP;
    oJargs.src                              = nullptr;
    oJargs.dst                              = dst + ( ( iCargs->dstRect.y + iIndex ) * dst_bps ) + bdp_decal_x;
    oJargs.line                             = static_cast< uint32 >( iIndex );
}

static
void
BuildTransformJob_Chunks(
      const FTransformCommandArgs* iCargs
    , const int64 iSize
    , const int64 iCount
    , const int64 iOffset
    , const int64 iIndex
    , FTransformJobArgs& oJargs
)
{
    ULIS_ASSERT( false, "Chunk Scheduling not available for Transform" );
}

static
void
BuildResizeJob_Scanlines(
      const FResizeCommandArgs* iCargs
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
    , const int64 iIndex
    , FTransformJobArgs& oJargs
)
{
    const FFormatMetrics& fmt               = iCargs->src.FormatMetrics();
    const uint8* const ULIS_RESTRICT src    = iCargs->src.Bits();
    uint8* const ULIS_RESTRICT dst          = iCargs->dst.Bits();
    const uint32 src_bps                    = static_cast< uint32 >( iCargs->src.BytesPerScanLine() );
    const uint32 dst_bps                    = static_cast< uint32 >( iCargs->dst.BytesPerScanLine() );
    const uint32 src_decal_x                = iCargs->srcRect.x * iCargs->src.BytesPerPixel();
    const uint32 bdp_decal_x                = iCargs->dstRect.x * fmt.BPP;
    oJargs.src                              = nullptr;
    oJargs.dst                              = dst + ( ( iCargs->dstRect.y + iIndex ) * dst_bps ) + bdp_decal_x;
    oJargs.line                             = static_cast< uint32 >( iIndex );
}

static
void
BuildResizeJob_Chunks(
      const FResizeCommandArgs* iCargs
    , const int64 iSize
    , const int64 iCount
    , const int64 iOffset
    , const int64 iIndex
    , FTransformJobArgs& oJargs
)
{
    ULIS_ASSERT( false, "Chunk Scheduling not available for Resize" );
}


static
void
BuildBezierJob_Scanlines(
      const FTransformBezierCommandArgs* iCargs
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
    , const int64 iIndex
    , FTransformJobArgs& oJargs
)
{
    const FFormatMetrics& fmt               = iCargs->src.FormatMetrics();
    const uint8* const ULIS_RESTRICT src    = iCargs->src.Bits();
    uint8* const ULIS_RESTRICT dst          = iCargs->dst.Bits();
    const uint32 src_bps                    = static_cast< uint32 >( iCargs->src.BytesPerScanLine() );
    const uint32 dst_bps                    = static_cast< uint32 >( iCargs->dst.BytesPerScanLine() );
    const uint32 src_decal_x                = iCargs->srcRect.x * iCargs->src.BytesPerPixel();
    const uint32 bdp_decal_x                = iCargs->dstRect.x * fmt.BPP;
    oJargs.src                              = nullptr;
    oJargs.dst                              = dst + ( ( iCargs->dstRect.y + iIndex ) * dst_bps ) + bdp_decal_x;
    oJargs.line                             = static_cast< uint32 >( iIndex );
}

static
void
BuildBezierJob_Chunks(
      const FTransformBezierCommandArgs* iCargs
    , const int64 iSize
    , const int64 iCount
    , const int64 iOffset
    , const int64 iIndex
    , FTransformJobArgs& oJargs
)
{
    ULIS_ASSERT( false, "Chunk Scheduling not available for Bezier" );
}

/////////////////////////////////////////////////////
// Schedulers
#define ULIS_DEFINE_TRANSFORM_COMMAND_GENERIC( iName )          \
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(      \
    Schedule ## iName                                           \
    , FTransformJobArgs                                         \
    , FTransformCommandArgs                                     \
    , &Invoke ## iName < T >                                    \
    , &BuildTransformJob_Scanlines                              \
    , &BuildTransformJob_Chunks                                 \
)
#define ULIS_DEFINE_TRANSFORM_COMMAND_SPECIALIZATION( iName )   \
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(              \
    Schedule ## iName                                           \
    , FTransformJobArgs                                         \
    , FTransformCommandArgs                                     \
    , &Invoke ## iName                                          \
    , &BuildTransformJob_Scanlines                              \
    , &BuildTransformJob_Chunks                                 \
)

#define ULIS_DEFINE_RESIZE_COMMAND_GENERIC( iName )         \
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(  \
    Schedule ## iName                                       \
    , FTransformJobArgs                                     \
    , FResizeCommandArgs                                    \
    , &Invoke ## iName < T >                                \
    , &BuildResizeJob_Scanlines                             \
    , &BuildResizeJob_Chunks                                \
)
#define ULIS_DEFINE_RESIZE_COMMAND_SPECIALIZATION( iName )  \
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(          \
    Schedule ## iName                                       \
    , FTransformJobArgs                                     \
    , FResizeCommandArgs                                    \
    , &Invoke ## iName                                      \
    , &BuildResizeJob_Scanlines                             \
    , &BuildResizeJob_Chunks                                \
)

#define ULIS_DEFINE_BEZIER_COMMAND_GENERIC( iName )         \
ULIS_DEFINE_GENERIC_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(  \
    Schedule ## iName                                       \
    , FTransformJobArgs                                     \
    , FTransformBezierCommandArgs                           \
    , &Invoke ## iName < T >                                \
    , &BuildBezierJob_Scanlines                             \
    , &BuildBezierJob_Chunks                                \
)
#define ULIS_DEFINE_BEZIER_COMMAND_SPECIALIZATION( iName )  \
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL_CUSTOM(          \
    Schedule ## iName                                       \
    , FTransformJobArgs                                     \
    , FTransformBezierCommandArgs                           \
    , &Invoke ## iName                                      \
    , &BuildBezierJob_Scanlines                             \
    , &BuildBezierJob_Chunks                                \
)

ULIS_NAMESPACE_END

