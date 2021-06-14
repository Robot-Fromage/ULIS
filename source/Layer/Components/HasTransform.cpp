// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasCachedTransform.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasTransform class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasTransform.h"

ULIS_NAMESPACE_BEGIN

IHasTransform::IHasTransform(
      const FVec2F& iPivot
    , const FVec2F& iTranslation
    , ufloat iRotation
    , const FVec2F& iScale
    , const FOnTransformInfoChanged& iDelegate
)
    : TCallbackCapable< FOnTransformInfoChanged >( iDelegate )
    , mInfo {
          iPivot
        , iTranslation
        , iRotation
        , iScale
    }
{
    OnChanged( mInfo );
}

// Pivot setter
void
IHasTransform::ResetPivot() {
    mInfo.pivot = FVec2F( 0 );
    OnChanged( mInfo );
}

void
IHasTransform::SetPivot( const FVec2F& iValues ) {
    mInfo.pivot = iValues;
    OnChanged( mInfo );
}

void
IHasTransform::SetPivot( float iX, float iY ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetPivotX( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetPivotY( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::MovePivot( const FVec2F& iDelta ) {
    mInfo.pivot += iDelta;
    OnChanged( mInfo );
}

void
IHasTransform::MovePivot( float iDeltaX, float iDeltaY ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::MovePivotX( float iDelta ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::MovePivotY( float iDelta ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::MovePivotRight( float iDelta ) {
    mInfo.pivot += Right() * iDelta;
    OnChanged( mInfo );
}

void
IHasTransform::MovePivotDown( float iDelta ) {
    mInfo.pivot += Down() * iDelta;
    OnChanged( mInfo );
}

// Translation setter
void
IHasTransform::ResetTranslation() {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetTranslation( const FVec2F& iValues ) {
    // TODO
    OnChanged( mInfo );
}
void
IHasTransform::SetTranslation( float iX, float iY ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetTranslationX( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetTranslationY( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::Translate( const FVec2F& iDelta ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::Translate( float iDeltaX, float iDeltaY ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::TranslateX( float iDelta ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::TranslateY( float iDelta ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::TranslateZ( float iDelta ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::Move( const FVec2F& iDelta ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::Move( float iDeltaRight, float iDeltaDown ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::MoveRight( float iDelta ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::MoveDown( float iDelta ) {
    // TODO
    OnChanged( mInfo );
}

// Rotation setter
void
IHasTransform::ResetRotation() {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetRotation( float iAngleRad ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::Rotate( float iAngleRad ) {
    // TODO
    OnChanged( mInfo );
}

// Scale setter
void
IHasTransform::ResetScale() {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetScale( const FVec2F& iValues ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetScale( float iX, float iY ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetScaleuniform( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetScaleX( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::SetScaleY( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::Stretch( const FVec2F& iValues ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::Stretch( float iX, float iY ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::StretchUniform( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::StretchX( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::StretchY( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::DeltaScale( const FVec2F& iValues ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::DeltaScale( float iX, float iY ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::DeltaScaleUniform( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::DeltaScaleX( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

void
IHasTransform::DeltaScaleY( float iValue ) {
    // TODO
    OnChanged( mInfo );
}

// Pivot Getter
const FVec2F&
IHasTransform::Pivot() const {
    return  mInfo.pivot;
}

float
IHasTransform::PivotX() const {
    return  mInfo.pivot.x;
}

float
IHasTransform::PivotY() const {
    return  mInfo.pivot.y;
}

// Translation Getter
const FVec2F&
IHasTransform::Translation() const {
    return  mInfo.translation;
}

float
IHasTransform::TranslationX() const {
    return  mInfo.translation.x;
}

float
IHasTransform::TranslationY() const {
    return  mInfo.translation.y;
}

// Rotation Getter
float
IHasTransform::Rotation() const {
    return  mInfo.rotation;
}

// Scale Getter
const FVec2F&
IHasTransform::Scale() const {
    return  mInfo.scale;
}

float
IHasTransform::ScaleX() const {
    return  mInfo.scale.x;
}

float
IHasTransform::ScaleY() const {
    return  mInfo.scale.y;
}

// Advanced Positioning
void
IHasTransform::TargetLocal( const FVec2F& iTarget ) {
}

void
IHasTransform::Target( const FVec2F& iTarget ) {
}

// Frame of ref API
FVec2F
IHasTransform::Right() const {
    return  FVec2F();
}

FVec2F
IHasTransform::Down() const {
    return  FVec2F();
}

FVec2F
IHasTransform::IntrinsicXAxis() const {
    return  FVec2F();
}

FVec2F
IHasTransform::IntrinsicYAxis() const {
    return  FVec2F();
}

FVec2F
IHasTransform::ExtrinsicXAxis() const {
    return  FVec2F();
}

FVec2F
IHasTransform::ExtrinsicYAxis() const {
    return  FVec2F();
}

// Conversion API
FVec3F
IHasTransform::ToLocal( const  FVec3F& iHPoint ) const {
    return  FVec3F();
}

FVec3F
IHasTransform::FromLocal( const  FVec3F& iHPoint ) const {
    return  FVec3F();
}

FVec2F
IHasTransform::PointToLocal( const FVec2F& iPoint ) const {
    return  FVec2F();
}

FVec2F
IHasTransform::PointFromLocal( const FVec2F& iPoint ) const {
    return  FVec2F();
}

FVec2F
IHasTransform::VecToLocal( const FVec2F& iVec ) const {
    return  FVec2F();
}

FVec2F
IHasTransform::VecFromLocal( const FVec2F& iVec ) const {
    return  FVec2F();
}

// Intermediate Matrix API
FMat3F
IHasTransform::PivotMatrix() const {
    return  FMat3F();
}

FMat3F
IHasTransform::TranslationMatrix() const {
    return  FMat3F();
}

FMat3F
IHasTransform::ScaleMatrix() const {
    return  FMat3F();
}

FMat3F
IHasTransform::RotationMatrix() const {
    return  FMat3F();
}

FMat3F
IHasTransform::Matrix() const {
    return  FMat3F();
}

FMat3F
IHasTransform::InverseMatrix() const {
    return  FMat3F();
}

ULIS_NAMESPACE_END

