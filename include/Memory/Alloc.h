// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Alloc.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for Alloc tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
//#include "Memory/Units.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FAlloc
/// @brief      The FAlloc class is a utility class that provides a mean of
///             accessing typedefs and static tools for Arenas and Pools.
/// @details    FAlloc does nothing and isn't instanciable, it's just at tool
///             made as a class 
class ULIS_API FAlloc
{
public:
    typedef uint8 tByte;                        ///< A byte or octet value, 8bits
    typedef uint8 const tConstByte;             ///< A const byte
    typedef tByte* tAlloc;                      ///< A base pointer for a heap allocation
    typedef tConstByte* const tConstAlloc;      ///< A const base pointer for a const heap allocation
    typedef tByte* tMetaBase;                   ///< A base pointer for a meta base in an Arena ( see docs for specific arena about their meta base )
    typedef tConstByte* const tConstMetaBase;   ///< A const base pointer for a const meta base in an Arena ( see docs for specific arena about their meta base )
    typedef tAlloc* tClient;                    ///< A pointer to a client, that is a pointer to a heap allocation susceptible to move after defrag.
    typedef tConstAlloc* const tConstClient;    ///< A const pointer to a const client, that is a const pointer to a const heap allocation susceptible to move after defrag.
    typedef uint64 tArenaSize;                  ///< A large size for an arena.
    typedef uint32 tAllocSize;                  ///< A large size for an allocation.

public:
    // Exhaustive delete *tors.
    ~FAlloc() = delete;                             // destructor
    FAlloc() = delete;                              // constructor
    FAlloc( const FAlloc& ) = delete;               // copy constructor
    FAlloc( FAlloc&& ) = delete;                    // move constructor
    FAlloc& operator=( const FAlloc& ) = delete;    // copy assignment operator
    FAlloc& operator=( FAlloc&& ) = delete;         // move assignment operator
};

ULIS_NAMESPACE_END

