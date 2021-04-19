// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         NoiseArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for NoiseArgs.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Conv/Conv.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Image/Block.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FNoiseCommandArgs
class FNoiseCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FNoiseCommandArgs() override
    {
    }

    FNoiseCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
    {}
};

ULIS_NAMESPACE_END

