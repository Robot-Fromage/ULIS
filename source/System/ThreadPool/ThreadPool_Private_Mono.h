// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ThreadPool_Private_Mono.h
* @author       Clement Berthaud && Eric Scholl
* @brief        This file provides the definition for the FThreadPool_Private
*               class for systems without actual thread support.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Array.h"
#include "Memory/Queue.h"
#include "Scheduling/Job.h"
#include "Scheduling/Command.h"

#pragma message( "MONO THREAD POOL ACTIVATED" )

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FThreadPool_Private
/// @brief      The FThreadPool_Private class provides the private implementation
///             for FThreadPool using the pimpl idiom.
/// @details    This version of the private implementation is for systems
///             without actual multi threading support.
///
///             \sa FThreadPool
class FThreadPool_Private
{
public:
    ~FThreadPool_Private();
    FThreadPool_Private( uint32 iNumWorkers );
    FThreadPool_Private( const FThreadPool_Private& ) = delete;
    FThreadPool_Private& operator=( const FThreadPool_Private& ) = delete;
    void ScheduleCommands( TQueue< const FCommand* >& ioCommands );
    void WaitForCompletion();
    void SetNumWorkers( uint32 iNumWorkers );
    uint32 GetNumWorkers() const;
    static uint32 MaxWorkers();
};

ULIS_NAMESPACE_END

