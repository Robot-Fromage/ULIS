// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
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
#include "Image/Pixel.h"
#include "Image/Color.h"
#include "Image/Sample.h"
#include "Math/Math.h"
#include "Conv/srgb2linear.h"

ULIS_NAMESPACE_BEGIN
FColor
MixRGB( ufloat iT, const ISample& iA, const ISample& iB )
{
    ufloat t = FMath::Clamp( iT, 0.f, 1.f );
    FColor rgbA( eFormat::Format_RGBAF );
    FColor rgbB( eFormat::Format_RGBAF );
    FColor rgbResult( eFormat::Format_RGBAF );
    Conv( iA, rgbA );
    Conv( iB, rgbB );
    rgbResult.SetRF( ( 1.f - t ) * rgbA.RF() + t * rgbB.RF() );
    rgbResult.SetGF( ( 1.f - t ) * rgbA.GF() + t * rgbB.GF() );
    rgbResult.SetBF( ( 1.f - t ) * rgbA.BF() + t * rgbB.BF() );
    rgbResult.SetAF( ( 1.f - t ) * rgbA.AF() + t * rgbB.AF() );
    return  rgbResult;
}

FColor
MixLab( ufloat iT, const ISample& iA, const ISample& iB )
{
    ufloat t = FMath::Clamp( iT, 0.f, 1.f );
    FColor LabA( eFormat::Format_LabAF );
    FColor LabB( eFormat::Format_LabAF );
    FColor LabResult( eFormat::Format_LabAF );
    Conv( iA, LabA );
    Conv( iB, LabB );
    LabResult.SetLF( ( 1.f - t ) * LabA.LF() + t * LabB.LF() );
    LabResult.SetaF( ( 1.f - t ) * LabA.aF() + t * LabB.aF() );
    LabResult.SetbF( ( 1.f - t ) * LabA.bF() + t * LabB.bF() );
    LabResult.SetAF( ( 1.f - t ) * LabA.AF() + t * LabB.AF() );
    return  LabResult;
}

ULIS_NAMESPACE_END

