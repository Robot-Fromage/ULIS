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
    ~FBlendCommandArgs() override {};
    FBlendCommandArgs(
          const FBlock& iSource
        , FBlock& iBackdrop
        , FRectI iSourceRect
        , FVec2F iSubpixelComponent
        , FVec2F iBuspixelComponent
        , eBlendMode iBlendingMode
        , eAlphaMode iAlphaMode
        , ufloat iOpacity
        , FVec2I iShift
        , FVec2I iBackdropCoverage
        , FRectI iBackdropWorkingRect
    )
        : ICommandArgs()
        , source( iSource )
        , backdrop( iBackdrop )
        , sourceRect( iSourceRect )
        , subpixelComponent( iSubpixelComponent )
        , buspixelComponent( iBuspixelComponent )
        , blendingMode( iBlendingMode )
        , alphaMode( iAlphaMode )
        , opacity( iOpacity )
        , shift( iShift )
        , backdropCoverage( iBackdropCoverage )
        , backdropWorkingRect( iBackdropWorkingRect )
        {}

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
};

/////////////////////////////////////////////////////
// FBlendJobArgs
class FBlendJobArgs final
    : public IJobArgs
{
public:

    ~FBlendJobArgs() override {};
    FBlendJobArgs(
          const uint32 iLine
        , const uint32 iSrc_bps
        , const uint8* iSrc
        , uint8* iBdp
    )
        : IJobArgs()
        , line( iLine )
        , src_bps( iSrc_bps )
        , src( iSrc )
        , bdp( iBdp )
    {}

    const uint32 line;
    const uint32 src_bps;
    const uint8* ULIS_RESTRICT src;
    uint8* ULIS_RESTRICT bdp;
};

ULIS_NAMESPACE_END

