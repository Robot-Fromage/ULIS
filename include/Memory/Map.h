// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Map.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the TMap class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/ForwardList.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TMap
/// @brief      The TMap class provides a simple associative array based on
///             forward list.
/// @details    The need of a custom TMap class rose from the fact
///             that std:: classes cannot be exported easily accross dll
///             boundaries when ULIS is compiled as a shared library.
template< typename T >
class TMap
{
};

ULIS_NAMESPACE_END

