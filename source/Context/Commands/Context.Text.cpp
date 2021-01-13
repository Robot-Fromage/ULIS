// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Text.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the Text API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

ULIS_NAMESPACE_BEGIN
void
FContext::RasterText(
      FBlock& iBlock
    , const std::wstring& iText
    , const FFont& iFont
    , uint32 iFontSize
    , const FMat3F& iTransform
    , const ISample& iColor
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

void
FContext::RasterTextAA(
      FBlock& iBlock
    , const std::wstring& iText
    , const FFont& iFont
    , uint32 iFontSize
    , const FMat3F& iTransform
    , const ISample& iColor
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
}

//static
FRectI
FContext::TextMetrics(
      const std::wstring& iText
    , const FFont& iFont
    , uint32 iFontSize
    , const FMat3F& iTransform
)
{
    return  FRectI();
}

ULIS_NAMESPACE_END

