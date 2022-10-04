// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Callback.cpp
* @author       Clement Berthaud
* @brief        This file provides definitions for the image callbacks.
* @license      Please refer to LICENSE.md
*/
#include "Core/Callback.h"

ULIS_NAMESPACE_BEGIN
void OnCleanup_FreeMemory( uint8* iData, void* iInfo )
{
    delete [] iData;
}

ULIS_NAMESPACE_END

