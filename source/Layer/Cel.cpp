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
#pragma once
#include "Layer/Cel.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// FCelExtension
FCelExtension::FCelExtension(
      uint32 iExposure
    , eCelExtensionExposureType iType
    , eCelExtensionBehaviour iBehaviour
)
    : mExposure( FMath::Max( uint32( 1 ), iExposure ) )
    , mType( iType )
    , mBehaviour( iBehaviour )
{}

uint32
FCelExtension::Exposure() const {
    return  mExposure;
}

eCelExtensionExposureType
FCelExtension::Type() const {
    return  mType;
}

eCelExtensionBehaviour
FCelExtension::Behaviour() const {
    return  mBehaviour;
}
void
FCelExtension::SetExposure( uint32 iValue ) {
    mExposure = FMath::Max( uint32( 1 ), iValue );
}

void
FCelExtension::SetType( eCelExtensionExposureType iValue ) {
    mType = iValue;
}

void
FCelExtension::SetBehaviour( eCelExtensionBehaviour iValue ) {
    mBehaviour = iValue;
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

