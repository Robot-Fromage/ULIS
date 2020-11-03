// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Transform2D.cpp
* @author       Clement Berthaud
* @brief        This file provides the Transform2D class definition.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Maths/Transform2D.h"
#include "Data/Block.h"
#include "Maths/Maths.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/vec2.hpp>

#include <Eigen/Core>
#include <Eigen/Dense>

ULIS2_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FTransform2D
//----------------------------------------------------------------------------------------------
//------------------------------------------------------------------- Construction / Destruction
FTransform2D::FTransform2D()
    : mMatrix( 1.f )
    , mDirtyID( true )
    , mDirtyInverseMatrix( true )
{
}


FTransform2D::FTransform2D( const glm::mat3& iMat )
    : mMatrix( iMat )
    , mDirtyID( true )
    , mDirtyInverseMatrix( true )
{
}


//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------- Public API
const std::string&
FTransform2D::ID() const {
    UpdateID();
    return  mID;
}


const glm::mat3&
FTransform2D::Matrix() const {
     return  mMatrix;
}


const glm::mat3&
FTransform2D::InverseMatrix() const {
    UpdateInverseMatrix();
    return  mInverseMatrix;
}

void
FTransform2D::Decompose( float* iTx, float* iTy, float* iRotation, float* iScaleX, float* iScaleY, float* iSkewX, float* iSkewY ) const {
    DecomposeMatrix( mMatrix, iTx, iTy, iRotation, iScaleX, iScaleY, iSkewX, iSkewY );
}

//----------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- Private API
void
FTransform2D::UpdateID() const {
    if( !mDirtyID )
        return;

    constexpr auto sz   = sizeof( glm::mat3 );
    char* buf           = new char[ sz + 1 ];
    memcpy( buf, glm::value_ptr( mMatrix ), sz );
    buf[sz]     = 0;
    mID         = buf;
    mDirtyID    = false;
    delete [] buf;
}


void
FTransform2D::UpdateInverseMatrix() const {
    if( !mDirtyInverseMatrix )
        return;

    mInverseMatrix = glm::inverse( mMatrix );
    mDirtyInverseMatrix = false;
}

/////////////////////////////////////////////////////
// Makers
glm::mat3
MakeIdentityMatrix()
{
    return  glm::mat3(1.f);
}


glm::mat3
MakeRotationMatrix( float iAngleRag )
{
    return  glm::rotate( glm::mat3(1.f), iAngleRag );
}


glm::mat3
MakeScaleMatrix( float iX, float iY )
{
    return  glm::scale( glm::mat3( 1.f ), glm::vec2( iX, iY ) );
}


glm::mat3
MakeShearMatrix( float iX, float iY )
{
    return  glm::shearY( glm::shearX( glm::mat3(1.f), iX ), iY );
}


glm::mat3
MakeTranslationMatrix( float iX, float iY )
{
    return  glm::translate( glm::mat3( 1.f), glm::vec2( iX, iY ) );
}


glm::mat3
ComposeMatrix( const glm::mat3& iA, const glm::mat3& iB )
{
    // Non commutative operation order matters.
    return  iB * iA;
}

void
DecomposeMatrix( const glm::mat3& iMat, float* iTx, float* iTy, float* iRotation, float* iScaleX, float* iScaleY, float* iSkewX, float* iSkewY ) {
    float a = iMat[0][0];
    float b = iMat[0][1];
    float c = iMat[1][0];
    float d = iMat[1][1];
    float e = iMat[2][0];
    float f = iMat[2][1];
    float delta = a * d - b * c;

    float tx = e;
    float ty = f;
    float rot = 0.f;
    float scx = 0.f;
    float scy = 0.f;
    float skx = 0.f;
    float sky = 0.f;
    // Apply the QR-like decomposition.
    if( a != 0.f || b != 0.f ) {
        float r = sqrtf( a * a + b * b );
        rot = FMaths::Sign( b ) * acosf( a / r );
        scx = r;
        scy = delta / r;
        skx = atanf( ( a * c + b * d ) / ( r * r ) );
        sky = 0.f;
    } else if( c != 0.f || d != 0.f ) {
        float s = sqrtf( c * c + d * d );
        rot = FMaths::kPIf / 2.f - ( d > 0 ? acosf( -c / s) : - acosf( c / s ) );
        scx = delta / s;
        scy = s;
        skx = 0.f;
        sky = atanf( ( a * c + b * d ) / ( s * s ) );
    } else {
    }
    *iTx        = tx;
    *iTy        = ty;
    *iRotation  = rot;
    *iScaleX    = scx;
    *iScaleY    = scy;
    *iSkewX     = skx;
    *iSkewY     = sky;
}


glm::mat3 GetPerspectiveMatrix( const FVec2F iSrc[], const FVec2F iDst[] ) {
    //      c00*xi + c01*yi + c02
    // ui = ---------------------
    //      c20*xi + c21*yi + c22
    //      c10*xi + c11*yi + c12
    // vi = ---------------------
    //      c20*xi + c21*yi + c22
    // Coefficients are calculated by solving linear system:
    // / x0 y0  1  0  0  0 -x0*u0 -y0*u0 \ /c00\ /u0\
    // | x1 y1  1  0  0  0 -x1*u1 -y1*u1 | |c01| |u1|
    // | x2 y2  1  0  0  0 -x2*u2 -y2*u2 | |c02| |u2|
    // | x3 y3  1  0  0  0 -x3*u3 -y3*u3 |.|c10|=|u3|
    // |  0  0  0 x2 y2  1 -x2*v2 -y2*v2 | |c20| |v2|
    // |  0  0  0 x0 y0  1 -x0*v0 -y0*v0 | |c11| |v0|
    // |  0  0  0 x1 y1  1 -x1*v1 -y1*v1 | |c12| |v1|
    // \  0  0  0 x3 y3  1 -x3*v3 -y3*v3 / \c21/ \v3/
    // CB:| It would be faster if we solve the system by hand, eliminating any redundant calculations, but i don't have time for this right now.
    // Use inverse matrix approach instead:
    // /c00\ / x0 y0  1  0  0  0 -x0*u0 -y0*u0 \-1  /u0\
    // |c01| | x1 y1  1  0  0  0 -x1*u1 -y1*u1 |    |u1|
    // |c02| | x2 y2  1  0  0  0 -x2*u2 -y2*u2 |    |u2|
    // |c10|=| x3 y3  1  0  0  0 -x3*u3 -y3*u3 |  * |u3|
    // |c20| |  0  0  0 x2 y2  1 -x2*v2 -y2*v2 |    |v2|
    // |c11| |  0  0  0 x0 y0  1 -x0*v0 -y0*v0 |    |v0|
    // |c12| |  0  0  0 x1 y1  1 -x1*v1 -y1*v1 |    |v1|
    // \c21/ \  0  0  0 x3 y3  1 -x3*v3 -y3*v3 /    \v3/
    Eigen::Matrix< double, 8, 8, Eigen::RowMajor > mat;
    mat << iSrc[0].x, iSrc[0].y, 1, 0, 0, 0, -iSrc[0].x * iDst[0].x, - iSrc[0].y * iDst[0].x
         , iSrc[1].x, iSrc[1].y, 1, 0, 0, 0, -iSrc[1].x * iDst[1].x, - iSrc[1].y * iDst[1].x
         , iSrc[2].x, iSrc[2].y, 1, 0, 0, 0, -iSrc[2].x * iDst[2].x, - iSrc[2].y * iDst[2].x
         , iSrc[3].x, iSrc[3].y, 1, 0, 0, 0, -iSrc[3].x * iDst[3].x, - iSrc[3].y * iDst[3].x
         , 0, 0, 0, iSrc[2].x, iSrc[2].y, 1, -iSrc[2].x * iDst[2].y, - iSrc[2].y * iDst[2].y
         , 0, 0, 0, iSrc[0].x, iSrc[0].y, 1, -iSrc[0].x * iDst[0].y, - iSrc[0].y * iDst[0].y
         , 0, 0, 0, iSrc[1].x, iSrc[1].y, 1, -iSrc[1].x * iDst[1].y, - iSrc[1].y * iDst[1].y
         , 0, 0, 0, iSrc[3].x, iSrc[3].y, 1, -iSrc[3].x * iDst[3].y, - iSrc[3].y * iDst[3].y;
    Eigen::Vector< double, 8 > vec( iDst[0].x, iDst[1].x, iDst[2].x, iDst[3].x, iDst[2].y, iDst[0].y, iDst[1].y, iDst[3].y );
    Eigen::Matrix< double, 8, 8 > inverse = mat.inverse();
    Eigen::Vector< double, 8 > res = inverse * vec;
    return  glm::mat3( res[0]
                     , res[1]
                     , res[2]
                     , res[3]
                     , res[4]
                     , res[5]
                     , res[6]
                     , res[7]
                     , 1 );
}

FVec2F HomographyTransform( const FVec2F& iPoint, const glm::mat3& iMat ) {
    return  FVec2F( ( iPoint.x * iMat[0][0] + iPoint.y * iMat[0][1] + iMat[0][2] ) / ( iPoint.x * iMat[2][0] + iPoint.y * iMat[2][1] + iMat[2][2] )
                  , ( iPoint.x * iMat[1][0] + iPoint.y * iMat[1][1] + iMat[1][2] ) / ( iPoint.x * iMat[2][0] + iPoint.y * iMat[2][1] + iMat[2][2] ) );
}

ULIS2_NAMESPACE_END

