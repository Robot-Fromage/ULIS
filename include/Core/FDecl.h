// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         FDecl.h
* @author       Clement Berthaud
* @brief        This file provides core forward declarations for the ULIS library.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Platform.h"

ULIS_NAMESPACE_BEGIN
class   FBlock;
struct  FCatmullRomSpline;
class   FColor;
class   FColorSpace;
class   FCommandQueue;
class   FContext;
struct  FCubicBezierControlPoint;
class   FEvent;
class   FFilePathRegistry;
class   FFont;
class   FFontFamilyEntry;
class   FFontRegistry;
class   FFontStyleEntry;
struct  FFormatMetrics;
class   FHardwareMetrics;
struct  FMath;
class   FPixel;
class   FSchedulePolicy;
struct  FSplineLinearSample;
struct  FSplineParametricSample;
class   FString;
class   FTextEngine;
class   FTexture;
class   FThreadPool;
struct  FTileElement;
class   FTransformation2D;
class   FWString;
class   IHasColorSpace;
class   IHasFormat;
class   ISample;
class   ITiledBlock;
class   ITilePool;
template< typename T > class    TArray;
template< typename T > struct   TVector2;
typedef TVector2< int >         FVec2I;
typedef TVector2< float >       FVec2F;
typedef TVector2< double >      FVec2D;
typedef TVector2< uint8 >       FVec2UI8;
typedef TVector2< uint16 >      FVec2UI16;
typedef TVector2< uint32 >      FVec2UI32;
typedef TVector2< uint64 >      FVec2UI64;
typedef TVector2< int16 >       FVec2I16;
typedef TVector2< int32 >       FVec2I32;
typedef TVector2< int64 >       FVec2I64;
template< typename T > struct   TVector3;
typedef TVector3< int >         FVec3I;
typedef TVector3< float >       FVec3F;
template< typename T > struct   TVector4;
typedef TVector4< float >       FVec4F;
template< typename T > struct   TRectangle;
typedef TRectangle< int >       FRectI;
typedef TRectangle< float >     FRectF;
template< typename T > class    TMatrix2;
typedef TMatrix2< float >       FMat2F;
template< typename T > class    TMatrix3;
typedef TMatrix3< float >       FMat3F;
template< typename T > class    TMatrix4;
typedef TMatrix4< float >       FMat4F;

ULIS_NAMESPACE_END

