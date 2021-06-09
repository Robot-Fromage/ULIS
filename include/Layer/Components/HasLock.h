// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasLock.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the IHasLock* classes.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Common/LayerUtils.h"

ULIS_NAMESPACE_BEGIN
ULIS_DECLARE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( IHasLock, bool, false, IsLocked, SetLocked )
ULIS_DECLARE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( IHasPaintLock, bool, false, IsPaintLocked, SetPaintLocked )
ULIS_DECLARE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( IHasTimeLock, bool, false, IsTimeLocked, SetTimeLocked )
ULIS_NAMESPACE_END

