// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         FDecl.h
* @author       Clement Berthaud
* @brief        This file provides core forward declarations for the ULIS2 library.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Platform.h"

/////////////////////////////////////////////////////
// FDecl Typedefs and enums
ULIS2_NAMESPACE_BEGIN
class   FBlock;
class   IPixel;
class   FPixelValue;
class   FPixelProxy;
class   FProfileRegistry;
class   FColorProfile;
class   FThreadPool;
struct  FRect;
struct  FPerfInfo;
struct  FHostDeviceInfo;
class   FFontRegistry;
class   FFontEngine;
struct  FFormatInfo;
class   FFont;
class   FTransform2D;
struct  FTileElement;
class ITiledBlock;
template< uint8 _MICRO, uint8 _MACRO > class TTilePool;
template< uint8 _MICRO, uint8 _MACRO > class TTiledBlock;

template< typename T >
struct TVec2;
typedef TVec2< int >            FVec2I;
typedef TVec2< float >          FVec2F;
typedef TVec2< int64 >          FVec2I64;
typedef TVec2< int32 >          FVec2I32;

ULIS2_NAMESPACE_END

