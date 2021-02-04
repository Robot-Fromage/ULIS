// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Gradient.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FGradient class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/Color.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Pixel.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FGradient
/// @brief      The FGradient class provides a mean of storing and manipulating
///             gradients in various formats.
class ULIS_API FGradient final
{
public:
    /*! Destroy the gradient. */
    ~FGradient();

    /*! Construct an empty gradient with preferred interpolation format. */
    FGradient( eFormat iFormat );

    FGradient( const FGradient& ) = delete;
    FGradient& operator=( const FGradient& ) = delete;

public:
    /*! Getter for gradient steps. */
    const TArray< ufloat >& Steps() const;

    /*! Getter for gradient colors. */
    const TArray< FColor >& Colors() const;

    /*! Reset the gradient, keep the same format. */
    void Reset();

    /*! Reset the gradient, change the format. */
    void Reset( eFormat iFormat );

    /*! Change the preferred interpolation format. */
    void ReinterpretInterpolationFormat( eFormat iFormat );

    /*! Add a step to the gradient, value is clamped in [0;1]. Return the index.*/
    int AddStep( ufloat iStep, const ISample& iValue );

    /*! Getter for first Index at step */
    int IndexAtStep( ufloat iStep ) const;

    /*! Getter for num steps */
    int NumSteps() const;

    /*! Compute linearly interpolated color at step. */
    FColor ColorAtStep( ufloat iStep ) const;

private:
    TArray< ufloat > mSteps;    ///< The gradient steps.
    TArray< FColor > mColors;   ///< The gradient colors.
    eFormat mFormat;            ///< The gradient prefered interpolation format.
};

ULIS_NAMESPACE_END

