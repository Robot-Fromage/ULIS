// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasCachedTransform.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the IHasCachedTransform class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Components/TransformInfo.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      IHasCachedTransform
/// @brief      Simple HasCachedTransform class.
class ULIS_API IHasTransform
{
protected:
//    IHasTransform( const FMat3F& iMatrix = FMat3F() );

public:
    // TODO: Check, how do we chose to manipulate it ?
    // Obtain reference to matrix and manipulate it
    // Make APIs like Rotate() Translate() SetTranslation(), etc...
    // Is it gonna be easy to make this an AnimatedProperty in a 2D timeline ?

    // Getters
    FVec2F Pivot() const;
    float PivotX() const;
    float PivotY() const;

    FVec2F Translation() const;
    float TranslationX() const;
    float TranslationY() const;

    float Rotation() const;

    FVec2F Scale() const;
    float ScaleX() const;
    float ScaleY() const;

    // Setters
    void SetPivot( float iX, float iY );
    void SetPivotX( float iX );
    void SetPivotY( float iY );

    void MovePivot( float iDeltaX, float iDeltaY );
    void MovePivotX( float iDeltaX );
    void MovePivotY( float iDeltaY );

    void Move( float iDeltaX, float iDeltaY );
    void MoveX( float iDeltaX );
    void MoveY( float iDeltaY );

    void SetTranslation( float iX, float iY );
    void SetTranslationX( float iX );
    void SetTranslationY( float iY );

    void SetScale( float iX, float iY );
    void SetScaleX( float iX );
    void SetScaleY( float iY );
    void SetScaleUniform( float iValue );

    void DeltaScale( float iDeltaX, float iDeltaY );
    void DeltaScaleX( float iDeltaX );
    void DeltaScaleY( float iDeltaY );
    void DeltaScaleUniform( float iDelta );

    void SetStretch( float iX, float iY );
    void SetStretchX( float iX );
    void SetStretchY( float iY );
    void StretchUniform( float iValue );

    void SetRotation( float iValue );

    void ResetPivot();
    void ResetTranslation();
    void ResetScale();
    void ResetStretch();
    void ResetRotation();

private:
    FTransformInfo mInfo;
};

ULIS_NAMESPACE_END

