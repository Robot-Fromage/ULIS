// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ClearArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FClearCommandArgs structure.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/ScheduleArgs.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FClearCommandArgs
class FClearCommandArgs final
    : public ICommandArgs
{
public:
    ~FClearCommandArgs() override
    {
    };

    FClearCommandArgs(
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
// FClearJobArgs
class FClearJobArgs final
    : public IJobArgs
{
public:

    ~FClearJobArgs() override {};
    FClearJobArgs(
          uint8* const iDst
        , const uint32 iCount
        , const uint32 iStride
    )
        : IJobArgs()
        , dst( iDst )
        , count( iCount )
        , stride( iStride )
    {}

    uint8* const ULIS_RESTRICT dst;
    const uint32 count;
    const uint32 stride;
};

ULIS_NAMESPACE_END

