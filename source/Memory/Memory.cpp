// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Memory.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the memory functions.
* @license      Please refer to LICENSE.md
*/
#include "Memory/Memory.h"

ULIS_NAMESPACE_BEGIN
void*
XMalloc(
    uint64 iSizeBytes
)
{
    return  malloc( iSizeBytes );
}

void
XFree(
    void* iAlloc
)
{
    free( iAlloc );
}

template< typename Derived, typename Base >
Derived& DynamicCast( Base& iBase ) {
    return  dynamic_cast< Derived& >( iBase );
}

ULIS_NAMESPACE_END

