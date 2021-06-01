// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Layer.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the ILayer class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Layer.h"

ULIS_NAMESPACE_BEGIN
ILayer::~ILayer()
{
}

ILayer::ILayer(
      const FString& iName
    , bool iLocked
    , bool iVisible
    , const FColor& iColor
    , tParent* iParent
)
    : tSuperClass( iParent )
    , mName( iName )
    , mLocked( iLocked )
    , mVisible( iVisible )
    , mColor( iColor.ToFormat( Format_RGBA8 ) )
{}

const FString&
ILayer::Name() const
{
    return  mName;
}

bool
ILayer::Locked() const
{
    return  mLocked;
}

bool
ILayer::Visible() const
{
    return  mVisible;
}

const FColor&
ILayer::Color() const
{
    // supposed to be always RGBA8
    return  mColor;
}

void
ILayer::SetName( const FString& iName )
{
    mName = iName;
}

void
ILayer::SetLocked( bool iValue )
{
    mLocked = iValue;
}

void
ILayer::SetVisible( bool iValue )
{
    mVisible = iValue;
}

void
ILayer::SetColor( const FColor& iColor )
{
    // Ensure conversion to local mColor format,
    // supposed to be always RGBA8
    FColor::ConvertFormat( iColor, mColor );
}

ULIS_NAMESPACE_END

