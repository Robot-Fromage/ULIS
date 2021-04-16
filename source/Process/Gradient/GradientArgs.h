// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BlendArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for BlendArgs.
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
#include "Image/Gradient.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FGradientCommandArgs
class FGradientCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FGradientCommandArgs() override
    {
    }

    FGradientCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , const FVec2F& iStart
        , const FVec2F& iEnd
        , const FSanitizedGradient& iGradient
        , ufloat iDithering
        , eGradientType iType
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , start( iStart )
        , end( iEnd )
        , gradient( iGradient )
        , dithering( iDithering )
        , type( iType )
    {}

    const FVec2F start;
    const FVec2F end;
    const FSanitizedGradient& gradient;
    const ufloat dithering;
    const eGradientType type;
};

ULIS_NAMESPACE_END

