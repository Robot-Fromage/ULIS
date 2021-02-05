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
#include "Image/Sample.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FGradientStep
/// @brief      The FGradientStep class provides a mean of storing a gradient
///             step.
class ULIS_API FGradientStep final
{
public:
    /*! Destroy the gradient step. */
    ~FGradientStep();

    /*! Construct gradient step. */
    FGradientStep();

    /*! Construct gradient step. */
    FGradientStep( const FColor& iColor, ufloat iStep );

public:
    /*! Set the step. */
    void Step( ufloat iValue );

    /*! Get the step. */
    ufloat Step() const;

    /*! Get the color, editable. */
    FColor& Color();

    /*! Get the color. */
    const FColor& Color() const;

private:
    ufloat mStep;   ///< The gradient step value.
    FColor mColor;  ///< The gradient step color.
};

template class ULIS_API TArray< FGradientStep >;

/////////////////////////////////////////////////////
/// @class      FGradient
/// @brief      The FGradient class provides a mean of storing and manipulating
///             gradients in various formats.
/// @details    Gradients have at least two colors, white to black by default.
///             Steps are in [0;1]. Interpolation is linear in the preferred
///             format. It is assumed for simplicity that there are always two
///             colors, one at 0 and one at 1.
class ULIS_API FGradient final
{
public:
    /*! Destroy the gradient. */
    ~FGradient();

    /*!
    Construct gradient with preferred interpolation format.
    By default, gradients are white to black.
    */
    FGradient( eFormat iFormat );

    FGradient( const FGradient& ) = delete;
    FGradient& operator=( const FGradient& ) = delete;

public:
    /*! Getter for gradient steps. */
    const TArray< FGradientStep >& Steps() const;

    /*! Reset the gradient, keep the same format. */
    void Reset();

    /*! Reset the gradient, change the format. */
    void Reset( eFormat iFormat );

    /*! Change the preferred interpolation format. */
    void ReinterpretInterpolationFormat( eFormat iFormat );

    /*! Add a step to the gradient, value is clamped in [0;1]. Return the index.*/
    uint64 AddStep( ufloat iStep, const ISample& iValue );

    /*! Erase a step in the gradient from index. */
    void EraseStep( uint64 iIndex );

    /*! Getter for num steps */
    uint64 NumSteps() const;

    /*!
    Compute linearly interpolated color at step.
    This method is convenient and abstracts away a lot of stuff, but it is also
    quite inefficient if called in a performance critical loop.
    */
    FColor ColorAtStep( ufloat iStep ) const;

private:
    TArray< FGradientStep > mSteps; ///< The gradient steps.
    eFormat mFormat; ///< The gradient prefered interpolation format.
};

ULIS_NAMESPACE_END

