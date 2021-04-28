// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         ULIS.h
* @author       Clement Berthaud
* @brief        This file provides a unique way to include all the necessary library files.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
// Core
#include "Core/AXI.h"
// Context
#include "Context/Context.h"
// Image
#include "Image/Block.h"
#include "Image/Color.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Pixel.h"
#include "Image/Sample.h"
#include "Image/Gradient.h"
#include "Image/Kernel.h"
#include "Image/StructuringElement.h"
// Layer
#include "Layer/Layer.h"
#include "Layer/LayerFolder.h"
#include "Layer/LayerImage.h"
#include "Layer/LayerStack.h"
// Font
#include "Font/Font.h"
#include "Font/FontEngine.h"
// Maths
#include "Math/Geometry/Matrix.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Interpolation/Bezier.h"
#include "Math/Interpolation/CatmullRom.h"
#include "Math/Interpolation/Spline.h"
#include "Math/Math.h"
// Memory
#include "Memory/Array.h"
#include "Memory/Memory.h"
#include "Memory/ForwardList.h"
#include "Memory/Queue.h"
// String
#include "String/String.h"
#include "String/WString.h"
// System
#include "System/Device.h"
#include "System/FilePathRegistry.h"
#include "System/ThreadPool.h"
// Scheduling
#include "Scheduling/CommandQueue.h"
#include "Scheduling/Event.h"
#include "Scheduling/SchedulePolicy.h"
/*
// Sparse
#include "Sparse/Chunk.h"
#include "Sparse/Tile.h"
#include "Sparse/TiledBlock.h"
#include "Sparse/TilePool.h"
*/
