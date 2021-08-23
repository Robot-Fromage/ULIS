// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Transform2D.cpp
* @author       Clement Berthaud
* @brief        This file provides the Transform2D class definition.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Maths/Transform2D.h"
#include "Maths/Transform2D_Private.h"
ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FTransform2D
//----------------------------------------------------------------------------------------------
//------------------------------------------------------------------- Construction / Destruction
FTransform2D::~FTransform2D() {
    if( mImp )
        delete  mImp;
}

FTransform2D::FTransform2D()
    : mImp( new FTransform2D_imp() )
{
}

FTransform2D::FTransform2D( const FTransform2D& iOther )
    : mImp( new FTransform2D_imp( *iOther.mImp ) )
{
}

FTransform2D::FTransform2D( FTransform2D&& iOther )
{
    mImp = iOther.mImp;
    iOther.mImp = nullptr;
}

FTransform2D&
FTransform2D::operator=( const FTransform2D& iOther ) {
    *mImp = *iOther.mImp;
    return  *this;
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------- Public API
const FTransform2D::FTransform2D_imp&
FTransform2D::GetImp() const {
    return  *mImp;
}

const tByte*
FTransform2D::Ptr() const {
    return  mImp->Ptr();
}

FTransform2D
FTransform2D::Inverse() const {
    return  FTransform2D( new FTransform2D_imp( glm::inverse( mImp->Matrix() ) ) );
}

//----------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Private API
FTransform2D::FTransform2D( FTransform2D_imp* iVal )
    : mImp( iVal )
{
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------- Static API
//static
FTransform2D
FTransform2D::MakeFromMatrix( float iM00, float iM10, float iM20, float iM01, float iM11, float iM21, float iM02, float iM12, float iM22 ) {
    return  FTransform2D( new FTransform2D_imp( glm::mat3( iM00, iM10, iM20, iM01, iM11, iM21, iM02, iM12, iM22 ) ) );
}

//static
FTransform2D
FTransform2D::MakeIdentityTransform() {
    return  FTransform2D( new FTransform2D_imp( MakeIdentityMatrix() ) );
}

//static
FTransform2D
FTransform2D::MakeRotationTransform( float iAngleRag ) {
    return  FTransform2D( new FTransform2D_imp( MakeRotationMatrix( iAngleRag ) ) );
}

//static
FTransform2D
FTransform2D::MakeScaleTransform( float iX, float iY ) {
    return  FTransform2D( new FTransform2D_imp( MakeScaleMatrix( iX, iY ) ) );
}

//static
FTransform2D
FTransform2D::MakeShearTransform( float iX, float iY ) {
    return  FTransform2D( new FTransform2D_imp( MakeShearMatrix( iX, iY ) ) );
}

//static
FTransform2D
FTransform2D::MakeTranslationTransform( float iX, float iY ) {
    return  FTransform2D( new FTransform2D_imp( MakeTranslationMatrix( iX, iY ) ) );
}

//static
FTransform2D
FTransform2D::ComposeTransforms( const FTransform2D& iA, const FTransform2D& iB ) {
    return  FTransform2D( new FTransform2D_imp( ComposeMatrix( iA.GetImp().Matrix(), iB.GetImp().Matrix() ) ) );
}

//static
void
FTransform2D::DecomposeTransform( const FTransform2D& iTransform, float* oTx, float* oTy, float* oRotation, float* oScaleX, float* oScaleY, float* oSkewX, float* oSkewY ) {
    DecomposeMatrix( iTransform.GetImp().Matrix(), oTx, oTy, oRotation, oScaleX, oScaleY, oSkewX, oSkewY );
}

//static
FTransform2D
FTransform2D::GetPerspectiveTransform( const FVec2F iSrc[], const FVec2F iDst[] ) {
    return  FTransform2D( new FTransform2D_imp( GetPerspectiveMatrix( iSrc, iDst ) ) );
}

//static
FVec2F
FTransform2D::DoHomographyTransform( const FVec2F& iPoint, const FTransform2D& iTransform ) {
    return  HomographyTransform( iPoint, iTransform.GetImp().Matrix() );
}

ULIS3_NAMESPACE_END

