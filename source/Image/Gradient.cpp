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
#include "Image/Gradient.h"
#include "Memory/ContainerAlgorithms.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FSanitizedGradient
FSanitizedGradient::~FSanitizedGradient() {
}

FSanitizedGradient::FSanitizedGradient( eFormat iFormat, const FGradient& iGradient )
    : IHasFormat( iFormat )
    , IHasColorSpace( iGradient.ColorSpace() )
{
    // Save sizes as we will use them many times
    const uint64 inColorSize = iGradient.ColorSteps().Size();
    const uint64 inAlphaSize = iGradient.AlphaSteps().Size();

    // Reserve to avoid multiple reallocations, and + 2 for extra first and last
    // guards at params 0.f and 1.f in case they are missing.
    mColorSteps.Reserve( inColorSize + 2 );
    mAlphaSteps.Reserve( inAlphaSize + 2 );

    // Copy colors
    for( uint64 i = 0; i < inColorSize; ++i )
        mColorSteps.PushBack( *iGradient.ColorSteps()[i] );

    // Copy alpha
    for( uint64 i = 0; i < inAlphaSize; ++i )
        mAlphaSteps.PushBack( FAlphaStep( iGradient.AlphaSteps()[i]->Param(), FMath::Clamp( iGradient.AlphaSteps()[i]->Value(), 0.f, 1.f ) ) );

    // Make sure they are sorted by parameters in case the input gradient wasn't
    // sanitized or sorted.
    FContainerAlgorithms::BubbleSort( mColorSteps );
    FContainerAlgorithms::BubbleSort( mAlphaSteps );

    // Sanitize the color formats in case they weren't in the appropriate one.
    for( uint64 i = 0; i < mColorSteps.Size(); ++i ) {
        FColor& temp = mColorSteps[i].Value();
        temp = temp.ToFormat( Format() );
    }

    // Make sure there are at least the two end colors if the input gradient was empty,
    // that gives us a sanitized black to white gradient
    if( inColorSize == 0 ) {
        mColorSteps.PushBack( FColorStep( 0.f, FColor::Black.ToFormat( Format() ) ) );
        mColorSteps.PushBack( FColorStep( 1.f, FColor::White.ToFormat( Format() ) ) );
    }

    // Make sure there are at least the two end alphas if the input gradient was empty,
    // that gives us a sanitized fully transparent gradient
    if( inAlphaSize == 0 ) {
        mAlphaSteps.PushBack( FAlphaStep( 0.f, 1.f ) );
        mAlphaSteps.PushBack( FAlphaStep( 1.f, 1.f ) );
    }

    // Sanitize first color step, if missing at 0.f we duplicate the first one
    // to the very beginning.
    if( mColorSteps.Front().Param() > 0.f )
        mColorSteps.Insert( 0, FColorStep( 0.f, mColorSteps.Front().Value() ) );

    // Sanitize last color step, if missing at 1.f we duplicate the last one
    // to the very end.
    if( mColorSteps.Back().Param() < 1.f )
        mColorSteps.PushBack( FColorStep( 1.f, mColorSteps.Back().Value() ) );

    // Sanitize first alpha step, if missing at 0.f we duplicate the first one
    // to the very beginning.
    if( mAlphaSteps.Front().Param() > 0.f )
        mAlphaSteps.Insert( 0, FAlphaStep( 0.f, mAlphaSteps.Front().Value() ) );

    // Sanitize last alpha step, if missing at 1.f we duplicate the last one
    // to the very end.
    if( mAlphaSteps.Back().Param() < 1.f )
        mAlphaSteps.PushBack( FAlphaStep( 1.f, mAlphaSteps.Back().Value() ) );

    // Erase duplicate color first entries, keep only the latest one.
    while( mColorSteps[1].Param() == 0.f )
        mColorSteps.Erase( 0 );

    // Erase duplicate color  last entries, keep only the first one
    while( mColorSteps[ mColorSteps.Size() - 2 ].Param() == 1.f )
        mColorSteps.PopBack();

    // Erase duplicate alpha first entries, keep only the latest one.
    while( mAlphaSteps[1].Param() == 0.f )
        mAlphaSteps.Erase( 0 );

    // Erase duplicate alpha  last entries, keep only the first one
    while( mAlphaSteps[ mAlphaSteps.Size() - 2 ].Param() == 1.f )
        mAlphaSteps.PopBack();

    // Sanitize TArrays to occupy minimal memory.
    mColorSteps.Shrink();
    mAlphaSteps.Shrink();

    // Build color index LUT
    const uint64 maxColor = mColorSteps.Size() - 1;
    for( uint64 i = 0; i < maxColor; ++i ) {
        const FColorStep& prev = mColorSteps[i];
        const FColorStep& next = mColorSteps[i+1];
        const uint8 indPrev = uint8( prev.Param() * range );
        const uint8 indNext = uint8( next.Param() * range );
        for( uint8 j = indPrev; j < indNext; ++j )
            mIndexLUTColor[j] = static_cast< uint8 >( i );
    }

    // Build alpha index LUT
    const uint64 maxAlpha = mAlphaSteps.Size() - 1;
    for( uint64 i = 0; i < maxAlpha; ++i ) {
        const FAlphaStep& prev = mAlphaSteps[i];
        const FAlphaStep& next = mAlphaSteps[i+1];
        const uint8 indPrev = uint8( prev.Param() * range );
        const uint8 indNext = uint8( next.Param() * range );
        for( uint8 j = indPrev; j < indNext; ++j )
            mIndexLUTAlpha[j] = static_cast< uint8 >( i );
    }
}

const TArray< FColorStep >&
FSanitizedGradient::ColorSteps() const {
    return  mColorSteps;
}

const TArray< FAlphaStep >&
FSanitizedGradient::AlphaSteps() const {
    return  mAlphaSteps;
}

const uint8*
FSanitizedGradient::IndexLUTColor() const {
    return  mIndexLUTColor;
}

const uint8*
FSanitizedGradient::IndexLUTAlpha() const {
    return  mIndexLUTAlpha;
}

void
FSanitizedGradient::ReinterpretInterpolationFormat( eFormat iFormat ) {
    ReinterpretFormat( iFormat );
    for( uint64 i = 0; i < mColorSteps.Size(); ++i ) {
        FColor& temp = mColorSteps[i].Value();
        temp = temp.ToFormat( Format() );
    }
}

uint8
FSanitizedGradient::FastColorIndexAtParameter( ufloat iParam ) const {
    return  mIndexLUTColor[ uint8( FMath::Clamp( iParam, 0.f, 1.f ) * ( range - 1 ) ) ];
}

uint8
FSanitizedGradient::FastAlphaIndexAtParameter( ufloat iParam ) const {
    return  mIndexLUTAlpha[ uint8( FMath::Clamp( iParam, 0.f, 1.f ) * ( range - 1 ) ) ];
}

/////////////////////////////////////////////////////
// FGradient
FGradient::~FGradient()
{
}

FGradient::FGradient( eFormat iFormat )
    : IHasFormat( iFormat )
    , IHasColorSpace()
{
}

TArray< FSharedColorStep >&
FGradient::ColorSteps()
{
    return  mColorSteps;
}

const TArray< FSharedColorStep >&
FGradient::ColorSteps() const
{
    return  mColorSteps;
}

TArray< FSharedAlphaStep >&
FGradient::AlphaSteps()
{
    return  mAlphaSteps;
}

const TArray< FSharedAlphaStep >&
FGradient::AlphaSteps() const
{
    return  mAlphaSteps;
}

void
FGradient::Reset()
{
    mColorSteps.Clear();
    mAlphaSteps.Clear();
}

void
FGradient::Reset( eFormat iFormat )
{
    ReinterpretFormat( iFormat );
    Reset();
}

void
FGradient::ReinterpretInterpolationFormat( eFormat iFormat )
{
    ReinterpretFormat( iFormat );
    for( uint64 i = 0; i < mColorSteps.Size(); ++i ) {
        FColor& temp = mColorSteps[i]->Value();
        temp = temp.ToFormat( Format() );
    }
}

void
FGradient::AddColorStep( ufloat iParam, const ISample& iColor )
{
    mColorSteps.PushBack( FColorStep::MakeShared( iParam, iColor ) );
}

void
FGradient::AddAlphaStep( ufloat iParam, ufloat iAlpha )
{
    mAlphaSteps.PushBack( FAlphaStep::MakeShared( iParam, iAlpha ) );
}

FSanitizedGradient
FGradient::Sanitized() const {
    return  FSanitizedGradient( Format(), *this );
}

FSanitizedGradient
FGradient::Sanitized( eFormat iFormat ) const {
    return  FSanitizedGradient( iFormat, *this );
}

void
FGradient::Sort()
{
    mColorSteps.Shrink();
    mAlphaSteps.Shrink();
    FContainerAlgorithms::BubbleSort( mColorSteps );
    FContainerAlgorithms::BubbleSort( mAlphaSteps );
    ReinterpretInterpolationFormat( Format() );
}

ULIS_NAMESPACE_END

