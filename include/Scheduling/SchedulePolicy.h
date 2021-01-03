// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SchedulePolicy.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FSchedulePolicy class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
enum eScheduleRunPolicy : uint8 {
      ScheduleRun_Mono  = 0
    , ScheduleRun_Multi = 1
};

enum eScheduleModePolicy : uint8 {
      ScheduleMode_Scanlines = 0
    , ScheduleMode_Chunks = 1
};

enum eScheduleParameterPolicy : uint8 {
      ScheduleParameter_Count = 0
    , ScheduleParameter_Length = 1
};

/////////////////////////////////////////////////////
/// @class      FSchedulePolicy
/// @brief      The FSchedulePolicy class provides a way to control how a task
///             is dispatched and scheduled on a multithreaded system, in
///             coordination with a FThreadPool and a FCommandQueue.
/// @details    The FSchedulePolicy specifies wether a task should be
///             multithreaded or not, and if so which policy should be applied.
///             Wether the scheduling is done on a chunk basis, or on a scanline
///             basis, for example.
///
///             \sa FContext
///             \sa FThreadPool
///             \sa FHardwareMetrics
///             \sa FCommandQueue
class ULIS_API FSchedulePolicy
{
public:
    /*! Destructor */
    ~FSchedulePolicy();

    /*!
        Constructor with default values.
        Defaults to mono scanline run.
    */
    FSchedulePolicy(
          eScheduleRunPolicy iRun = ScheduleRun_Mono
        , eScheduleModePolicy iMode = ScheduleMode_Scanlines
        , eScheduleParameterPolicy iParam = ScheduleParameter_Count
        , int64 iValue = -1
    );

    /*! Getter for the Run Policy. */
    eScheduleRunPolicy RunPolicy() const;

    /*! Getter for the Mode Policy. */
    eScheduleModePolicy ModePolicy() const;

    /*! Getter for the Parameter Policy. */
    eScheduleParameterPolicy ParameterPolicy() const;

    /*! Getter for the Parameter Value. */
    int64 Value() const;

private:
    eScheduleRunPolicy          mRun;
    eScheduleModePolicy         mMode;
    eScheduleParameterPolicy    mParameter;
    int64                       mValue;
};

ULIS_NAMESPACE_END

