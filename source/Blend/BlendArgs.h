// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
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
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/ScheduleArgs.h"
#include "Conv/Conv.h"
#include "Image/Block.h"
#include <vectorclass.h>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FBlendCommandArgs
class FBlendCommandArgs final
    : public ICommandArgs
{
public:
    ~FBlendCommandArgs() override
    {
        if( color )
            delete color;
    };

    FBlendCommandArgs(
          const FBlock& iSource
        , FBlock& iBackdrop
        , const FRectI& iSourceRect
        , const FVec2F& iSubpixelComponent
        , const FVec2F& iBuspixelComponent
        , const eBlendMode iBlendingMode
        , const eAlphaMode iAlphaMode
        , const ufloat iOpacity
        , const FVec2I& iShift
        , const FVec2I& iBackdropCoverage
        , const FRectI& iBackdropWorkingRect
        , const FBlock* iColor = nullptr
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
        , color( iColor )
        {}

    const FBlock& source;
    FBlock& backdrop;
    const FRectI sourceRect;
    const FVec2F subpixelComponent;
    const FVec2F buspixelComponent;
    const eBlendMode blendingMode;
    const eAlphaMode alphaMode;
    const ufloat opacity;
    const FVec2I shift;
    const FVec2I backdropCoverage;
    const FRectI backdropWorkingRect;
    const FBlock* const color;
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
        , uint8* const iBdp
        , const fpConvertFormat iFwd
        , const fpConvertFormat iBkd
        , const Vec4i iIDT
    )
        : IJobArgs()
        , line( iLine )
        , src_bps( iSrc_bps )
        , src( iSrc )
        , bdp( iBdp )
        , fwd( iFwd )
        , bkd( iBkd )
        , idt( iIDT )
    {}

    const uint32 line;
    const uint32 src_bps;
    const uint8* const ULIS_RESTRICT src;
    uint8* const ULIS_RESTRICT bdp;
    const fpConvertFormat fwd;
    const fpConvertFormat bkd;
    const Vec4i idt;
};

ULIS_NAMESPACE_END

