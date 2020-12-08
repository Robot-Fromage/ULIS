// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Blend.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Blend API.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/Conv.h"
#include "Dispatch/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/DualBufferArgs.h"
#include <vectorclass.h>

// Include MEM Generic Implementation
#include "Blend/Generic/BlendMT_Separable_MEM_Generic.h"
#include "Blend/Generic/BlendMT_NonSeparable_MEM_Generic.h"
#include "Blend/Generic/BlendMT_Misc_MEM_Generic.h"
#include "Blend/Generic/AlphaBlendMT_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_Separable_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_NonSeparable_MEM_Generic.h"
#include "Blend/Generic/TiledBlendMT_Misc_MEM_Generic.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FBlendCommandArgs
class FBlendCommandArgs final
    : public FDualBufferCommandArgs
{
public:
    ~FBlendCommandArgs() override
    {
        if( color )
            delete color;
    };

    FBlendCommandArgs(
          const FBlock& iSrc
        , FBlock& iDst
        , const FRectI& iSrcRect
        , const FRectI& iDstRect
        , const FVec2F& iSubpixelComponent
        , const FVec2F& iBuspixelComponent
        , const eBlendMode iBlendingMode
        , const eAlphaMode iAlphaMode
        , const ufloat iOpacity
        , const FVec2I& iShift
        , const FVec2I& iBackdropCoverage
        , const fpConvertFormat iFwd
        , const fpConvertFormat iBkd
        , const Vec4i iIDT
        , const uint32 iSrcBps
        , const FBlock* iColor = nullptr
    )
        : FDualBufferCommandArgs(
              iSrc
            , iDst
            , iSrcRect
            , iDstRect
            )
        , subpixelComponent( iSubpixelComponent )
        , buspixelComponent( iBuspixelComponent )
        , blendingMode( iBlendingMode )
        , alphaMode( iAlphaMode )
        , opacity( iOpacity )
        , shift( iShift )
        , backdropCoverage( iBackdropCoverage )
        , fwd( iFwd )
        , bkd( iBkd )
        , idt( iIDT )
        , color( iColor )
        , src_bps( iSrcBps )
        {}

    const FVec2F subpixelComponent;
    const FVec2F buspixelComponent;
    const eBlendMode blendingMode;
    const eAlphaMode alphaMode;
    const ufloat opacity;
    const FVec2I shift;
    const FVec2I backdropCoverage;
    const fpConvertFormat fwd;
    const fpConvertFormat bkd;
    const Vec4i idt;
    const uint32 src_bps;
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
          const uint8* iSrc
        , uint8* const iBdp
        , const uint32 iLine
        
    )
        : IJobArgs()
        , src( iSrc )
        , bdp( iBdp )
        , line( iLine )
    {}

    const uint8* ULIS_RESTRICT src;
    uint8* ULIS_RESTRICT bdp;
    uint32 line;
};
ULIS_NAMESPACE_END

