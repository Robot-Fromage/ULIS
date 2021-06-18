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
    : FOnTransformInfoChanged( iDelegate )
    , mInfo {
          iPivot
        , iTranslation
        , iRotation
        , iScale
    }
{
    Invoke( mInfo );
}

// Pivot setter
void
IHasTransform::ResetPivot() {
    mInfo.pivot = FVec2F( 0 );
    Invoke( mInfo );
}

void
IHasTransform::SetPivot( const FVec2F& iValues ) {
    mInfo.pivot = iValues;
    Invoke( mInfo );
}

void
IHasTransform::SetPivot( float iX, float iY ) {
    mInfo.pivot = ( iX, iY );
    Invoke( mInfo );
}

void
IHasTransform::SetPivotX( float iValue ) {
    mInfo.pivot.x = iValue;
    Invoke( mInfo );
}

void
IHasTransform::SetPivotY( float iValue ) {
    mInfo.pivot.y = iValue;
    Invoke( mInfo );
}

void
IHasTransform::MovePivot( const FVec2F& iDelta ) {
    mInfo.pivot += iDelta;
    Invoke( mInfo );
}

void
IHasTransform::MovePivot( float iDeltaX, float iDeltaY ) {
    mInfo.pivot.x += iDeltaX;
    mInfo.pivot.y += iDeltaY;
    Invoke( mInfo );
}

void
IHasTransform::MovePivotX( float iDelta ) {
    mInfo.pivot.x += iDelta;
    Invoke( mInfo );
}

void
IHasTransform::MovePivotY( float iDelta ) {
    mInfo.pivot.y += iDelta;
    Invoke( mInfo );
}

void
IHasTransform::MovePivotRight( float iDelta ) {
    mInfo.pivot += Right() * iDelta;
    Invoke( mInfo );
}

void
IHasTransform::MovePivotDown( float iDelta ) {
    mInfo.pivot += Down() * iDelta;
    Invoke( mInfo );
}

// Translation setter
void
IHasTransform::ResetTranslation() {
    mInfo.translation = FVec2F( 0 );
    Invoke( mInfo );
}

void
IHasTransform::SetTranslation( const FVec2F& iValues ) {
    mInfo.translation = iValues;
    Invoke( mInfo );
}
void
IHasTransform::SetTranslation( float iX, float iY ) {
    mInfo.translation = ( iX, iY );
    Invoke( mInfo );
}

void
IHasTransform::SetTranslationX( float iValue ) {
    mInfo.translation.x = iValue;
    Invoke( mInfo );
}

void
IHasTransform::SetTranslationY( float iValue ) {
    mInfo.translation.y = iValue;
    Invoke( mInfo );
}

void
IHasTransform::Translate( const FVec2F& iDelta ) {
    mInfo.translation += iDelta;
    Invoke( mInfo );
}

void
IHasTransform::Translate( float iDeltaX, float iDeltaY ) {
    mInfo.translation.x += iDeltaX;
    mInfo.translation.y += iDeltaY;
    Invoke( mInfo );
}

void
IHasTransform::TranslateX( float iDelta ) {
    mInfo.translation.x += iDelta;
    Invoke( mInfo );
}

void
IHasTransform::TranslateY( float iDelta ) {
    mInfo.translation.y += iDelta;
    Invoke( mInfo );
}

void
IHasTransform::Move( const FVec2F& iDelta ) {
    mInfo.translation += VecFromLocal( iDelta );
}

void
IHasTransform::Move( float iDeltaRight, float iDeltaDown ) {
    mInfo.translation += VecFromLocal( FVec2F( iDeltaRight, iDeltaDown ) );
    Invoke( mInfo );
}

void
IHasTransform::MoveRight( float iDelta ) {
    mInfo.translation += Right() * iDelta;
    Invoke( mInfo );
}

void
IHasTransform::MoveDown( float iDelta ) {
    mInfo.translation += Down() * iDelta;
    Invoke( mInfo );
}

// Rotation setter
void
IHasTransform::ResetRotation() {
    mInfo.rotation = 0;
    Invoke( mInfo );
}

void
IHasTransform::SetRotation( float iAngleRad ) {
    mInfo.rotation = iAngleRad;
    Invoke( mInfo );
}

void
IHasTransform::Rotate( float iAngleRad ) {
    mInfo.rotation += iAngleRad;
    Invoke( mInfo );
}

// Scale setter
void
IHasTransform::ResetScale() {
    mInfo.scale = FVec2F( 1 );
    Invoke( mInfo );
}

void
IHasTransform::SetScale( const FVec2F& iValues ) {
    mInfo.scale = iValues;
    Invoke( mInfo );
}

void
IHasTransform::SetScale( float iX, float iY ) {
    mInfo.scale = ( iX, iY );
    Invoke( mInfo );
}

void
IHasTransform::SetScaleuniform( float iValue ) {
    mInfo.scale = ( iValue, iValue );
    Invoke( mInfo );
}

void
IHasTransform::SetScaleX( float iValue ) {
    mInfo.scale.x = iValue;
    Invoke( mInfo );
}

void
IHasTransform::SetScaleY( float iValue ) {
    mInfo.scale.y = iValue;
    Invoke( mInfo );
}

void
IHasTransform::Stretch( const FVec2F& iValues ) {
    mInfo.scale *= iValues;
    Invoke( mInfo );
}

void
IHasTransform::Stretch( float iX, float iY ) {
    mInfo.scale *= ( iX, iY );
    Invoke( mInfo );
}

void
IHasTransform::StretchUniform( float iValue ) {
    mInfo.scale *= FVec2F( iValue );
    Invoke( mInfo );
}

void
IHasTransform::StretchX( float iValue ) {
    mInfo.scale.x *= iValue;
    Invoke( mInfo );
}

void
IHasTransform::StretchY( float iValue ) {
    mInfo.scale.y *= iValue;
    Invoke( mInfo );
}

void
IHasTransform::DeltaScale( const FVec2F& iValues ) {
    mInfo.scale += iValues;
    Invoke( mInfo );
}

void
IHasTransform::DeltaScale( float iX, float iY ) {
    mInfo.scale += ( iX, iY );
    Invoke( mInfo );
}

void
IHasTransform::DeltaScaleUniform( float iValue ) {
    mInfo.scale += ( iValue, iValue );
    Invoke( mInfo );
}

void
IHasTransform::DeltaScaleX( float iValue ) {
    mInfo.scale.x += iValue;
    Invoke( mInfo );
}

void
IHasTransform::DeltaScaleY( float iValue ) {
    mInfo.scale.y += iValue;
    Invoke( mInfo );
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
    if( iTarget != FVec2F(0) )
    {
    // produit scalaire & vecto à chercher dans FVec2F
    }
}

// Frame of ref API
FVec2F
IHasTransform::Right() const {
    return FVec2F( Matrix() * FVec3F( 1, 0, 0 ) ).Normalize();
}

FVec2F
IHasTransform::Down() const {
    return FVec2F( Matrix() * FVec3F( 0, 1, 0 ) ).Normalize();
}

FVec2F
IHasTransform::LocalXAxis() const {
    return FVec2F( 1, 0 );
}

FVec2F
IHasTransform::LocalYAxis() const {
    return FVec2F( 0, 1 );
}

FVec2F
IHasTransform::ExternalXAxis() const {
    return FVec2F( InverseMatrix() * FVec3F( 1, 0, 0 ) ).Normalize();
}

FVec2F
IHasTransform::ExternalYAxis() const {
    return FVec2F( InverseMatrix() * FVec3F( 0, 1, 0 ) ).Normalize();
}

// Conversion API
FVec3F
IHasTransform::ToLocal( const  FVec3F& iHPoint ) const {
    return FVec3F( Matrix() * iHPoint );
}

FVec3F
IHasTransform::FromLocal( const  FVec3F& iHPoint ) const {
    return FVec3F( InverseMatrix() * iHPoint );
}

FVec2F
IHasTransform::PointToLocal( const FVec2F& iPoint ) const {
    return FVec2F( Matrix() * FVec3F( iPoint, 1 ) );
}

FVec2F
IHasTransform::PointFromLocal( const FVec2F& iPoint ) const {
    return FVec2F( InverseMatrix() * FVec3F( iPoint, 1 ) );
}

FVec2F
IHasTransform::VecToLocal( const FVec2F& iVec ) const {
    return FVec2F( Matrix() * FVec3F( iVec, 0 ) );
}

FVec2F
IHasTransform::VecFromLocal( const FVec2F& iVec ) const {
    return FVec2F( InverseMatrix() * FVec3F( iVec, 0 ) );
}

// Intermediate Matrix API
FMat3F
IHasTransform::PivotMatrix() const {
    return  FMat3F::MakeTranslationMatrix( mInfo.pivot.x, mInfo.pivot.y );
}

FMat3F
IHasTransform::TranslationMatrix() const {
    return  FMat3F::MakeTranslationMatrix( mInfo.translation.x, mInfo.translation.y );
}

FMat3F
IHasTransform::ScaleMatrix() const {
    return FMat3F::MakeScaleMatrix( mInfo.scale.x, mInfo.scale.y );
}

FMat3F
IHasTransform::RotationMatrix() const {
    return  FMat3F::MakeRotationMatrix( mInfo.rotation ) ;
}

FMat3F
IHasTransform::Matrix() const {
    return  IHasTransform::PivotMatrix() * IHasTransform::RotationMatrix() * IHasTransform::ScaleMatrix() * IHasTransform::TranslationMatrix() * IHasTransform::PivotMatrix().Inverse() ;
}

FMat3F
IHasTransform::InverseMatrix() const {
    return  IHasTransform::Matrix().Inverse();
}

ULIS_NAMESPACE_END

