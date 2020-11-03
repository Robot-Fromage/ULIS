// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Color.CColor.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "ULIS/Color/ULIS.Color.CColor.h"
#include "ULIS/Maths/ULIS.Maths.Utility.h"
#include <cstdlib>
#include <cmath>

namespace ULIS {
/////////////////////////////////////////////////////
// CColor
//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------- Construction
CColor::CColor()
{
    SetRGB( 0, 0, 0 );
}


CColor::CColor( int r, int g, int b, int alpha )
{
    SetRGB( r, g, b, alpha );
}


CColor::CColor( eCColorModel iMode ) :
    mMode( iMode )
{
    mRepr.rgb = { 0, 0, 0, 0, 0 };
}


CColor::CColor( uint32 rgbHexValue )
{
    SetRGBHexValue( rgbHexValue );
}


//--------------------------------------------------------------------------------------
//----------------------------------------------------------------- Comparison Operators


bool
CColor::operator==( const  CColor& Other )  const
{
    return mMode == Other.mMode &&
        mRepr.array[0] == Other.mRepr.array[0] &&
        mRepr.array[1] == Other.mRepr.array[1] &&
        mRepr.array[2] == Other.mRepr.array[2] &&
        mRepr.array[3] == Other.mRepr.array[3] &&
        mRepr.array[4] == Other.mRepr.array[4];
}


bool
CColor::operator!=( const  CColor& Other )  const
{
    return !operator==( Other );
}


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------- Instance Conversion API



CColor
CColor::ToGrey()  const
{
    if( mMode == eCColorModel::kG )
        return  *this;

    if( mMode != eCColorModel::kRGB )
        return  ToRGB().ToGrey();

    CColor out;
    int r = Red();
    int g = Green();
    int b = Blue();
    int grey = ( r + g + b ) / 3;

    out.SetGrey( grey, Alpha() );
    return  out;
}


CColor
CColor::ToRGB()  const
{
    if( mMode == eCColorModel::kRGB )
        return  *this;

    CColor out;
    switch( mMode )
    {
        case eCColorModel::kG:
        {
            int grey = Grey();
            out.SetRGB( grey, grey, grey, Alpha() );
            break;
        }

        case eCColorModel::kHSL:
        {
            float h = HSLHueF() * 360;
            float s = HSLSaturationF();
            float l = LightnessF();
            float r, g, b;
            r = g = b = 0.f;
            float var_h = h / 60;
            if ( var_h == 6 ) var_h = 0;
            int var_i = var_h;
            float C = ( 1 - fabs( 2 * l - 1 ) ) * s;
            float X = C * ( 1 - fabs( fmod( var_h, 2 ) - 1 ) );
            float m = l - C / 2;
            if      ( var_i == 0 ) { r = C; g = X; b = 0; }
            else if ( var_i == 1 ) { r = X; g = C; b = 0; }
            else if ( var_i == 2 ) { r = 0; g = C; b = X; }
            else if ( var_i == 3 ) { r = 0; g = X; b = C; }
            else if ( var_i == 4 ) { r = X; g = 0; b = C; }
            else                   { r = C; g = 0; b = X; }
            out.SetRGBF( r + m, g + m, b + m, AlphaF() );
            break;
        }


        case eCColorModel::kHSV:
        {
            float h = HSVHueF();
            float s = HSVSaturationF();
            float v = ValueF();
            float r, g, b;
            r = g = b = 0.f;
            float var_h = 0.f;
            float var_i = 0.f;
            float var_1 = 0.f;
            float var_2 = 0.f;
            float var_3 = 0.f;
            if ( s == 0 )
            {
               r = v;
               g = v;
               b = v;
            }
            else
            {
               var_h = h * 6;
               if ( var_h == 6 ) var_h = 0 ;
               var_i = int( var_h );
               var_1 = v * ( 1 - s );
               var_2 = v * ( 1 - s * ( var_h - var_i ) );
               var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );

               if      ( var_i == 0 ) { r = v     ; g = var_3 ; b = var_1 ; }
               else if ( var_i == 1 ) { r = var_2 ; g = v     ; b = var_1 ; }
               else if ( var_i == 2 ) { r = var_1 ; g = v     ; b = var_3 ; }
               else if ( var_i == 3 ) { r = var_1 ; g = var_2 ; b = v     ; }
               else if ( var_i == 4 ) { r = var_3 ; g = var_1 ; b = v     ; }
               else                   { r = v     ; g = var_1 ; b = var_2 ; }
            }

            out.SetRGBF( r, g, b, AlphaF() );
            break;
        }


        case eCColorModel::kCMYK:
        {
            float c = CyanF();
            float m = MagentaF();
            float y = YellowF();
            float k = KeyF();
            float r = 1.f - ( c * ( 1.f - k ) + k );
            float g = 1.f - ( m * ( 1.f - k ) + k );
            float b = 1.f - ( y * ( 1.f - k ) + k );
            out.SetRGBF( r, g, b, AlphaF() );
            break;
        }


        default:
        {
            break;
        }
    }

    return  out;
}


CColor
CColor::ToHSL()  const
{
    if( mMode == eCColorModel::kHSL )
        return  *this;

    if( mMode != eCColorModel::kRGB )
        return  ToRGB().ToHSL();

    CColor out;
    float r = RedF();
    float g = GreenF();
    float b = BlueF();
    float cmin = FMath::Min3( r, g, b );
    float cmax = FMath::Max3( r, g, b );
    float delta = cmax - cmin;
    float deltaAdd = cmax + cmin;
    float h = 0.0;
    float s = 0.0;
    float l = 0.0;

    l = ( deltaAdd ) / 2.0;

    if ( delta < FMath::kEpsilonf ){
        h = 0.0;
        s = 0.0;
    }
    else
    {
        s = ( l < 0.5 ) ? delta / deltaAdd : delta / ( 2.0 - deltaAdd );

        float deltaR = ( ( ( cmax - r ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
        float deltaG = ( ( ( cmax - g ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
        float deltaB = ( ( ( cmax - b ) / 6.0 ) + ( delta / 2.0 ) ) / delta;

             if( fabs( r - cmax ) < FMath::kEpsilonf )   h = deltaB - deltaG;
        else if( fabs( g - cmax ) < FMath::kEpsilonf )   h = (1.0 / 3.0) + deltaR - deltaB;
        else if( fabs( b - cmax ) < FMath::kEpsilonf )   h = (2.0 / 3.0) + deltaG - deltaR;

        if( h < 0.0 ) h += 1.0;
        if( h > 1.0 ) h -= 1.0;
    }

    out.SetHSLF( h, s, l, AlphaF() );
    return  out;
}


CColor
CColor::ToHSV()  const
{
    if( mMode == eCColorModel::kHSV )
        return  *this;

    if( mMode != eCColorModel::kRGB )
        return  ToRGB().ToHSV();

    CColor out;
    float r = RedF();
    float g = GreenF();
    float b = BlueF();
    float cmin = FMath::Min3( r, g, b );
    float cmax = FMath::Max3( r, g, b );
    float delta = cmax - cmin;
    float h = 0.0;
    float s = 0.0;
    float v = 0.0;

    v = cmax;

    if ( delta < FMath::kEpsilonf ){
        h = 0.0;
        s = 0.0;
    }
    else
    {
        s = (delta / cmax );

        float deltaR = ( ( ( cmax - r ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
        float deltaG = ( ( ( cmax - g ) / 6.0 ) + ( delta / 2.0 ) ) / delta;
        float deltaB = ( ( ( cmax - b ) / 6.0 ) + ( delta / 2.0 ) ) / delta;

        if( fabs( r - cmax ) < FMath::kEpsilonf )        h = deltaB - deltaG;
        else if( fabs( g - cmax ) < FMath::kEpsilonf )   h = (1.0 / 3.0) + deltaR - deltaB;
        else if( fabs( b - cmax ) < FMath::kEpsilonf )   h = (2.0 / 3.0) + deltaG - deltaR;

        if( h < 0.0 ) h += 1.0;

        if( h > 1.0 ) h -= 1.0;
    }

    out.SetHSVF( h, s, v, AlphaF() );
    return  out;
}


CColor
CColor::ToCMYK()  const
{
    if( mMode == eCColorModel::kCMYK )
        return  *this;

    if( mMode != eCColorModel::kRGB )
        return  ToRGB().ToCMYK();

    CColor out;
    float r = RedF();
    float g = GreenF();
    float b = BlueF();
    float ik = FMath::Max3( r, g, b );
    float k = 1.f - ik;

    if( ik == 0 )
        ik = 1; //Division by 0 countermeasure, doesn't affect the result

    float c = ( ( 1.f - r ) - k ) / ( ik );
    float m = ( ( 1.f - g ) - k ) / ( ik );
    float y = ( ( 1.f - b ) - k ) / ( ik );
    out.SetCMYKF( c, m, y, k, AlphaF() );
    return  out;
}


CColor
CColor::ToModel( eCColorModel iModel )  const
{
    if( mMode == iModel )
        return  *this;

    switch( iModel )
    {
        case eCColorModel::kG:      return  ToGrey();
        case eCColorModel::kRGB:    return  ToRGB();
        case eCColorModel::kHSL:    return  ToHSL();
        case eCColorModel::kHSV:    return  ToHSV();
        case eCColorModel::kCMYK:   return  ToCMYK();
        default: return  CColor( eCColorModel::kInvalid );
    }
}


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Static Makers API


//static
CColor
CColor::FromRGBHexValue( uint32 rgbHexValue, int alpha )
{
    CColor out;
    out.SetRGBHexValue( rgbHexValue, alpha );
    return out;
}


//static
CColor
CColor::FromGrey( int g, int alpha )
{
    CColor out;
    out.SetGrey( g, alpha );
    return out;
}


//static
CColor
CColor::FromRGB( int r, int g, int b, int alpha )
{
    CColor out;
    out.SetRGB( r, g, b, alpha );
    return out;
}


//static
CColor
CColor::FromHSL( int h, int s, int l, int alpha )
{
    CColor out;
    out.SetHSL( h, s, l, alpha );
    return out;
}


//static
CColor
CColor::FromHSV( int h, int s, int v, int alpha )
{
    CColor out;
    out.SetHSV( h, s, v, alpha );
    return out;
}


//static
CColor
CColor::FromCMYK( int c, int m, int y, int k, int alpha )
{
    CColor out;
    out.SetCMYK( c, m, y, k );
    return out;
}


//static
CColor
CColor::FromGreyF( float g, float alpha )
{
    CColor out;
    out.SetGreyF( g, alpha );
    return  out;
}


//static
CColor
CColor::FromRGBF( float r, float g, float b, float alpha )
{
    CColor out;
    out.SetRGBF( r, g, b, alpha );
    return  out;
}


//static
CColor
CColor::FromHSLF( float h, float s, float l, float alpha )
{
    CColor out;
    out.SetHSLF( h, s, l, alpha );
    return  out;
}


//static
CColor
CColor::FromHSVF( float h, float s, float v, float alpha )
{
    CColor out;
    out.SetHSVF( h, s, v, alpha );
    return  out;
}


//static
CColor
CColor::FromCMYKF( float c, float m, float y, float k, float alpha )
{
    CColor out;
    out.SetCMYKF( c, m, y, k, alpha );
    return  out;
}


//static
bool
CColor::IsSimilar( const CColor& A, const CColor& B, int threshold )
{
    CColor Argb = A.ToRGB();
    CColor Brgb = B.ToRGB();
    int AR = Argb.Red();
    int AG = Argb.Green();
    int AB = Argb.Blue();
    int Aalpha = Argb.Alpha();
    int BR = Brgb.Red();
    int BG = Brgb.Green();
    int BB = Brgb.Blue();
    int Balpha = Brgb.Alpha();
    return  ( fabs( AR - BR ) <= threshold ) &&
            ( fabs( AG - BG ) <= threshold ) &&
            ( fabs( AB - BB ) <= threshold ) &&
            ( fabs( Aalpha - Balpha ) <= threshold );
}

//--------------------------------------------------------------------------------------
//------------------------------------------ Instance Representation Access API, Getters


int
CColor::Alpha()  const
{
    return  mRepr.rgb.alpha >> 8;
}


float
CColor::AlphaF()  const
{
    return  mRepr.rgb.alpha / float( UINT16_MAX );
}


// Hex
uint32
CColor::RGBHexValue()  const
{
    CColor rgb = this->ToRGB();
    int r = rgb.Red();
    int g = rgb.Green();
    int b = rgb.Blue();
    uint32 out = ( uint32( r ) << 16 ) + ( uint32( g ) << 8 ) + ( uint32( b ) << 0 );
    return  out;
}

 // Grey
int
CColor::Grey()  const
{
    if( mMode != eCColorModel::kG )
        return  ToGrey().Grey();
    return  mRepr.grey.g >> 8;
}


float
CColor::GreyF()  const
{
    if( mMode != eCColorModel::kG )
        return  ToGrey().GreyF();
    return  mRepr.grey.g / float( UINT16_MAX );
}


// RGB
int
CColor::Red()  const
{
    if( mMode != eCColorModel::kRGB )
        return  ToRGB().Red();
    return  mRepr.rgb.r >> 8;
}


int
CColor::Green()  const
{
    if( mMode != eCColorModel::kRGB )
        return  ToRGB().Green();
    return  mRepr.rgb.g >> 8;
}


int
CColor::Blue()  const
{
    if( mMode != eCColorModel::kRGB )
        return  ToRGB().Blue();
    return  mRepr.rgb.b >> 8;
}


float
CColor::RedF()  const
{
    if( mMode != eCColorModel::kRGB )
        return  ToRGB().RedF();
    return  mRepr.rgb.r / float( UINT16_MAX );
}


float
CColor::GreenF()  const
{
    if( mMode != eCColorModel::kRGB )
        return  ToRGB().GreenF();
    return  mRepr.rgb.g / float( UINT16_MAX );
}


float
CColor::BlueF()  const
{
    if( mMode != eCColorModel::kRGB )
        return  ToRGB().BlueF();
    return  mRepr.rgb.b / float( UINT16_MAX );
}


// HSV Alias
int
CColor::Hue()  const
{
    return  HSVHue();
}


int
CColor::Saturation()  const
{
    return  HSVSaturation();
}


float
CColor::HueF()  const
{
    return  HSVHueF();
}


float
CColor::SaturationF()  const
{
    return  HSVSaturationF();
}


// HSL
int
CColor::HSLHue()  const
{
    if( mMode != eCColorModel::kHSL )
        return  ToHSL().HSLHue();
    return  mRepr.hsl.h / 100;
}


int
CColor::HSLSaturation()  const
{
    if( mMode != eCColorModel::kHSL )
        return  ToHSL().HSLSaturation();
    return  mRepr.hsl.s >> 8;
}


int
CColor::Lightness()  const
{
    if( mMode != eCColorModel::kHSL )
        return  ToHSL().Lightness();
    return  mRepr.hsl.l >> 8;
}


float
CColor::HSLHueF()  const
{
    if( mMode != eCColorModel::kHSL )
        return  ToHSL().HSLHueF();
    return  mRepr.hsl.h / 36000.f ;
}


float
CColor::HSLSaturationF()  const
{
    if( mMode != eCColorModel::kHSL )
        return  ToHSL().HSLSaturationF();
    return  mRepr.hsl.s / float( UINT16_MAX );
}


float
CColor::LightnessF()  const
{
    if( mMode != eCColorModel::kHSL )
        return  ToHSL().LightnessF();
    return  mRepr.hsl.l / float( UINT16_MAX );
}


// HSV
int
CColor::HSVHue()  const
{
    if( mMode != eCColorModel::kHSV )
        return  ToHSV().HSVHue();
    return  mRepr.hsv.h / 100;
}


int
CColor::HSVSaturation()  const
{
    if( mMode != eCColorModel::kHSV )
        return  ToHSV().HSVSaturation();
    return  mRepr.hsv.s >> 8;
}


int
CColor::Value()  const
{
    if( mMode != eCColorModel::kHSV )
        return  ToHSV().Value();
    return  mRepr.hsv.v >> 8;
}


float
CColor::HSVHueF()  const
{
    if( mMode != eCColorModel::kHSV )
        return  ToHSV().HSVHueF();
    return  mRepr.hsv.h / 36000.f ;
}


float
CColor::HSVSaturationF()  const
{
    if( mMode != eCColorModel::kHSV )
        return  ToHSV().HSVSaturationF();
    return  mRepr.hsv.s / float( UINT16_MAX );
}


float
CColor::ValueF()  const
{
    if( mMode != eCColorModel::kHSV )
        return  ToHSV().ValueF();
    return  mRepr.hsv.v / float( UINT16_MAX );
}


// CMYK
int
CColor::Cyan()  const
{
    if( mMode != eCColorModel::kCMYK )
        return  ToCMYK().Cyan();
    return  mRepr.cmyk.c >> 8;
}


int
CColor::Magenta()  const
{
    if( mMode != eCColorModel::kCMYK )
        return  ToCMYK().Magenta();
    return  mRepr.cmyk.m >> 8;
}


int
CColor::Yellow()  const
{
    if( mMode != eCColorModel::kCMYK )
        return  ToCMYK().Yellow();
    return  mRepr.cmyk.y >> 8;
}


int
CColor::Key()  const
{
    if( mMode != eCColorModel::kCMYK )
        return  ToCMYK().Black();
    return  mRepr.cmyk.k >> 8;
}


int
CColor::Black()  const
{
    return  Key();
}


float
CColor::CyanF()  const
{
    if( mMode != eCColorModel::kCMYK )
        return  ToCMYK().CyanF();
    return  mRepr.cmyk.c / float( UINT16_MAX );
}


float
CColor::MagentaF()  const
{
    if( mMode != eCColorModel::kCMYK )
        return  ToCMYK().MagentaF();
    return  mRepr.cmyk.m / float( UINT16_MAX );
}


float
CColor::YellowF()  const
{
    if( mMode != eCColorModel::kCMYK )
        return  ToCMYK().YellowF();
    return  mRepr.cmyk.y / float( UINT16_MAX );
}


float
CColor::KeyF()  const
{
    if( mMode != eCColorModel::kCMYK )
        return  ToCMYK().KeyF();
    return  mRepr.cmyk.k / float( UINT16_MAX );
}


float
CColor::BlackF()  const
{
    return  KeyF();
}


//--------------------------------------------------------------------------------------
//------------------------------------------ Instance Representation Access API, Setters


void
CColor::SetAlpha( int value )
{
    mRepr.rgb.alpha = 0x101 * FMath::Clamp( value, 0, 0xff );
}

void
CColor::SetAlphaF( float value )
{
    mRepr.rgb.alpha = floor( FMath::Clamp( value, 0.f, 1.f ) * float( UINT16_MAX ) );
}


void
CColor::SetGrey( int g )
{
    if( mMode != eCColorModel::kG )
        SetGrey( g, Alpha() );
    else
        mRepr.grey.g = 0x101 * FMath::Clamp( g, 0, 0xff );
}


void
CColor::SetGreyF( float g )
{
    if( mMode != eCColorModel::kG )
        SetGreyF( g, AlphaF() );
    else
        mRepr.grey.g = floor( FMath::Clamp( g, 0.f, 1.f ) * float( UINT16_MAX ) );
}


void
CColor::SetRed( int value )
{
    if( mMode != eCColorModel::kRGB )
        SetRGB( value, Green(), Blue(), Alpha() );
    else
        mRepr.rgb.r = 0x101 * FMath::Clamp( value, 0, 0xff );
}


void
CColor::SetGreen( int value )
{
    if( mMode != eCColorModel::kRGB )
        SetRGB( Red(), value, Blue(), Alpha() );
    else
        mRepr.rgb.g = 0x101 * FMath::Clamp( value, 0, 0xff );
}


void
CColor::SetBlue( int value )
{
    if( mMode != eCColorModel::kRGB )
        SetRGB( Red(), Green(), value, Alpha() );
    else
        mRepr.rgb.b = 0x101 * FMath::Clamp( value, 0, 0xff );
}


void
CColor::SetRedF( float value )
{
    if( mMode != eCColorModel::kRGB )
        SetRGB( value, GreenF(), BlueF(), AlphaF() );
    else
        mRepr.rgb.r = floor( FMath::Clamp( value, 0.f, 1.f ) * float( UINT16_MAX ) );
}


void
CColor::SetGreenF( float value )
{
    if( mMode != eCColorModel::kRGB )
        SetRGB( RedF(), value, BlueF(), AlphaF() );
    else
        mRepr.rgb.g = floor( FMath::Clamp( value, 0.f, 1.f ) * float( UINT16_MAX ) );
}


void
CColor::SetBlueF( float value )
{
    if( mMode != eCColorModel::kRGB )
        SetRGB( RedF(), GreenF(), value, AlphaF() );
    else
        mRepr.rgb.b = floor( FMath::Clamp( value, 0.f, 1.f ) * float( UINT16_MAX ) );
}


void
CColor::SetRGBHexValue( uint32 rgbHexValue, int alpha )
{
    uint32 rmask = 0x00ff0000;
    uint32 gmask = 0x0000ff00;
    uint32 bmask = 0x000000ff;
    int r = static_cast< int >( uint32( rgbHexValue & rmask ) >> 16 );
    int g = static_cast< int >( uint32( rgbHexValue & gmask ) >> 8 );
    int b = static_cast< int >( uint32( rgbHexValue & bmask ) >> 0 );
    SetRGB( r, g, b, alpha );
}



void
CColor::SetGrey( int g, int alpha )
{
    mMode = eCColorModel::kG;
    mRepr.grey.alpha = 0x101 * FMath::Clamp( alpha, 0, 0xff );
    mRepr.grey.g     = 0x101 * FMath::Clamp( g, 0, 0xff );
    mRepr.grey._0    = 0;
    mRepr.grey._1    = 0;
    mRepr.grey._     = 0;
}


void
CColor::SetRGB( int r, int g, int b, int alpha )
{
    mMode = eCColorModel::kRGB;
    mRepr.rgb.alpha = 0x101 * FMath::Clamp( alpha, 0, 0xff );
    mRepr.rgb.r     = 0x101 * FMath::Clamp( r, 0, 0xff );
    mRepr.rgb.g     = 0x101 * FMath::Clamp( g, 0, 0xff );
    mRepr.rgb.b     = 0x101 * FMath::Clamp( b, 0, 0xff );
    mRepr.rgb._     = 0;
}


void
CColor::SetHSL( int h, int s, int l, int alpha )
{
    while( h < 0 ) { h += 360; }
    mMode = eCColorModel::kHSL;
    mRepr.hsl.alpha = 0x101 * FMath::Clamp( alpha, 0, 0xff );
    mRepr.hsl.h     = ( h % 360 ) * 100;
    mRepr.hsl.s     = 0x101 * FMath::Clamp( s, 0, 0xff );
    mRepr.hsl.l     = 0x101 * FMath::Clamp( l, 0, 0xff );
    mRepr.hsl._     = 0;
}


void
CColor::SetHSV( int h, int s, int v, int alpha )
{
    while( h < 0 ) { h += 360; }
    mMode = eCColorModel::kHSV;
    mRepr.hsv.alpha = 0x101 * FMath::Clamp( alpha, 0, 0xff );
    mRepr.hsv.h     = ( h % 360 ) * 100;
    mRepr.hsv.s     = 0x101 * FMath::Clamp( s, 0, 0xff );
    mRepr.hsv.v     = 0x101 * FMath::Clamp( v, 0, 0xff );
    mRepr.hsv._     = 0;
}


void
CColor::SetCMYK( int c, int m, int y, int k, int alpha )
{
    mMode = eCColorModel::kCMYK;
    mRepr.cmyk.alpha = 0x101 * FMath::Clamp( alpha, 0, 0xff );
    mRepr.cmyk.c     = 0x101 * FMath::Clamp( c, 0, 0xff );
    mRepr.cmyk.m     = 0x101 * FMath::Clamp( m, 0, 0xff );
    mRepr.cmyk.y     = 0x101 * FMath::Clamp( y, 0, 0xff );
    mRepr.cmyk.k     = 0x101 * FMath::Clamp( k, 0, 0xff );
}


void
CColor::SetGreyF( float g, float alpha )
{
    mMode = eCColorModel::kG;
    mRepr.grey.alpha = floor( FMath::Clamp( alpha, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.grey.g     = floor( FMath::Clamp( g, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.grey._0    = 0;
    mRepr.grey._1    = 0;
    mRepr.grey._     = 0;
}


void
CColor::SetRGBF( float r, float g, float b, float alpha )
{
    mMode = eCColorModel::kRGB;
    mRepr.rgb.alpha = floor( FMath::Clamp( alpha, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.rgb.r     = floor( FMath::Clamp( r, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.rgb.g     = floor( FMath::Clamp( g, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.rgb.b     = floor( FMath::Clamp( b, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.rgb._     = 0;
}


void
CColor::SetHSLF( float h, float s, float l, float alpha )
{
    mMode = eCColorModel::kHSL;
    mRepr.hsl.alpha = floor( FMath::Clamp( alpha, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.hsl.h     = floor( FMath::Clamp( h, 0.f, 1.f ) * 36000 );
    mRepr.hsl.s     = floor( FMath::Clamp( s, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.hsl.l     = floor( FMath::Clamp( l, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.hsl._     = 0;
}


void
CColor::SetHSVF( float h, float s, float v, float alpha )
{
    mMode = eCColorModel::kHSV;
    mRepr.hsv.alpha = floor( FMath::Clamp( alpha, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.hsv.h     = floor( FMath::Clamp( h, 0.f, 1.f ) * 36000 );
    mRepr.hsv.s     = floor( FMath::Clamp( s, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.hsv.v     = floor( FMath::Clamp( v, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.hsv._     = 0;
}


void
CColor::SetCMYKF( float c, float m, float y, float k, float alpha )
{
    mMode = eCColorModel::kCMYK;
    mRepr.cmyk.alpha = floor( FMath::Clamp( alpha, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.cmyk.c     = floor( FMath::Clamp( c, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.cmyk.m     = floor( FMath::Clamp( m, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.cmyk.y     = floor( FMath::Clamp( y, 0.f, 1.f ) * float( UINT16_MAX ) );
    mRepr.cmyk.k     = floor( FMath::Clamp( k, 0.f, 1.f ) * float( UINT16_MAX ) );
}

} // namespace ULIS
