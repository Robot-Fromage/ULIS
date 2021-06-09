// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasLock.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the ILockable class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasLock.h"

ULIS_NAMESPACE_BEGIN
ULIS_DEFINE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( IHasLock, bool, IsLocked, SetLocked )
ULIS_DEFINE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( IHasPaintLock, bool, IsLocked, SetLocked )
ULIS_DEFINE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( IHasTimeLock, bool, IsLocked, SetLocked )
ULIS_NAMESPACE_END

