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

class IJobArgs {
public:
    /*! Destructor */
    virtual ~IJobArgs() {};
};


class FClearJobArgs final
    : public IJobArgs
{
public:
    ~FClearJobArgs() override {};
    FClearJobArgs(
          const uint8 iDst
        , const uint8 iSize
    )
        : IJobArgs()
        , dst( iDst )
        , size( iSize )
    {}

    const uint8 dst;
    const uint8 size;
};

int
main() {
    FClearJobArgs a( 1, 2 );
    return  0;
}

