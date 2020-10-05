// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         SchedulePolicy.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FSchedulePolicy class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
enum eScheduleRunPolicy : uint8 {
      ScheduleRun_Mono  = 0
    , ScheduleRun_Multi = 1
};

enum eScheduleFlowPolicy : uint8 {
      ScheduleFlow_Blocking = 0
    , ScheduleFlow_NonBlocking = 2
};

enum eScheduleModePolicy : uint8 {
      ScheduleMode_Scanlines = 0
    , ScheduleMode_Chunks = 4
};

enum eScheduleParameterPolicy : uint8 {
      ScheduleParameter_Count = 0
    , ScheduleParameter_Length = 8
};

/////////////////////////////////////////////////////
/// @class      FSchedulePolicy
/// @brief      The FSchedulePolicy class provides a way to control how a task
///             is dispatched and scheduled on a multithreaded system, in
///             coordination with a FOldThreadPool and a FCommandQueue.
/// @details    The FSchedulePolicy specifies wether a task should be
///             multithreaded or not, and if so which policy should be applied.
///             Wether the scheduling is done on a chunk basis, or on a scanline
///             basis, for example.
///
///             \sa FContext
///             \sa FOldThreadPool
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
        , eScheduleFlowPolicy iFlow = ScheduleFlow_Blocking
        , eScheduleModePolicy iMode = ScheduleMode_Scanlines
        , eScheduleParameterPolicy iParam = ScheduleParameter_Count
        , uint32 iValue = 1
    );

    /*! Getter for the Run Policy. */
    eScheduleRunPolicy RunPolicy() const;

    /*! Getter for the Flow Policy. */
    eScheduleFlowPolicy FlowPolicy() const;

    /*! Getter for the Mode Policy. */
    eScheduleModePolicy ModePolicy() const;

    /*! Getter for the Parameter Policy. */
    eScheduleParameterPolicy ParameterPolicy() const;

    /*! Getter for the Parameter Value. */
    uint32 Value() const;

private:
    eScheduleRunPolicy          mRun;
    eScheduleFlowPolicy         mFlow;
    eScheduleModePolicy         mMode;
    eScheduleParameterPolicy    mParameter;
    uint32                      mValue;
};

ULIS_NAMESPACE_END

