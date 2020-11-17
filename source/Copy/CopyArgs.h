// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         CopyArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FCopyCommandArgs structure.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Copy/CopyDispatch.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/ScheduleArgs.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FCopyCommandArgs
class FCopyCommandArgs final
    : public ICommandArgs
{
public:
    ~FCopyCommandArgs() override
    {
    };

    FCopyCommandArgs(
          const FBlock& iSource
        , FBlock& iDestination
        , const FRectI& iSourceRect
        , const FRectI& iDestinationRect
        , const bool iContiguous
    )
        : ICommandArgs()
        , source( iSource )
        , destination( iDestination )
        , sourceRect( iSourceRect )
        , destinationRect( iDestinationRect )
        , contiguous( iContiguous )
        {}

    const FBlock& source;
    FBlock& destination;
    const FRectI sourceRect;
    const FRectI destinationRect;
    const bool contiguous;
};

/////////////////////////////////////////////////////
// FCopyJobArgs
class FCopyJobArgs final
    : public IJobArgs
{
public:

    ~FCopyJobArgs() override {};
    FCopyJobArgs(
          uint8* const iDst
        , const int64 iSize
    )
        : IJobArgs()
        , dst( iDst )
        , size( iSize )
    {}

    uint8* const ULIS_RESTRICT dst;
    const int64 size;
};
ULIS_NAMESPACE_END

