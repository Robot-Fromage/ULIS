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
//#include "Process/IO/Disk.h"
//#include "Process/IO/Clipboard.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
void
FContext::LoadFromFile(
      FBlock& ioBlock
    , const std::string& iPath
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

void
FContext::SaveToFile(
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

void
FContext::LoadFromClipboard(
      FBlock& ioBlock
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

void
FContext::SaveToClipboard(
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
FContext::ClipboardHasImage()
{
    return  false;
}

ULIS_NAMESPACE_END

