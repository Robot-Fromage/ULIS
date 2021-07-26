/*
*   kaze
*__________________
* @file         PathInfo_Private.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FPathInfo_Private tools.
* @copyright    Copyright 2021 Clement Berthaud.
* @license      Please refer to LICENSE.md
*/
#include "System/PathInfo/PathInfo_Private.h"

#if defined( ULIS_WIN )
#include "System/PathInfo/PathInfo_Windows.inl"
#elif defined( ULIS_MACOS )
#include "System/PathInfo/PathInfo_macOS.inl"
#elif defined( ULIS_LINUX )
#include "System/PathInfo/PathInfo_Linux.inl"
#else
#include "System/PathInfo/PathInfo_Generic.inl"
#endif

ULIS_NAMESPACE_BEGIN
FPathInfo_Private::FPathInfo_Private()
    : fontPaths()
    , appDataPath()
    , diskPaths()
    , homePath()
{
}

ULIS_NAMESPACE_END

