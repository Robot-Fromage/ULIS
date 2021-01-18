// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ContextualDispatchTable.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FContextualDispatchTable class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Conv/Conv.h"
#include "Context/Context.h"
#include "Scheduling/Command.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FContext::FContextualDispatchTable
struct FContext::FContextualDispatchTable
{
    friend class FContext;

public:
    /*! Constructor */
    FContextualDispatchTable( const FHardwareMetrics& iHardwareMetrics, eFormat iFormat, ePerformanceIntent iPerfIntent );

    /*! Destructor */
    ~FContextualDispatchTable();

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleBlend( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleBlendMisc;
            case BlendQualifier_Separable       : return  mScheduleBlendSeparable;
            case BlendQualifier_NonSeparable    : return  mScheduleBlendNonSeparable;
            default: return  nullptr;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleBlendSubpixel( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleBlendMiscSubpixel;
            case BlendQualifier_Separable       : return  mScheduleBlendSeparableSubpixel;
            case BlendQualifier_NonSeparable    : return  mScheduleBlendNonSeparableSubpixel;
            default: return  nullptr;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleTiledBlend( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleTiledBlendMisc;
            case BlendQualifier_Separable       : return  mScheduleTiledBlendSeparable;
            case BlendQualifier_NonSeparable    : return  mScheduleTiledBlendNonSeparable;
            default: return  nullptr;
        }
    }

private:
    const fpCommandScheduler mScheduleBlendSeparable;
    const fpCommandScheduler mScheduleBlendNonSeparable;
    const fpCommandScheduler mScheduleBlendMisc;
    const fpCommandScheduler mScheduleBlendSeparableSubpixel;
    const fpCommandScheduler mScheduleBlendNonSeparableSubpixel;
    const fpCommandScheduler mScheduleBlendMiscSubpixel;
    const fpCommandScheduler mScheduleAlphaBlend;
    const fpCommandScheduler mScheduleAlphaBlendSubpixel;
    const fpCommandScheduler mScheduleTiledBlendSeparable;
    const fpCommandScheduler mScheduleTiledBlendNonSeparable;
    const fpCommandScheduler mScheduleTiledBlendMisc;
    const fpCommandScheduler mScheduleClear;
    const fpCommandScheduler mScheduleCopy;
    const fpCommandScheduler mScheduleConvertFormat;
    const fpCommandScheduler mScheduleFill;
    const fpCommandScheduler mScheduleFillPreserveAlpha;
    const fpCommandScheduler mScheduleRasterText;
    const fpCommandScheduler mScheduleRasterTextAA;
    const fpCommandScheduler mScheduleResizeArea;
    const fpCommandScheduler mScheduleResizeBicubic;
    const fpCommandScheduler mScheduleResizeBilinear;
    const fpCommandScheduler mScheduleResizeNN;
    const fpCommandScheduler mScheduleTransformAffineBicubic;
    const fpCommandScheduler mScheduleTransformAffineBilinear;
    const fpCommandScheduler mScheduleTransformAffineNN;
    const fpCommandScheduler mScheduleTransformAffineTiledBicubic;
    const fpCommandScheduler mScheduleTransformAffineTiledBilinear;
    const fpCommandScheduler mScheduleTransformAffineTiledNN;
    const fpCommandScheduler mScheduleTransformBezierBicubic;
    const fpCommandScheduler mScheduleTransformBezierBilinear;
    const fpCommandScheduler mScheduleTransformBezierNN;
    const fpCommandScheduler mScheduleTransformPerspectiveBicubic;
    const fpCommandScheduler mScheduleTransformPerspectiveBilinear;
    const fpCommandScheduler mScheduleTransformPerspectiveNN;

    const fpConvertFormat mArgConvForwardBlendNonSeparable;
    const fpConvertFormat mArgConvBackwardBlendNonSeparable;
};

ULIS_NAMESPACE_END

