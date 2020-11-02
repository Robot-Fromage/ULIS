// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         ULIS.h
* @author       Clement Berthaud
* @brief        This file provides a unique way to include all the necessary library files.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
// Blend
#include "Blend/Modes.h"
// Clear
#include "Clear/Clear.h"
// Conv
#include "Conv/Conv.h"
#include "Conv/ConvBuffer.h"
// Copy
#include "Copy/Copy.h"
// Fill
#include "Fill/Fill.h"
#include "Fill/FillPreserveAlpha.h"
// FX
//#include "FX/.h"
// Image
#include "Image/Block.h"
#include "Image/Color.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Pixel.h"
#include "Image/Sample.h"
#include "Image/Sparse/TiledBlock.h"
#include "Image/Sparse/TilePool.h"
// IO
#include "IO/Clipboard.h"
#include "IO/Disk.h"
// Maths
#include "Math/Geometry/Matrix.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Geometry/Transformation2D.h"
#include "Math/Interpolation/Bezier.h"
#include "Math/Interpolation/CatmullRom.h"
#include "Math/Interpolation/Spline.h"
#include "Math/Math.h"
// Memory
#include "Memory/Array.h"
#include "Memory/Memory.h"
// Misc
#include "Misc/Extract.h"
#include "Misc/Filter.h"
#include "Misc/GammaCompress.h"
#include "Misc/Premult.h"
#include "Misc/Sanitize.h"
#include "Misc/SummedAreaTable.h"
#include "Misc/Swap.h"
#include "Misc/Trim.h"
// Mix
#include "Mix/Mix.h"
// Raster
#include "Draw/Draw.h"
// String
#include "String/String.h"
#include "String/WString.h"
// System
#include "System/FilePathRegistry.h"
#include "System/Device.h"
// Text
#include "Text/Font.h"
#include "Text/FontRegistry.h"
#include "Text/Text.h"
#include "Text/TextEngine.h"
// Thread
#include "Thread/ThreadPool.h"
// Transform
#include "Transform/Methods.h"
#include "Transform/Transform.h"
// Context
#include "Context/Context.h"
// Scheduling
#include "Scheduling/CommandQueue.h"
#include "Scheduling/SchedulePolicy.h"
#include "Scheduling/Event.h"

