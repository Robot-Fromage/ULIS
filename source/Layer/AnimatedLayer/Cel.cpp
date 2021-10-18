// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Cel.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for a basic animation CEL.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/AnimatedLayer/Cel.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
/// FCelExtension
FCelFauxFixe::FCelFauxFixe(
      eCelFauxFixeMode iMode
    , uint32 iExposure
    , eCelExtensionExposureType iType
)
    : mMode( iMode )
    , mExposure( iExposure )
    , mType( iType )
{}

bool
FCelFauxFixe::operator==( const FCelFauxFixe& iOther ) const {
    return  mMode == iOther.mMode
        &&  mExposure == iOther.mExposure
        &&  mType == iOther.mType;
}

eCelFauxFixeMode
FCelFauxFixe::Mode() const {
    return  mMode;
}

uint32
FCelFauxFixe::Exposure() const {
    return  mExposure;
}

eCelExtensionExposureType
FCelFauxFixe::Type() const {
    return  mType;
}

void
FCelFauxFixe::SetMode( eCelFauxFixeMode iValue ) {
    mMode = iValue;
}

void
FCelFauxFixe::SetExposure( uint32 iValue ) {
    mExposure = FMath::Max( uint32( 1 ), iValue );
}

void
FCelFauxFixe::SetType( eCelExtensionExposureType iValue ) {
    mType = iValue;
}

/////////////////////////////////////////////////////
/// FCelExtension
FCelExtension::FCelExtension(
      eCelExtensionBehaviour iBehaviour
    , const FCelFauxFixe& iFauxFixe
)
    : mBehaviour( iBehaviour )
    , mFauxFixe( iFauxFixe )
{}

bool
FCelExtension::operator==( const FCelExtension& iOther ) const {
    return  mBehaviour == iOther.mBehaviour
        &&  mFauxFixe == iOther.mFauxFixe;
}

eCelExtensionBehaviour
FCelExtension::Behaviour() const {
    return  mBehaviour;
}

const FCelFauxFixe&
FCelExtension::FauxFixe() const {
    return  mFauxFixe;
}

void
FCelExtension::SetBehaviour( eCelExtensionBehaviour iValue ) {
    mBehaviour = iValue;
}

void
FCelExtension::SetFauxFixe( const FCelFauxFixe& iValue ) {
    mFauxFixe = iValue;
}

/////////////////////////////////////////////////////
/// FCelInfo
FCelInfo::FCelInfo(
      uint32 iExposure
    , const FCelExtension& iPreBehaviour
    , const FCelExtension& iPostBehaviour
)
    : mExposure( FMath::Max( uint32( 1 ), iExposure ) )
    , mPreBehaviour( iPreBehaviour )
    , mPostBehaviour( iPostBehaviour )
{}

bool
FCelInfo::operator==( const FCelInfo& iOther ) const {
    return  mExposure == iOther.mExposure
        &&  mPreBehaviour == iOther.mPreBehaviour
        &&  mPostBehaviour == iOther.mPostBehaviour;
}

uint32
FCelInfo::Exposure() const {
    return  mExposure;
}

FCelExtension&
FCelInfo::PreBehaviour() {
    return  mPreBehaviour;
}

const FCelExtension&
FCelInfo::PreBehaviour() const {
    return  mPreBehaviour;
}

FCelExtension&
FCelInfo::PostBehaviour() {
    return  mPostBehaviour;
}

const FCelExtension&
FCelInfo::PostBehaviour() const {
    return  mPostBehaviour;
}

ULIS_NAMESPACE_END

