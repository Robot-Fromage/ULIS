// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         SummedAreaTable.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the SummedAreaTable entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/SummedAreaTable.h"
#include "Base/HostDeviceInfo.h"
#include "Misc/Filter.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"
#include "Misc/Dispatch/Dispatch_SAT.ipp"
#include "Misc/Dispatch/Dispatch_PremultipliedSAT.ipp"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// SAT
FBlock* XGetSummedAreaTable( FThreadPool*             iThreadPool
                           , bool                     iBlocking
                           , uint32                   iPerfIntent
                           , const FHostDeviceInfo&   iHostDeviceInfo
                           , bool                     iCallCB
                           , const FBlock*            iSource )
{
    // Assertions
    ULIS3_ASSERT( iThreadPool,              "Bad pool."                                             );
    ULIS3_ASSERT( iSource,                  "Bad source."                                           );
    ULIS3_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    tFormat satFormat = ( ( ( iSource->Format() & ULIS3_E_TYPE ) & ULIS3_E_DEPTH ) ) | ULIS3_W_TYPE( ULIS3_TYPE_UFLOAT ) | ULIS3_W_FLOATING( 1 ) | ULIS3_W_DEPTH( 4 ) );
    tFormat test = ULIS3_FORMAT_RGBAF;
    FBlock* sat = new FBlock( iSource->Width(), iSource->Height(), satFormat );

    // Query dispatched method
    fpDispatchedSATFunc fptr = QueryDispatchedSATFunctionForParameters( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo() );
    ULIS3_ASSERT( fptr, "No dispatch function found." );
    fptr( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iSource, sat );

    sat->Invalidate( iCallCB );

    return  sat;
}

/////////////////////////////////////////////////////
// Premultiplied SAT
FBlock* XGetPremultipliedSummedAreaTable( FThreadPool*             iThreadPool
                                        , bool                     iBlocking
                                        , uint32                   iPerfIntent
                                        , const FHostDeviceInfo&   iHostDeviceInfo
                                        , bool                     iCallCB
                                        , const FBlock*            iSource )
{
    // Assertions
    ULIS3_ASSERT( iThreadPool,              "Bad pool."                                             );
    ULIS3_ASSERT( iSource,                  "Bad source."                                           );
    ULIS3_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    tFormat satFormat = ( ( ( iSource->Format() & ULIS3_E_TYPE ) & ULIS3_E_DEPTH ) ) | ULIS3_W_TYPE( ULIS3_TYPE_UFLOAT ) | ULIS3_W_FLOATING( 1 ) | ULIS3_W_DEPTH( 4 ) );
    tFormat test = ULIS3_FORMAT_RGBAF;
    FBlock* sat = new FBlock( iSource->Width(), iSource->Height(), satFormat );

    // Query dispatched method
    fpDispatchedSATFunc fptr = QueryDispatchedPremultipliedSATFunctionForParameters( iPerfIntent, iHostDeviceInfo, iSource->FormatInfo() );
    ULIS3_ASSERT( fptr, "No dispatch function found." );
    fptr( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iSource, sat );

    sat->Invalidate( iCallCB );

    return  sat;
}

ULIS3_NAMESPACE_END

