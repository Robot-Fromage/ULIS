// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Disk.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the disk IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/Dispatcher.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FDiskIOCommandArgs
class FDiskIOCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FDiskIOCommandArgs() override
    {
    }

    FDiskIOCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , const std::string& iPath
        , const eFileFormat iFileFormat = eFileFormat::FileFormat_png
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , path( iPath )
        , fileFormat( iFileFormat )
    {}

    const std::string path;
    const eFileFormat fileFormat;
};

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleLoadFromFile_MEM_Generic )
ULIS_DECLARE_COMMAND_SCHEDULER( ScheduleSaveToFile_MEM_Generic )

ULIS_DECLARE_DISPATCHER( FDispatchedLoadFromFileInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedLoadFromFileInvocationSchedulerSelector
    , &ScheduleLoadFromFile_MEM_Generic< T >
)

ULIS_DECLARE_DISPATCHER( FDispatchedSaveToFileInvocationSchedulerSelector )
ULIS_DEFINE_DISPATCHER_GENERIC_GROUP_MONO(
      FDispatchedSaveToFileInvocationSchedulerSelector
    , &ScheduleSaveToFile_MEM_Generic< T >
)

ULIS_NAMESPACE_END

