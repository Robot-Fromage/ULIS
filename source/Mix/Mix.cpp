// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Mix.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Mix entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Mix/Mix.h"
#include "Conv/Conv.h"
#include "Data/Pixel.h"
#include "Maths/Maths.h"
#include "Conv/srgb2linear.h"

ULIS3_NAMESPACE_BEGIN
FPixelValue
MixNative( ufloat iT, IPixel* iA, IPixel* iB )
{
    ULIS3_ASSERT( iA, "Bad input A" );
    ULIS3_ASSERT( iB, "Bad input B" );
    ufloat t = FMaths::Clamp( iT, 0.f, 1.f );
    FPixelValue native_ret( iA->Format() );
    FPixelValue native_B( iA->Format() );
    Conv( *iB, native_B );
    const uint8 spp = iA->NumSamples();
    for( uint8 i = 0; i < spp; ++i )
        native_ret.SetValue< ufloat >( i, ( 1.f - t ) * iA->GetValue< ufloat >( i ) + t * iB->GetValue< ufloat >( i ) );
    return  native_ret;
}


FPixelValue
MixLinearRGB( ufloat iT, IPixel* iA, IPixel* iB )
{
    ULIS3_ASSERT( iA, "Bad input A" );
    ULIS3_ASSERT( iB, "Bad input B" );
    ufloat t = FMaths::Clamp( iT, 0.f, 1.f );
    FPixelValue rgbA( ULIS3_FORMAT_RGBAF );
    FPixelValue rgbB( ULIS3_FORMAT_RGBAF );
    FPixelValue rgbResult( ULIS3_FORMAT_RGBAF );
    Conv( *iA, rgbA );
    Conv( *iB, rgbB );
    rgbResult.SetRF( srgb2linear( ( 1.f - t ) * rgbA.RF() + t * rgbB.RF() ) );
    rgbResult.SetGF( srgb2linear( ( 1.f - t ) * rgbA.GF() + t * rgbB.GF() ) );
    rgbResult.SetBF( srgb2linear( ( 1.f - t ) * rgbA.BF() + t * rgbB.BF() ) );
    rgbResult.SetAF( srgb2linear( ( 1.f - t ) * rgbA.AF() + t * rgbB.AF() ) );
    return  rgbResult;
}


FPixelValue
MixRGB( ufloat iT, IPixel* iA, IPixel* iB )
{
    ULIS3_ASSERT( iA, "Bad input A" );
    ULIS3_ASSERT( iB, "Bad input B" );
    ufloat t = FMaths::Clamp( iT, 0.f, 1.f );
    FPixelValue rgbA( ULIS3_FORMAT_RGBAF );
    FPixelValue rgbB( ULIS3_FORMAT_RGBAF );
    FPixelValue rgbResult( ULIS3_FORMAT_RGBAF );
    Conv( *iA, rgbA );
    Conv( *iB, rgbB );
    rgbResult.SetRF( ( 1.f - t ) * rgbA.RF() + t * rgbB.RF() );
    rgbResult.SetGF( ( 1.f - t ) * rgbA.GF() + t * rgbB.GF() );
    rgbResult.SetBF( ( 1.f - t ) * rgbA.BF() + t * rgbB.BF() );
    rgbResult.SetAF( ( 1.f - t ) * rgbA.AF() + t * rgbB.AF() );
    return  rgbResult;
}


FPixelValue
MixLab( ufloat iT, IPixel* iA, IPixel* iB )
{
    ULIS3_ASSERT( iA, "Bad input A" );
    ULIS3_ASSERT( iB, "Bad input B" );
    ufloat t = FMaths::Clamp( iT, 0.f, 1.f );
    FPixelValue LabA( ULIS3_FORMAT_LabAF );
    FPixelValue LabB( ULIS3_FORMAT_LabAF );
    FPixelValue LabResult( ULIS3_FORMAT_LabAF );
    Conv( *iA, LabA );
    Conv( *iB, LabB );
    LabResult.SetLF( ( 1.f - t ) * LabA.LF() + t * LabB.LF() );
    LabResult.SetaF( ( 1.f - t ) * LabA.aF() + t * LabB.aF() );
    LabResult.SetbF( ( 1.f - t ) * LabA.bF() + t * LabB.bF() );
    LabResult.SetAF( ( 1.f - t ) * LabA.AF() + t * LabB.AF() );
    return  LabResult;
}

ULIS3_NAMESPACE_END

