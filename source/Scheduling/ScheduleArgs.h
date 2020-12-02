// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ScheduleArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ICommmandArgs class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      ICommandArgs
/// @brief      The ICommandArgs class provides a virtual base class to implement
///             the arguments objects for each type of operations, used in
///             coordination with a FOldThreadPool and a FCommandQueue.
/// @details    The ICommandArgs does nothing special by itself, it is meant to
///             be used in a polymorphic way.
class ULIS_API ICommandArgs {
public:
    /*! Destructor */
    virtual ~ICommandArgs() = 0;
};

/////////////////////////////////////////////////////
/// @class      IJobArgs
/// @brief      The IJobArgs class provides a virtual base class to implement
///             the arguments objects for each type of operations, used in
///             coordination with a FOldThreadPool and a FCommandQueue.
/// @details    The IJobArgs does nothing special by itself, it is meant to
///             be used in a polymorphic way.
class ULIS_API IJobArgs {
public:
    /*! Destructor */
    virtual ~IJobArgs() = 0;
};

/////////////////////////////////////////////////////
/// @class      FSimpleBufferCommandArgs
/// @brief      The FSimpleBufferCommandArgs class provides a class to implement
///             the arguments objects for operations, used on simple buffers.
/// @details    FSimpleBufferCommandArgs is usually used for simple operations on a
///             single buffer, such as Clear or Fill, for instance.
class FSimpleBufferCommandArgs
    : public ICommandArgs
{
public:
    virtual ~FSimpleBufferCommandArgs() override {}
    FSimpleBufferCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
    )
        : ICommandArgs()
        , block( iBlock )
        , rect( iRect )
    {}

    FBlock& block;
    const FRectI rect;
};

/////////////////////////////////////////////////////
/// @class      FSimpleBufferJobArgs
/// @brief      The FSimpleBufferJobArgs class provides a class to implement
///             the arguments objects for operations, used on simple buffers.
/// @details    FSimpleBufferJobArgs is usually used for simple operations on a
///             single buffer, such as Clear or Fill, for instance.
class FSimpleBufferJobArgs final
    : public IJobArgs
{
public:
    ~FSimpleBufferJobArgs() override {}

    uint8* ULIS_RESTRICT dst;
    int64 size;
};

/////////////////////////////////////////////////////
// Job Building
void
BuildSimpleBufferJob_Scanlines(
      const FSimpleBufferCommandArgs* iCargs
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
    , const int64 iIndex
    , FSimpleBufferJobArgs& oJargs
)
{
    const FFormatMetrics& fmt       = iCargs->block.FormatMetrics();
    uint8* const ULIS_RESTRICT dst  = iCargs->block.Bits() + iCargs->rect.x * fmt.BPP;
    const int64 bps                 = static_cast< int64 >( iCargs->block.BytesPerScanLine() );
    const int64 size                = iCargs->rect.w * fmt.BPP;
    oJargs.dst                      = dst + ( iCargs->rect.y + iIndex ) * bps;
    oJargs.size                     = size;
}

void
BuildSimpleBufferJob_Chunks(
      const FSimpleBufferCommandArgs* iCargs
    , const int64 iSize 
    , const int64 iCount
    , const int64 iOffset
    , const int64 iIndex
    , FSimpleBufferJobArgs& oJargs
)
{
    uint8* const ULIS_RESTRICT dst  = iCargs->block.Bits();
    const int64 btt                 = static_cast< int64 >( iCargs->block.BytesTotal() );
    oJargs.dst                      = dst + iIndex;
    oJargs.size                     = FMath::Min( iOffset + iSize, btt ) - iOffset;
}

template<
      typename TJobArgs
    , typename TCommandArgs
    , void (*TDelegateInvoke)(
          const TJobArgs*
        , const TCommandArgs*
        )
>
void
ScheduleSimpleBufferJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , bool iContiguous
)
{
    const FSimpleBufferCommandArgs* cargs  = dynamic_cast< const FSimpleBufferCommandArgs* >( iCommand->Args() );
    RangeBasedSchedulingBuildJobs<
          FSimpleBufferJobArgs
        , FSimpleBufferCommandArgs
        , TDelegateInvoke
        , BuildSimpleBufferJob_Scanlines
        , BuildSimpleBufferJob_Chunks
    >
    (
          iCommand
        , iPolicy
        , static_cast< int64 >( cargs->block.BytesTotal() )
        , cargs->rect.h
        , iContiguous
    );
}


ULIS_NAMESPACE_END

