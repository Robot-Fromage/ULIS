// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Gradient.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FGradient class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Image/Gradient.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FGradientStep
FGradientStep::~FGradientStep()
{
}

FGradientStep::FGradientStep()
    : mStep( 0.f )
    , mColor()
{
}

FGradientStep::FGradientStep( const FColor& iColor, ufloat iStep )
    : mStep( iStep )
    , mColor( iColor )
{
}

void
FGradientStep::Step( ufloat iValue )
{
    ULIS_ASSERT( iValue >= 0.f && iValue <= 1.f, "Error bad clamping" );
    mStep = iValue;
}

ufloat
FGradientStep::Step() const
{
    return  mStep;
}

FColor&
FGradientStep::Color()
{
    return  mColor;
}

const
FColor& FGradientStep::Color() const
{
    return  mColor;
}

/////////////////////////////////////////////////////
// FGradient
FGradient::~FGradient()
{
}

FGradient::FGradient( eFormat iFormat )
    : mFormat( iFormat )
{
    mSteps.PushBack( FGradientStep( FColor::GreyAF( 1.f ).ToFormat( mFormat ), 0.f ) );
    mSteps.PushBack( FGradientStep( FColor::GreyAF( 0.f ).ToFormat( mFormat ), 1.f ) );
}

const TArray< FGradientStep >&
FGradient::Steps() const
{
    return  mSteps;
}

void
FGradient::Reset()
{
    mSteps.Clear();
    mSteps.PushBack( FGradientStep( FColor::GreyAF( 1.f ).ToFormat( mFormat ), 0.f ) );
    mSteps.PushBack( FGradientStep( FColor::GreyAF( 0.f ).ToFormat( mFormat ), 1.f ) );
}

void
FGradient::Reset( eFormat iFormat )
{
    mFormat = iFormat;
    Reset();
}

void
FGradient::ReinterpretInterpolationFormat( eFormat iFormat )
{
    mFormat = iFormat;
    for( uint64 i = 0; i < mSteps.Size(); ++i )
        mSteps[i].Color().ToFormat( mFormat );
}

uint64
FGradient::AddStep( ufloat iStep, const ISample& iValue )
{
    ufloat step = FMath::Clamp( iStep, std::numeric_limits< float >::min(), 1.f );
    uint64 index = 0;
    for( uint64 i = 0; i < mSteps.Size(); ++i ) {
        if( mSteps[i].Step() >= step ) {
            index = i;
            break;
        }
    }
    mSteps.Insert( index, FGradientStep( iValue.ToFormat( mFormat ), step ) );
    return  index;
}

void
FGradient::EraseStep( uint64 iIndex )
{
    mSteps.Erase( iIndex );
}

uint64
FGradient::NumSteps() const
{
    return  mSteps.Size();
}

FColor
FGradient::ColorAtStep( ufloat iStep ) const
{
    ufloat step = FMath::Clamp( iStep, std::numeric_limits< float >::min(), 1.f );
    uint64 upper = 0;
    for( uint64 i = 0; i < mSteps.Size(); ++i ) {
        if( mSteps[i].Step() >= step ) {
            upper = i;
            break;
        }
    }
    uint64 lower = upper - 1;
    ufloat delta = mSteps[ upper ].Step() - mSteps[ lower ].Step();
    ufloat t = step / delta;
    return  FColor::MixFormat( mSteps[ lower ].Color(), mSteps[ upper ].Color(), mFormat, t );
}

ULIS_NAMESPACE_END

