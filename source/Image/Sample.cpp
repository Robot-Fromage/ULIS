// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Sample.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the ISample classes.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Image/Sample.h"
#include "Image/Color.h"
#include "Image/Pixel.h"
#include "Process/Conv/Conv.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Implementation details
namespace detail {
template< typename T >
FColor
MixFormat_imp( const ISample& iA, const ISample& iB, eFormat iFormat, ufloat iLerpParameter  )
{
    ufloat t = FMath::Clamp( iLerpParameter, 0.f, 1.f );
    ufloat u = 1.f - t;
    FColor res( iFormat );
    FFormatMetrics fmt( iFormat );
    for( uint8 i = 0; i < fmt.SPP; ++i )
        res.SetChannelT< T >( i, static_cast< T >( u * iA.ChannelT< T >( i ) + t * iB.ChannelT< T >( i ) ) );
    return  res;
}

template< typename T >
void
Premultiply_imp( ISample& iSample )
{
    const FFormatMetrics& fmt = iSample.FormatMetrics();
    ufloat alpha = static_cast< float >( iSample.AlphaT< T >() );
    ufloat maxtype = static_cast< float >( MaxType< T >() );
    for( uint8 i = 0; i < fmt.NCC; ++i )
        iSample.SetChannelT< T >( i, static_cast< T >( ( iSample.ChannelT< T >( i ) * alpha ) / MaxType< T >() ) );
}

template< typename T >
void
Unpremultiply_imp( ISample& iSample )
{
    const FFormatMetrics& fmt = iSample.FormatMetrics();
    ufloat alpha = static_cast< float >( iSample.AlphaT< T >() );
    ufloat maxtype = static_cast< float >( MaxType< T >() );

    if( alpha == 0.f ){
        for( uint8 i = 0; i < fmt.NCC; ++i )
            iSample.SetChannelT< T >( i, static_cast< T >( 0 ) );
    } else {
        for( uint8 i = 0; i < fmt.NCC; ++i )
            iSample.SetChannelT< T >( i, static_cast< T >( ( iSample.ChannelT< T >( i ) * MaxType< T >() ) / alpha ) );
    }
}

} // namespace detail

ISample::~ISample()
{}

ISample::ISample( uint8* iData, eFormat iFormat, const FColorSpace* iColorSpace )
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , mSignal( iData )
{
}

bool
ISample::operator==( const  ISample& iOther )  const {

    if( Format() != iOther.Format() )
        return  false;

    bool bytePerfectMatch = true;
    for( int i = 0; i < BytesPerPixel(); ++i ) {
        if( mSignal[i] != iOther.mSignal[i] ) {
            bytePerfectMatch = false;
            break;
        }
    }

    return  bytePerfectMatch;
}

bool
ISample::operator!=( const  ISample& iOther )  const {
    return  !( *this == iOther );
}

uint8*
ISample::Bits()
{
    return  mSignal;
}

const uint8*
ISample::Bits() const
{
    return  mSignal;
}

FColor
ISample::ToFormat( eFormat iDstFormat ) const
{
    FColor dst( iDstFormat );
    if( Format() == iDstFormat ) {
        memcpy( dst.Bits(), Bits(), dst.BytesPerPixel() );
    } else {
        fpConvertFormat fptr = QueryDispatchedConvertFormatInvocation( Format(), iDstFormat );
        fptr( FPixel( *this ), FPixel( dst ), 1 );
    }
    return  dst;
}

//static
void
ISample::ConvertFormat( const ISample& iSrc, ISample& iDst )
{
    if( iSrc.Format() == iDst.Format() ) {
        memcpy( iDst.Bits(), iSrc.Bits(), iDst.BytesPerPixel() );
    } else {
        fpConvertFormat fptr = QueryDispatchedConvertFormatInvocation( iSrc.Format(), iDst.Format() );
        fptr( FPixel( iSrc ), FPixel( iDst ), 1 );
    }
}

//static
FColor
ISample::MixFormat( const ISample& iA, const ISample& iB, eFormat iFormat, ufloat iLerpParameter )
{
    FColor tmp0 = iA.ToFormat( iFormat );
    FColor tmp1 = iB.ToFormat( iFormat );
    #define TMP_CALL( _TYPE_ID, _TYPE, _E2, _E3 ) return  detail::MixFormat_imp< _TYPE >( tmp0, tmp1, iFormat, iLerpParameter );
    ULIS_SWITCH_FOR_ALL_DO( static_cast< eType >( ULIS_R_TYPE( iFormat ) ), ULIS_FOR_ALL_TYPES_ID_DO, TMP_CALL, 0, 0, 0 )
    #undef TMP_CALL

    // Shut warning C4715, static analysis cannot figure all paths are handled.
    ULIS_ASSERT( false, "Bad control path, no spec for typeid." );
    return  FColor();
}

FColor
ISample::MixRGB( const ISample& iA, const ISample& iB, ufloat iLerpParameter )
{
    ufloat t = FMath::Clamp( iLerpParameter, 0.f, 1.f );
    FColor tmp0 = iA.ToFormat( eFormat::Format_RGBAF );
    FColor tmp1 = iB.ToFormat( eFormat::Format_RGBAF );
    FColor res( eFormat::Format_RGBAF );
    res.SetRF( ( 1.f - t ) * tmp0.RF() + t * tmp1.RF() );
    res.SetGF( ( 1.f - t ) * tmp0.GF() + t * tmp1.GF() );
    res.SetBF( ( 1.f - t ) * tmp0.BF() + t * tmp1.BF() );
    res.SetAF( ( 1.f - t ) * tmp0.AF() + t * tmp1.AF() );
    return  res;
}

FColor
ISample::MixLab( const ISample& iA, const ISample& iB, ufloat iLerpParameter )
{
    ufloat t = FMath::Clamp( iLerpParameter, 0.f, 1.f );
    FColor tmp0 = iA.ToFormat( eFormat::Format_LabAF );
    FColor tmp1 = iB.ToFormat( eFormat::Format_LabAF );
    FColor res( eFormat::Format_RGBAF );
    res.SetLF( ( 1.f - t ) * tmp0.LF() + t * tmp1.RF() );
    res.SetaF( ( 1.f - t ) * tmp0.aF() + t * tmp1.GF() );
    res.SetbF( ( 1.f - t ) * tmp0.bF() + t * tmp1.BF() );
    res.SetAF( ( 1.f - t ) * tmp0.AF() + t * tmp1.AF() );
    return  res;
}

void
ISample::Premultiply()
{
    #define TMP_CALL( _TYPE_ID, _TYPE, _E2, _E3 ) return  detail::Premultiply_imp< _TYPE >( *this );
    ULIS_SWITCH_FOR_ALL_DO( Type(), ULIS_FOR_ALL_TYPES_ID_DO, TMP_CALL, 0, 0, 0 )
    #undef TMP_CALL
}

void
ISample::Unpremultiply()
{
    #define TMP_CALL( _TYPE_ID, _TYPE, _E2, _E3 ) return  detail::Unpremultiply_imp< _TYPE >( *this );
    ULIS_SWITCH_FOR_ALL_DO( Type(), ULIS_FOR_ALL_TYPES_ID_DO, TMP_CALL, 0, 0, 0 )
    #undef TMP_CALL
}

FColor
ISample::Premultiplied() const
{
    FColor res = *this;
    res.Premultiply();
    return  res;
}

FColor
ISample::Unpremultiplied() const
{
    FColor res = *this;
    res.Unpremultiply();
    return  res;
}

ULIS_NAMESPACE_END

