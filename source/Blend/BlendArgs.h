// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS
*__________________
*
* @file         BlendArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FBlendCommandArgs structure.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Blend/Modes.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/ScheduleArgs.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FBlendCommandArgs
class FBlendCommandArgs final
    : public ICommandArgs
{
public:
    struct FArgs {
        const FBlock& source;
        FBlock& backdrop;
        FRectI sourceRect;
        FVec2F subpixelComponent;
        FVec2F buspixelComponent;
        eBlendMode blendingMode;
        eAlphaMode alphaMode;
        ufloat opacity;
        FVec2I shift;
        FVec2I backdropCoverage;
        FRectI backdropWorkingRect;
    } bulk;

    virtual ~FBlendCommandArgs() override {};
    FBlendCommandArgs( const FArgs& ) = delete;
    FBlendCommandArgs( FArgs&& iInit ) : bulk( std::forward< FArgs >( iInit ) ) {}
};


/////////////////////////////////////////////////////
// FBlendJobArgs
class FBlendJobArgs final
    : public IJobArgs
{
public:
    struct FArgs {
        const uint32 line;
        const uint32 src_bps;
        const uint8* src;
        uint8* bdp;
    } bulk;

    virtual ~FBlendJobArgs() override {};
    FBlendJobArgs( const FArgs& ) = delete;
    FBlendJobArgs( FArgs&& iInit ) : bulk( std::forward< FArgs >( iInit ) ) {}
};

ULIS_NAMESPACE_END

