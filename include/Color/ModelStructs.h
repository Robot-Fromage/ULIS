// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         ModelStructs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for models structs.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Maths/Maths.h"

ULIS2_NAMESPACE_BEGIN
struct FGreyF
{
    float Grey;
};

struct FRGBF
{
    float R;
    float G;
    float B;
};

struct FRGBF_arr
{
    float m[3];
};

struct FHSVF
{
    float H;
    float S;
    float V;
};

struct FHSLF
{
    float H;
    float S;
    float L;
};

struct FCMYF
{
    float C;
    float M;
    float Y;
};

struct FCMYKF
{
    float C;
    float M;
    float Y;
    float K;
};

struct FYUVF
{
    float Y;
    float U;
    float V;
};

struct FLabF
{
    float L;
    float a;
    float b;
};

struct FXYZF
{
    float X;
    float Y;
    float Z;
};

struct FYxyF
{
    float Y;
    float x;
    float y;
};

struct FLChF
{
    float L;
    float C;
    float h;
};

static ULIS2_FORCEINLINE FCMYKF RGBToCMYK( const FRGBF& iValue )
{
    float ik = FMaths::Max3( iValue.R, iValue.G, iValue.B );
    float k = 1.f - ik;
    if( ik == 0 ) ik = 0.f;
    return  { ( ( 1.f - iValue.R ) - k ) / ik
            , ( ( 1.f - iValue.G ) - k ) / ik
            , ( ( 1.f - iValue.B ) - k ) / ik
            , k };
}

static ULIS2_FORCEINLINE FRGBF CMYKToRGB( const FCMYKF& iValue )
{
    return  { 1.f - ( iValue.C * ( 1.f - iValue.K ) + iValue.K )
            , 1.f - ( iValue.M * ( 1.f - iValue.K ) + iValue.K )
            , 1.f - ( iValue.Y * ( 1.f - iValue.K ) + iValue.K ) };
}


static ULIS2_FORCEINLINE FLChF LabToLCh( const FLabF& iValue )
{
    return  { iValue.L, powf( sqrtf( iValue.a ) + sqrtf( iValue.b ), 0.5f ), atan2f( iValue.b, iValue.a ) };
}

static ULIS2_FORCEINLINE FLabF LChToLab( const FLChF& iValue )
{
    return  { iValue.L, iValue.C * cosf( iValue.h ), iValue.C * sinf( iValue.h ) };
}



ULIS2_NAMESPACE_END

