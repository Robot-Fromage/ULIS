// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Kernel.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FKernel class.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Data/Block.h"

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FConvolutionKernel
/// @brief      The FKernel class provides a mean of storing and manipulating convolution kernels
/// @details    The kernel is actually a block in Gray floating point format.
/// @details    kernel data lives on CPU, kernel can also be used as matrix.
class ULIS2_API FKernel : public FBlock
{
public:
    typedef FKernel tSelf;
    typedef FBlock  tSuperClass;

    // Construction / Destruction
    virtual ~FKernel();

    FKernel( int iWidth, int iHeight );
};

ULIS2_NAMESPACE_END

