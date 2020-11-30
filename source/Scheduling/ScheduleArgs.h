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

ULIS_NAMESPACE_END

