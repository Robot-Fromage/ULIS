// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasVisibility.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasVisibility class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasVisibility.h"

ULIS_NAMESPACE_BEGIN
ULIS_DEFINE_SIMPLE_PSEUDO_COMPOSITION_CLASS_SINGLE_PROPERTY( IHasVisibility, bool, IsVisible, SetVisible )
ULIS_NAMESPACE_END

