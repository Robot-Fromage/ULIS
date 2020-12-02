// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ConvArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FConvCommandArgs structure.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/ConvDispatch.h"
#include "Math/Geometry/Rectangle.h"
#include "Scheduling/ScheduleArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FConvCommandArgs
class FConvCommandArgs final
    : public ICommandArgs
{
public:
    ~FConvCommandArgs() override
    {
    };

    FConvCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , const fpConversionInvocation iInvocation
        , const bool iContiguous
    )
        : ICommandArgs()
        , src( iSrc )
        , dst( iDst )
        , srcRect( iSrcRect )
        , dstRect( iDstRect )
        , invocation( iInvocation )
        , contiguous( iContiguous )
        {}

    const FBlock& src;
    FBlock& dst;
    const FRectI srcRect;
    const FRectI dstRect;
    fpConversionInvocation invocation;
    const bool contiguous;
};

/////////////////////////////////////////////////////
// FConvJobArgs
class FConvJobArgs final
    : public IJobArgs
{
public:

    ~FConvJobArgs() override {};
    FConvJobArgs(
          const uint8* const iSrc
        , uint8* const iDst
        , const int64 iSize
    )
        : IJobArgs()
        , src( iSrc )
        , dst( iDst )
        , size( iSize )
    {}

    const uint8* const ULIS_RESTRICT src;
    uint8* const ULIS_RESTRICT dst;
    const int64 size;
};

ULIS_NAMESPACE_END

