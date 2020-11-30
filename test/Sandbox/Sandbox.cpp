// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/**
*
*   ULIS
*__________________
*
* @file         Sandbox.cpp
* @author       Clement Berthaud
* @brief        Sandbox program without ULIS.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <cstdint>
#include <iostream>
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;

class ICommandArgs {
public:
    virtual ~ICommandArgs() {};
};


class FClearCommandArgs final
    : public ICommandArgs
{
public:
    ~FClearCommandArgs() override {};
    FClearCommandArgs() {};

    uint8 dst;
    uint8 size;
};


class IJobArgs {
public:
    virtual ~IJobArgs() {};
};


class FClearJobArgs final
    : public IJobArgs
{
public:
    ~FClearJobArgs() override {};
    FClearJobArgs() {};

    uint8 dst;
    uint8 size;
};

void
InvokeClear(
      const FClearJobArgs* jargs
    , const FClearCommandArgs* cargs
)
{
    const volatile auto test = 0;
}

static
void
BuildClearJob(
      const uint32 iNumJobs
    , const uint32 iNumTasksPerJob
    , FClearJobArgs& iJob
)
{
    const volatile auto test = 0;
}

template< typename T, typename U, void (*TDelegateInvoke)( const T*, const U* ), void (*TDelegateBuild)( const uint32, const uint32, T& ) >
static
void
RangeBaseSchedulingBuildJobs(
      const uint32 iNumJobs
    , const uint32 iNumTasksPerJob
)
{
    for( int i = 0; i < iNumJobs; ++i )
    {
        uint8* buf = new uint8[ iNumTasksPerJob * sizeof( T ) ];
        T* jargs = reinterpret_cast< T* >( buf );

        for( int i = 0; i < iNumTasksPerJob; ++i ) {
            new ( buf ) T();
            TDelegateBuild( iNumJobs, iNumTasksPerJob, jargs[i] );
        }

    }
}

int
main() {
    RangeBaseSchedulingBuildJobs< FClearJobArgs, FClearCommandArgs, InvokeClear, BuildClearJob >( 2, 1 );
    return  0;
}

