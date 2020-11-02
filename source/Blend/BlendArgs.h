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
#include "Blend/Modes.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/ScheduleArgs.h"
#include "Conv/ConvBuffer.h"
#include <vectorclass.h>

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

    const FBlock& const source;
    FBlock& const backdrop;
    const FRectI sourceRect;
    const FVec2F subpixelComponent;
    const FVec2F buspixelComponent;
    const eBlendMode blendingMode;
    const eAlphaMode alphaMode;
    const ufloat opacity;
    const FVec2I shift;
    const FVec2I backdropCoverage;
    const FRectI backdropWorkingRect;
};

/////////////////////////////////////////////////////
// FBlendJobArgs_Separable
class FBlendJobArgs_Separable final
    : public IJobArgs
{
public:

    ~FBlendJobArgs_Separable() override {};
    FBlendJobArgs_Separable(
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
    const uint8* const ULIS_RESTRICT src;
    uint8* const ULIS_RESTRICT bdp;
};

/////////////////////////////////////////////////////
// FBlendJobArgs_NonSeparable_MEM_Generic
class FBlendJobArgs_NonSeparable_MEM_Generic final
    : public IJobArgs
{
public:

    ~FBlendJobArgs_NonSeparable_MEM_Generic() override {};
    FBlendJobArgs_NonSeparable_MEM_Generic(
          const uint32 iLine
        , const uint32 iSrc_bps
        , const uint8* iSrc
        , uint8* iBdp
        , fpConversionInvocation iFwd
        , fpConversionInvocation iBkd
    )
        : IJobArgs()
        , line( iLine )
        , src_bps( iSrc_bps )
        , src( iSrc )
        , bdp( iBdp )
        , fwd( iFwd )
        , bkd( iBkd )
    {}

    const uint32 line;
    const uint32 src_bps;
    const uint8* const ULIS_RESTRICT src;
    uint8* const ULIS_RESTRICT bdp;
    const fpConversionInvocation fwd;
    const fpConversionInvocation bkd;
};

/////////////////////////////////////////////////////
// FBlendJobArgs_NonSeparable_SSE_RGBA8
class FBlendJobArgs_NonSeparable_SSE_RGBA8 final
    : public IJobArgs
{
public:

    ~FBlendJobArgs_NonSeparable_SSE_RGBA8() override {};
    FBlendJobArgs_NonSeparable_SSE_RGBA8(
          const uint32 iLine
        , const uint32 iSrc_bps
        , const uint8* iSrc
        , uint8* iBdp
        , fpConversionInvocation iFwd
        , fpConversionInvocation iBkd
        , Vec4i iIDT
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
    const fpConversionInvocation fwd;
    const fpConversionInvocation bkd;
    const Vec4i idt;
};

ULIS_NAMESPACE_END

