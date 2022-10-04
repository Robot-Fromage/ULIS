// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         MemoryPoolPolicy.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the FMemoryPoolPolicy class.
* @license      Please refer to LICENSE.md
*/
#include "Memory/MemoryPoolPolicy.h"

ULIS_NAMESPACE_BEGIN
//static
const FMemoryPoolPolicy FMemoryPoolPolicy::PermissiveGrowth( true );

FMemoryPoolPolicy::FMemoryPoolPolicy(
    bool iAllocPageIfOutOfSpace
)
    : bAllocPageIfOutOfSpace( iAllocPageIfOutOfSpace )
{}

bool
FMemoryPoolPolicy::AllocPageIfOutOfSpace() const {
    return  bAllocPageIfOutOfSpace;
}

void
FMemoryPoolPolicy::SetAllocPageIfOutOfSpace( bool iValue ) {
    bAllocPageIfOutOfSpace = iValue;
}

ULIS_NAMESPACE_END

