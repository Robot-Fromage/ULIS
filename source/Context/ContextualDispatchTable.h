// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ContextualDispatchTable.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FContextualDispatchTable class.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
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
    FContextualDispatchTable( const FHardwareMetrics& iHardwareMetrics, eFormat iFormat );

    /*! Destructor */
    ~FContextualDispatchTable();

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleBlend( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleBlendMisc;
            case BlendQualifier_Separable       : return  mScheduleBlendSeparable;
            case BlendQualifier_NonSeparable    : return  mScheduleBlendNonSeparable;
        }
    }

    ULIS_FORCEINLINE fpCommandScheduler QueryScheduleBlendSubpixel( eBlendMode iBlendingMode ) const
    {
        switch( BlendingModeQualifier( iBlendingMode ) ) {
            case BlendQualifier_Misc            : return  mScheduleBlendMiscSubpixel;
            case BlendQualifier_Separable       : return  mScheduleBlendSeparableSubpixel;
            case BlendQualifier_NonSeparable    : return  mScheduleBlendNonSeparableSubpixel;
        }
    }

private:
    const fpCommandScheduler mScheduleBlendSeparable;
    const fpCommandScheduler mScheduleBlendNonSeparable;
    const fpCommandScheduler mScheduleBlendMisc;
    const fpCommandScheduler mScheduleBlendSeparableSubpixel;
    const fpCommandScheduler mScheduleBlendNonSeparableSubpixel;
    const fpCommandScheduler mScheduleBlendMiscSubpixel;
};

ULIS_NAMESPACE_END

