// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Clipboard.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the clipboard IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "IO/Clipboard.h"

#if defined(ULIS_WIN)
#include "IO/Clipboard_imp/Clipboard_WindowsPlatform.ipp"
#elif defined(ULIS_MACOS)
#include "IO/Clipboard_imp/Clipboard_MacOSPlatform.ipp"
#elif defined(ULIS_LINUX)
#include "IO/Clipboard_imp/Clipboard_LinuxPlatform.ipp"
#else
#include "IO/Clipboard_imp/Clipboard_GenericPlatform.ipp"
#endif
