// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         BlendInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FBlendInfo class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/BlendInfo.h"
#include <type_traits>

ULIS_NAMESPACE_BEGIN
static_assert( std::is_trivial< FBlendInfo >::value );
static_assert( std::is_standard_layout< FBlendInfo >::value );
static_assert( std::is_aggregate< FBlendInfo >::value );
static_assert( std::is_pod< FBlendInfo >::value );
ULIS_NAMESPACE_END

