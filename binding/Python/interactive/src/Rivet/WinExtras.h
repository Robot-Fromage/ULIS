// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         WinExtras.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once
#define NOMINMAX
#include <windows.h>

// Utility Enum for Windows Versions
enum  eWindowsVersion
{
    kBadValue = 0,
    kWindowsXP,
    kWindowsXPSP1,
    kWindowsXPSP2,
    kWindowsXPSP3,
    kWindowsVista,
    kWindowsVistaSP1,
    kWindowsVistaSP2,
    kWindows7,
    kWindows7SP1,
    kWindows8,
    kWindows8Point1,
    kWindows10
};


// Public utility Functions related to the Windows API
int                 GetZOrder( HWND iTarget );
eWindowsVersion     GetWindowsVersion();
bool                IsWindowsVersion( eWindowsVersion iWindowsVersion );
bool                EnableGlassForWindow8OrGreater( HWND iWindow );
bool                DisableGlassForWindow8OrGreater( HWND iWindow );
static HMODULE      _load_sys_library( const WCHAR* name);

