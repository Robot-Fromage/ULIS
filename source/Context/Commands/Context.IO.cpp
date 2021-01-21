// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.IO.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the IO API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/IO/Disk.h"
//#include "Process/IO/Clipboard.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

#include <filesystem>
namespace fs = std::filesystem;

ULIS_NAMESPACE_BEGIN
ulError
FContext::FileLoad(
      FBlock& ioBlock
    , const std::string& iPath
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

ulError
FContext::FileSave(
      const FBlock& iBlock
    , const std::string& iPath
    , eFileFormat iFileFormat
    , int iQuality
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

ulError
FContext::ClipboardLoad(
      FBlock& ioBlock
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

ulError
FContext::ClipboardSave(
      const FBlock& iBlock
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

//static
bool
ClipboardCanBeLoadedFrom()
{
    return  false;
}

//static
bool
ClipboardCanBeSavedTo()
{
    return  false;
}

//static
bool
FileCanBeLoadedFrom()
{
    return  false;
}

//static
bool
FileCanBeSavedTo()
{
    return  false;
}

ULIS_NAMESPACE_END

