// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Pixel.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the Pixel classes.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Data/Pixel.h"
#include "Maths/Maths.h"

ULIS3_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// IPixel
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
IPixel::~IPixel()
{
}


IPixel::IPixel( tFormat iFormat, FColorProfile* iProfile )
    : mData( nullptr )
    , mInfo( iFormat )
    , mProfile( iProfile )
{
}

//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Comparison Operators
bool
IPixel::operator==( const  IPixel& iOther )  const {
    bool sameFormat = mInfo.FMT == iOther.mInfo.FMT;
    bool bytePerfectMatch = true;
    for( int i = 0; i < mInfo.BPP; ++i ) {
        if( mData[i] != iOther.mData[i] ) {
            bytePerfectMatch = false;
            break;
        }
    }

    return  sameFormat && bytePerfectMatch;
}


bool
IPixel::operator!=( const  IPixel& iOther )  const {
    return  !( *this == iOther );
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
tByte*
IPixel::Ptr()
{
    return  mData;
}


void
IPixel::AssignProfile( FColorProfile* iProfile )
{
    mProfile = iProfile;
}


const tByte*
IPixel::Ptr() const
{
    return  mData;
}


uint8
IPixel::BytesPerSample() const
{
    return  mInfo.BPC;
}


uint8
IPixel::Depth() const
{
    return  mInfo.BPP;
}


tFormat
IPixel::Format() const
{
    return  mInfo.FMT;
}


eColorModel
IPixel::Model() const
{
    return  static_cast< eColorModel >( ULIS3_R_MODEL( mInfo.FMT ) );
}


eType
IPixel::Type() const
{
    return  static_cast< eType >( ULIS3_R_TYPE( mInfo.FMT ) );
}


bool
IPixel::HasAlpha() const
{
    return  mInfo.HEA;
}


bool
IPixel::Swapped() const
{
    return  static_cast< bool >( ULIS3_R_SWAP( mInfo.FMT ) );
}


bool
IPixel::Reversed() const
{
    return  static_cast< bool >( ULIS3_R_REVERSE( mInfo.FMT ) );
}


uint8
IPixel::NumSamples() const
{
    return  mInfo.SPP;
}


uint8
IPixel::NumColorChannels() const
{
    return  mInfo.NCC;
}

uint8*
IPixel::IndexTable() const
{
    return  mInfo.IDT;
}

const FFormatInfo&
IPixel::FormatInfo() const
{
    return  mInfo;
}

FColorProfile*
IPixel::Profile() const
{
    return  mProfile;
}


uint8
IPixel::RedirectedIndex( uint8 iIndex ) const
{
    ULIS3_ASSERT( iIndex >= 0 && iIndex < mInfo.SPP, "Bad Index" );
    return  mInfo.IDT[ iIndex ];
}


uint8
IPixel::AlphaIndex() const
{
    ULIS3_ASSERT( mInfo.HEA, "Bad Call" );
    return  mInfo.AID;
}


void
IPixel::TweakFormat( tFormat iFormat )
{
    FFormatInfo newInfo( iFormat );
    ULIS3_ASSERT( newInfo.BPP == mInfo.BPP, "Bad tweak operation" );
    mInfo = newInfo;
}


void
IPixel::AssignMemoryUnsafe( const IPixel& iOther )
{
    memcpy( mData, iOther.Ptr(), Depth() );
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------ Generic Accesss API
tByte*
IPixel::SamplePtr( uint8 iIndex )
{
    ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
    return  ( tByte* )( Ptr() + ( (uint64)iIndex * BytesPerSample() ) );
}


template< typename T >
T*
IPixel::SamplePtrT( uint8 iIndex )
{
    ULIS3_ASSERT( Type() == eTypeFromT< T >(), "Bad type" )
    return  ( T* )( SamplePtr( iIndex ) );
}


const tByte*
IPixel::SamplePtr( uint8 iIndex ) const
{
    ULIS3_ASSERT( iIndex < NumSamples(), "Index out of range" );
    return  ( tByte* )( Ptr() + ( (uint64)iIndex * BytesPerSample() ) );
}


template< typename T >
const T*
IPixel::SamplePtrT( uint8 iIndex ) const
{
    ULIS3_ASSERT( Type() == eTypeFromT< T >(), "Bad type" )
    return  ( T* )( SamplePtr( iIndex ) );
}


template< typename T >
T
IPixel::GetValueRaw( uint8 iIndex ) const
{
    switch( Type() ) {
        case TYPE_UINT8     :   return  ConvType< uint8,   T >( *( (uint8*)   ( SamplePtrT< uint8 >( iIndex ) ) ) );
        case TYPE_UINT16    :   return  ConvType< uint16,  T >( *( (uint16*)  ( SamplePtrT< uint16 >( iIndex ) ) ) );
        case TYPE_UINT32    :   return  ConvType< uint32,  T >( *( (uint32*)  ( SamplePtrT< uint32 >( iIndex ) ) ) );
        case TYPE_UFLOAT    :   return  ConvType< ufloat,  T >( *( (ufloat*)  ( SamplePtrT< ufloat >( iIndex ) ) ) );
        case TYPE_UDOUBLE   :   return  ConvType< udouble, T >( *( (udouble*) ( SamplePtrT< udouble >( iIndex ) ) ) );
        default:                ULIS3_ASSERT( false, "Bad Type" ); return  T(0);
    }
}


template< typename T >
void
IPixel::SetValueRaw( uint8 iIndex, T iValue )
{
    switch( Type() ) {
        case TYPE_UINT8     :   *( (uint8*)   ( SamplePtrT< uint8 >( iIndex ) ) ) = ConvType< T, uint8   >( iValue ); break;
        case TYPE_UINT16    :   *( (uint16*)  ( SamplePtrT< uint16 >( iIndex ) ) ) = ConvType< T, uint16  >( iValue ); break;
        case TYPE_UINT32    :   *( (uint32*)  ( SamplePtrT< uint32 >( iIndex ) ) ) = ConvType< T, uint32  >( iValue ); break;
        case TYPE_UFLOAT    :   *( (ufloat*)  ( SamplePtrT< ufloat >( iIndex ) ) ) = ConvType< T, ufloat  >( iValue ); break;
        case TYPE_UDOUBLE   :   *( (udouble*) ( SamplePtrT< udouble >( iIndex ) ) ) = ConvType< T, udouble >( iValue ); break;
        default:                ULIS3_ASSERT( false, "Bad Type" );
    }
}


template< typename T > T&
IPixel::GetRefRaw( uint8 iIndex )
{
    return  (*SamplePtrT< T >( iIndex ));
}


template< typename T >
const T&
IPixel::GetConstRefRaw( uint8 iIndex ) const
{
    return  (*SamplePtrT< T >( iIndex ));
}


template< typename T >
T
IPixel::GetValue( uint8 iIndex ) const
{
    return  GetValueRaw< T >( RedirectedIndex( iIndex ) );
}


template< typename T >
T&
IPixel::GetRef( uint8 iIndex )
{
    return  GetRefRaw< T >( RedirectedIndex( iIndex ) );
}


template< typename T >
const T&
IPixel::GetConstRef( uint8 iIndex ) const
{
    return  GetConstRefRaw< T >( RedirectedIndex( iIndex ) );
}


template< typename T >
void
IPixel::SetValue( uint8 iIndex, T iValue )
{
    SetValueRaw< T >( RedirectedIndex( iIndex ), iValue );
}


/////////////////////////////////////////////////////
// FPixelValue
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FPixelValue::~FPixelValue()
{
    if( mData )
        delete [] mData;
}

FPixelValue::FPixelValue()
    : tParent( ULIS3_FORMAT_RGBA8, nullptr )
{
    mData = new tByte[ Depth() ];
    memset( mData, 0, Depth() );
}

FPixelValue::FPixelValue( uint32 iFormat, FColorProfile* iProfile )
    : tParent( iFormat, iProfile )
{
    mData = new tByte[ Depth() ];
    memset( mData, 0, Depth() );
}


FPixelValue::FPixelValue( const tByte* iData, tFormat iFormat, FColorProfile* iProfile )
    : tParent( iFormat, iProfile )
{
    mData = new tByte[ Depth() ];
    memcpy( mData, iData, Depth() );
}


FPixelValue::FPixelValue( const FPixelProxy& iProxy )
    : tParent( iProxy.Format(), iProxy.Profile() )
{
    mData = new tByte[ Depth() ];
    memcpy( mData, iProxy.Ptr(), Depth() );
}

FPixelValue::FPixelValue( const FPixelValue& iValue )
    : tParent( iValue.Format(), iValue.Profile() )
{
    mData = new tByte[ Depth() ];
    memcpy( mData, iValue.Ptr(), Depth() );
}


FPixelValue::FPixelValue( FPixelValue&& iValue )
    : tParent( iValue.Format(), iValue.Profile() )
{
    mData = iValue.mData;
    iValue.mData = nullptr;
}

FPixelValue&
FPixelValue::operator=( const FPixelValue& iOther ) {
    if( mData )
        delete [] mData;

    mInfo = iOther.mInfo;
    mProfile = iOther.mProfile;
    mData = new tByte[ Depth() ];
    memcpy( mData, iOther.Ptr(), Depth() );

    return  *this;
}


template< typename T >
FPixelValue::FPixelValue( uint32 iFormat, std::initializer_list< T > iValues, FColorProfile* iProfile )
    : tParent( iFormat, iProfile )
{
    mData = new tByte[ Depth() ];
    switch( Type() )
    {
        case TYPE_UINT8:        Set_imp< T, uint8   >( iValues ); return;
        case TYPE_UINT16:       Set_imp< T, uint16  >( iValues ); return;
        case TYPE_UINT32:       Set_imp< T, uint32  >( iValues ); return;
        case TYPE_UFLOAT:       Set_imp< T, ufloat  >( iValues ); return;
        case TYPE_UDOUBLE:      Set_imp< T, udouble >( iValues ); return;
    }
}

//--------------------------------------------------------------------------------------
//------------------------------------------------------------ Named static constructors
//static
FPixelValue
FPixelValue::FromGreyA8( uint8 iGrey, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_GA8, { iGrey, iA } );
}


//static
FPixelValue
FPixelValue::FromGreyA16( uint16 iGrey, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_GA16, { iGrey, iA } );
}


//static
FPixelValue
FPixelValue::FromGreyA32( uint32 iGrey, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_GA32, { iGrey, iA } );
}


//static
FPixelValue
FPixelValue::FromGreyAF( float iGrey, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_GAF, { iGrey, iA } );
}


//static
FPixelValue
FPixelValue::FromGreyAD( double iGrey, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_GAD, { iGrey, iA } );
}


//static
FPixelValue
FPixelValue::FromRGBA8( uint8 iR, uint8 iG, uint8 iB, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_RGBA8, { iR, iG, iB, iA } );
}


//static
FPixelValue
FPixelValue::FromRGBA16( uint16 iR, uint16 iG, uint16 iB, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_RGBA16, { iR, iG, iB, iA } );
}


//static
FPixelValue
FPixelValue::FromRGBA32( uint32 iR, uint32 iG, uint32 iB, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_RGBA32, { iR, iG, iB, iA } );
}


//static
FPixelValue
FPixelValue::FromRGBAF( float iR, float iG, float iB, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_RGBAF, { iR, iG, iB, iA } );
}


//static
FPixelValue
FPixelValue::FromRGBAD( double iR, double iG, double iB, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_RGBAD, { iR, iG, iB, iA } );
}


//static
FPixelValue
FPixelValue::FromHSVA8( uint8 iH, uint8 iS, uint8 iV, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSVA8, { iH, iS, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromHSVA16( uint16 iH, uint16 iS, uint16 iV, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSVA16, { iH, iS, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromHSVA32( uint32 iH, uint32 iS, uint32 iV, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSVA32, { iH, iS, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromHSVAF( float iH, float iS, float iV, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSVAF, { iH, iS, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromHSVAD( double iH, double iS, double iV, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSVAD, { iH, iS, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromHSLA8( uint8 iH, uint8 iS, uint8 iL, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSLA8, { iH, iS, iL, iA } );
}


//static
FPixelValue
FPixelValue::FromHSLA16( uint16 iH, uint16 iS, uint16 iL, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSLA16, { iH, iS, iL, iA } );
}


//static
FPixelValue
FPixelValue::FromHSLA32( uint32 iH, uint32 iS, uint32 iL, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSLA32, { iH, iS, iL, iA } );
}


//static
FPixelValue
FPixelValue::FromHSLAF( float iH, float iS, float iL, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSLAF, { iH, iS, iL, iA } );
}


//static
FPixelValue
FPixelValue::FromHSLAD( double iH, double iS, double iL, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_HSLAD, { iH, iS, iL, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYA8( uint8 iC, uint8 iM, uint8 iY, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYA8, { iC, iM, iY, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYA16( uint16 iC, uint16 iM, uint16 iY, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYA16, { iC, iM, iY, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYA32( uint32 iC, uint32 iM, uint32 iY, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYA32, { iC, iM, iY, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYAF( float iC, float iM, float iY, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYAF, { iC, iM, iY, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYAD( double iC, double iM, double iY, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYAD, { iC, iM, iY, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYKA8( uint8 iC, uint8 iM, uint8 iY, uint8 iK, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYKA8, { iC, iM, iY, iK, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYKA16( uint16 iC, uint16 iM, uint16 iY, uint16 iK, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYKA16, { iC, iM, iY, iK, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYKA32( uint32 iC, uint32 iM, uint32 iY, uint32 iK, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYKA32, { iC, iM, iY, iK, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYKAF( float iC, float iM, float iY, float iK, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYKAF, { iC, iM, iY, iK, iA } );
}


//static
FPixelValue
FPixelValue::FromCMYKAD( double iC, double iM, double iY, double iK, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_CMYKAD, { iC, iM, iY, iK, iA } );
}


//static
FPixelValue
FPixelValue::FromYUVA8( uint8 iY, uint8 iU, uint8 iV, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_YUVA8, { iY, iU, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromYUVA16( uint16 iY, uint16 iU, uint16 iV, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_YUVA16, { iY, iU, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromYUVA32( uint32 iY, uint32 iU, uint32 iV, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_YUVA32, { iY, iU, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromYUVAF( float iY, float iU, float iV, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_YUVAF, { iY, iU, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromYUVAD( double iY, double iU, double iV, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_YUVAD, { iY, iU, iV, iA } );
}


//static
FPixelValue
FPixelValue::FromLabA8( uint8 iL, uint8 ia, uint8 ib, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_LabA8, { iL, ia, ib, iA } );
}


//static
FPixelValue
FPixelValue::FromLabA16( uint16 iL, uint16 ia, uint16 ib, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_LabA16, { iL, ia, ib, iA } );
}


//static
FPixelValue
FPixelValue::FromLabA32( uint32 iL, uint32 ia, uint32 ib, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_LabA32, { iL, ia, ib, iA } );
}


//static
FPixelValue
FPixelValue::FromLabAF( float iL, float ia, float ib, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_LabAF, { iL, ia, ib, iA } );
}


//static
FPixelValue
FPixelValue::FromLabAD( double iL, double ia, double ib, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_LabAD, { iL, ia, ib, iA } );
}


//static
FPixelValue
FPixelValue::FromXYZA8( uint8 iX, uint8 iY, uint8 iZ, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_XYZA8, { iX, iY, iZ, iA } );
}


//static
FPixelValue
FPixelValue::FromXYZA16( uint16 iX, uint16 iY, uint16 iZ, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_XYZA16, { iX, iY, iZ, iA } );
}


//static
FPixelValue
FPixelValue::FromXYZA32( uint32 iX, uint32 iY, uint32 iZ, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_XYZA32, { iX, iY, iZ, iA } );
}


//static
FPixelValue
FPixelValue::FromXYZAF( float iX, float iY, float iZ, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_XYZAF, { iX, iY, iZ, iA } );
}


//static
FPixelValue
FPixelValue::FromXYZAD( double iX, double iY, double iZ, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_XYZAD, { iX, iY, iZ, iA } );
}


//static
FPixelValue
FPixelValue::FromYxyA8( uint8 iY, uint8 ix, uint8 iy, uint8 iA )
{
    return  FPixelValue( ULIS3_FORMAT_YxyA8, { iY, ix, iy, iA } );
}


//static
FPixelValue
FPixelValue::FromYxyA16( uint16 iY, uint16 ix, uint16 iy, uint16 iA )
{
    return  FPixelValue( ULIS3_FORMAT_YxyA16, { iY, ix, iy, iA } );
}


//static
FPixelValue
FPixelValue::FromYxyA32( uint32 iY, uint32 ix, uint32 iy, uint32 iA )
{
    return  FPixelValue( ULIS3_FORMAT_YxyA32, { iY, ix, iy, iA } );
}


//static
FPixelValue
FPixelValue::FromYxyAF( float iY, float ix, float iy, float iA )
{
    return  FPixelValue( ULIS3_FORMAT_YxyAF, { iY, ix, iy, iA } );
}


//static
FPixelValue
FPixelValue::FromYxyAD( double iY, double ix, double iy, double iA )
{
    return  FPixelValue( ULIS3_FORMAT_YxyAD, { iY, ix, iy, iA } );
}


/////////////////////////////////////////////////////
// FPixelProxy
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FPixelProxy::~FPixelProxy()
{
}

FPixelProxy::FPixelProxy( tFormat iFormat, FColorProfile* iProfile )
    : tParent( iFormat, iProfile )
{
    mData = nullptr;
}

FPixelProxy::FPixelProxy( tByte* iData, tFormat iFormat, FColorProfile* iProfile )
    : tParent( iFormat, iProfile )
{
    ULIS3_ASSERT( iData, "Bad data provided." );
    mData = iData;
}


FPixelProxy::FPixelProxy( const tByte* iData, tFormat iFormat, FColorProfile* iProfile )
    : tParent( iFormat, iProfile )
{
    ULIS3_ASSERT( iData, "Bad data provided." );
    mData = const_cast< tByte* >( iData );
}

FPixelProxy::FPixelProxy( const FPixelProxy& iValue )
    : tParent( iValue.Format(), iValue.mProfile )
{
    mData = iValue.mData;
}

FPixelProxy::FPixelProxy( FPixelProxy&& iValue )
    : tParent( iValue.Format(), iValue.mProfile )
{
    mData = iValue.mData;
}

FPixelProxy&
FPixelProxy::operator=( const FPixelProxy& iValue ) {
    mInfo = iValue.mInfo;
    mProfile = iValue.mProfile;
    mData = iValue.mData;
    return  *this;
}

void
FPixelProxy::SetPtr( tByte* iPtr )
{
    mData = iPtr;
}

void
FPixelProxy::SetPtr( const tByte* iPtr )
{
    mData = const_cast< tByte* >( iPtr );
}


/////////////////////////////////////////////////////
// Template Instanciations
#define X_DO_A( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE i* IPixel::SamplePtrT< i >( uint8 );
#define X_DO_B( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE const i* IPixel::SamplePtrT< i >( uint8 ) const;
#define X_DO_C( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE i IPixel::GetValueRaw< i >( uint8 ) const;
#define X_DO_D( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE i& IPixel::GetRefRaw< i >( uint8 );
#define X_DO_E( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE const i& IPixel::GetConstRefRaw< i >( uint8 ) const;
#define X_DO_F( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE i IPixel::GetValue< i >( uint8 ) const;
#define X_DO_G( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE i& IPixel::GetRef< i >( uint8 );
#define X_DO_H( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE const i& IPixel::GetConstRef< i >( uint8 ) const;
#define X_DO_I( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE void IPixel::SetValueRaw< i >( uint8, i iValue );
#define X_DO_J( i, _E0, _E1, _E2, _E3 ) ULIS3_API_TEMPLATE void IPixel::SetValue< i >( uint8, i iValue );
//#ifdef ULIS3_MSVC
ULIS3_FOR_ALL_TYPES_DO( X_DO_A, 0, 0, 0, 0 )
ULIS3_FOR_ALL_TYPES_DO( X_DO_B, 0, 0, 0, 0 )
ULIS3_FOR_ALL_TYPES_DO( X_DO_C, 0, 0, 0, 0 )
ULIS3_FOR_ALL_TYPES_DO( X_DO_D, 0, 0, 0, 0 )
ULIS3_FOR_ALL_TYPES_DO( X_DO_E, 0, 0, 0, 0 )
ULIS3_FOR_ALL_TYPES_DO( X_DO_F, 0, 0, 0, 0 )
ULIS3_FOR_ALL_TYPES_DO( X_DO_G, 0, 0, 0, 0 )
ULIS3_FOR_ALL_TYPES_DO( X_DO_H, 0, 0, 0, 0 )
ULIS3_FOR_ALL_TYPES_DO( X_DO_I, 0, 0, 0, 0 )
ULIS3_FOR_ALL_TYPES_DO( X_DO_J, 0, 0, 0, 0 )
//#endif // ULIS3_MSVC
#undef X_DO_A
#undef X_DO_B
#undef X_DO_C
#undef X_DO_D
#undef X_DO_E
#undef X_DO_F
#undef X_DO_G
#undef X_DO_H
#undef X_DO_I
#undef X_DO_J

template ULIS3_API FPixelValue::FPixelValue( uint32, std::initializer_list< int >, FColorProfile* );
template ULIS3_API FPixelValue::FPixelValue( uint32, std::initializer_list< float >, FColorProfile* );
template ULIS3_API FPixelValue::FPixelValue( uint32, std::initializer_list< double >, FColorProfile* );
template ULIS3_API FPixelValue::FPixelValue( uint32, std::initializer_list< uint8 >, FColorProfile* );
template ULIS3_API FPixelValue::FPixelValue( uint32, std::initializer_list< uint16 >, FColorProfile* );
template ULIS3_API FPixelValue::FPixelValue( uint32, std::initializer_list< uint32 >, FColorProfile* );
template ULIS3_API FPixelValue::FPixelValue( uint32, std::initializer_list< uint64 >, FColorProfile* );

ULIS3_NAMESPACE_END

