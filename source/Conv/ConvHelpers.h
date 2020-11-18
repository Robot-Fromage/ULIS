// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ConvHelpers.h
* @author       Clement Berthaud
* @brief        This file provides some conversion helper macros.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Conv/ConvArgs.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Command.h"
#include "Scheduling/Job.h"
#include "Scheduling/RangeBasedPolicyScheduler.h"

/////////////////////////////////////////////////////
// Macro Helpers for Redundant Conversion Operations
// Macro utils for implementations
#define U2_DREF_RED_CHAN( T, iPtr, iFmt, iChan )    ( *( ( T* )( iPtr ) + iFmt.IDT[ iChan ] ) )
#define U2_DREF_CHAN( T, iPtr, iChan )              ( *( ( T* )( iPtr ) + iChan ) )
#define U2_FWD_ALPHA                                if( iDstFormat.HEA ) { U2_DREF_CHAN( U, iDst, iDstFormat.AID ) = iSrcFormat.HEA? ConvType< T, U >( U2_DREF_CHAN( T, iSrc, iSrcFormat.AID ) ) : MaxType< U >(); }

#define U2_DREF_SRC( iChan )                        U2_DREF_RED_CHAN( T, iSrc, iSrcFormat, iChan )
#define U2_DREF_DST( iChan )                        U2_DREF_RED_CHAN( U, iDst, iDstFormat, iChan )
#define U2_DREF_TEMP( iChan )                       U2_DREF_RED_CHAN( ufloat, temp.Bits(), temp.FormatMetrics(), iChan )

ULIS_NAMESPACE_BEGIN
template< void (*TDelegateInvoke)( const FConvJobArgs*, const FConvCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildConvJobs_Scanlines(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iNumJobs
    , const int64 iNumTasksPerJob
)
{
    const FConvCommandArgs* cargs           = dynamic_cast< const FConvCommandArgs* >( iCommand->Args() );
    const FFormatMetrics& fmt               = cargs->src.FormatMetrics();
    const uint8* const ULIS_RESTRICT src    = cargs->src.Bits() + cargs->srcRect.x * fmt.BPP;
    uint8* const ULIS_RESTRICT dst          = cargs->dst.Bits() + cargs->dstRect.x * fmt.BPP;
    const int64 src_bps                     = static_cast< int64 >( cargs->src.BytesPerScanLine() );
    const int64 dst_bps                     = static_cast< int64 >( cargs->dst.BytesPerScanLine() );
    const int64 size                        = cargs->dstRect.w;

    for( int i = 0; i < iNumJobs; ++i )
    {
        uint8* buf = new uint8[ iNumTasksPerJob * sizeof( FConvJobArgs ) ];
        FConvJobArgs* jargs = reinterpret_cast< FConvJobArgs* >( buf );
        for( int i = 0; i < iNumTasksPerJob; ++i )
            new ( buf ) FConvJobArgs(
              src + ( cargs->srcRect.y + i ) * src_bps
            , dst + ( cargs->dstRect.y + i ) * dst_bps
            , size
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FConvJobArgs, FConvCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
    }
}

template< void (*TDelegateInvoke)( const FConvJobArgs*, const FConvCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildConvJobs_Chunks(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
    , const int64 iSize
    , const int64 iCount
)
{
    const FConvCommandArgs* cargs = dynamic_cast< const FConvCommandArgs* >( iCommand->Args() );
    ULIS_ASSERT( cargs->contiguous, "Bad scheduling, illegal policy if contiguous flag is not set." );

    const uint8* const ULIS_RESTRICT src    = cargs->src.Bits();
    uint8* const ULIS_RESTRICT dst          = cargs->dst.Bits();
    const int64 area                        = static_cast< int64 >( cargs->src.Rect().Area() );

    int64 index = 0;
    for( int i = 0; i < iCount; ++i )
    {
        uint8* buf = new uint8[ sizeof( FConvJobArgs ) ];
        FConvJobArgs* jargs = reinterpret_cast< FConvJobArgs* >( buf );
        new ( buf ) FConvJobArgs(
              src + index
            , dst + index
            , FMath::Min( index + iSize, area ) - index
        );
        FJob* job = new FJob(
              1
            , &ResolveScheduledJobCall< FConvJobArgs, FConvCommandArgs, TDelegateInvoke >
            , jargs );
        iCommand->AddJob( job );
        index += iSize;
    }
    return;
}

template< void (*TDelegateInvoke)( const FConvJobArgs*, const FConvCommandArgs* ) >
ULIS_FORCEINLINE
static
void
BuildConvJobs(
      FCommand* iCommand
    , const FSchedulePolicy& iPolicy
)
{
    const FConvCommandArgs* cargs   = dynamic_cast< const FConvCommandArgs* >( iCommand->Args() );
    const int64 area                = static_cast< int64 >( cargs->src.Rect().Area() );
    RangeBasedPolicyScheduleJobs< &BuildConvJobs_Scanlines< TDelegateInvoke >, &BuildConvJobs_Chunks< TDelegateInvoke > >( iCommand, iPolicy, area, cargs->dstRect.h, cargs->contiguous );
}
ULIS_NAMESPACE_END

