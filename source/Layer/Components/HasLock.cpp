// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasLock.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasLock* classes.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasLock.h"

ULIS_NAMESPACE_BEGIN
// We define the methods of the IHasLock* classes
ULIS_DEFINE_PIC_SIMPLE( IHasLock, bool, IsLocked, SetLocked )
ULIS_DEFINE_PIC_SIMPLE( IHasPaintLock, bool, IsPaintLocked, SetPaintLocked )
ULIS_DEFINE_PIC_SIMPLE( IHasTimeLock, bool, IsTimeLocked, SetTimeLocked )
ULIS_NAMESPACE_END

