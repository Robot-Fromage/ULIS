// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ScheduleArgs.h
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the declaration for the ICommmandArgs class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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
///             coordination with a FThreadPool and a FCommandQueue.
/// @details    The ICommandArgs does nothing special by itself, it is meant to
///             be used in a polymorphic way.
class ICommandArgs {
public:
    /*! Destructor */
    virtual ~ICommandArgs() = 0;

    ICommandArgs(
        const FRectI& iDstRect
    )
        : dstRect( iDstRect )
    {}

    const FRectI dstRect;
};

/////////////////////////////////////////////////////
/// @class      IJobArgs
/// @brief      The IJobArgs class provides a virtual base class to implement
///             the arguments objects for each type of operations, used in
///             coordination with a FThreadPool and a FCommandQueue.
/// @details    The IJobArgs does nothing special by itself, it is meant to
///             be used in a polymorphic way.
class IJobArgs {
public:
    /*! Destructor */
    virtual ~IJobArgs() = 0;
};

ULIS_NAMESPACE_END

