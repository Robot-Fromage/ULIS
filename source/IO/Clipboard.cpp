// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Clipboard.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the clipboard IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "IO/Clipboard.h"

#if defined(ULIS3_WIN)
#include "IO/Clipboard_imp/Clipboard_WindowsPlatform.ipp"
#elif defined(ULIS3_MACOS)
#include "IO/Clipboard_imp/Clipboard_MacOSPlatform.ipp"
#elif defined(ULIS3_LINUX)
#include "IO/Clipboard_imp/Clipboard_LinuxPlatform.ipp"
#else
#include "IO/Clipboard_imp/Clipboard_GenericPlatform.ipp"
#endif
