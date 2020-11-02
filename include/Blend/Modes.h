// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Modes.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the Blend and Alpha modes enums and related info.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// eBlendMode
enum eBlendMode : uint8
{
      Blend_Normal
    , Blend_Top
    , Blend_Back
    , Blend_Behind
    , Blend_Dissolve
    , Blend_BayerDither8x8
    , Blend_Darken
    , Blend_Multiply
    , Blend_ColorBurn
    , Blend_LinearBurn
    , Blend_DarkerColor
    , Blend_Lighten
    , Blend_Screen
    , Blend_ColorDodge
    , Blend_LinearDodge
    , Blend_LighterColor
    , Blend_Overlay
    , Blend_SoftLight
    , Blend_HardLight
    , Blend_VividLight
    , Blend_LinearLight
    , Blend_PinLight
    , Blend_HardMix
    , Blend_Phoenix
    , Blend_Reflect
    , Blend_Glow
    , Blend_Difference
    , Blend_Exclusion
    , Blend_Add
    , Blend_Substract
    , Blend_Divide
    , Blend_Average
    , Blend_Negation
    , Blend_Hue
    , Blend_Saturation
    , Blend_Color
    , Blend_Luminosity
    , Blend_PartialDerivative
    , Blend_Whiteout
    , Blend_AngleCorrected
    , NumBlendingModes
};


static const char* kwBlendMode[] =
{
      "Normal"
    , "Top"
    , "Back"
    , "Behind"
    , "Dissolve"
    , "Dither"
    , "Darken"
    , "Multiply"
    , "ColorBurn"
    , "LinearBurn"
    , "DarkerColor"
    , "Lighten"
    , "Screen"
    , "ColorDodge"
    , "LinearDodge"
    , "LighterColor"
    , "Overlay"
    , "SoftLight"
    , "HardLight"
    , "VividLight"
    , "LinearLight"
    , "PinLight"
    , "HardMix"
    , "Phoenix"
    , "Reflect"
    , "Glow"
    , "Difference"
    , "Exclusion"
    , "Add"
    , "Substract"
    , "Divide"
    , "Average"
    , "Negation"
    , "Hue"
    , "Saturation"
    , "Color"
    , "Luminosity"
    , "PartialDerivative"
    , "Whiteout"
    , "AngleCorrected"
    , "Invalid"
};


/////////////////////////////////////////////////////
// eAlphaMode
enum eAlphaMode : uint8
{
      Alpha_Normal
    , Alpha_Erase
    , Alpha_Top
    , Alpha_Back
    , Alpha_Sub
    , Alpha_Add
    , Alpha_Mul
    , Alpha_Min
    , Alpha_Max
    , NumAlphaModes
};


static const char* kwAlphaMode[] =
{
      "Normal"
    , "Erase"
    , "Top"
    , "Back"
    , "Sub"
    , "Add"
    , "Mul"
    , "Min"
    , "Max"
    , "Invalid"
};

/////////////////////////////////////////////////////
// eBlendQualifier
enum eBlendQualifier : uint8
{
      BlendQualifier_Separable
    , BlendQualifier_NonSeparable
    , BlendQualifier_Misc
};

static
inline
constexpr
eBlendQualifier
BlendingModeQualifier( eBlendMode iBlendingMode )
{
    switch( iBlendingMode ) {
        case Blend_Dissolve             : return  BlendQualifier_Misc;
        case Blend_BayerDither8x8       : return  BlendQualifier_Misc;
        case Blend_DarkerColor          : return  BlendQualifier_NonSeparable;
        case Blend_LighterColor         : return  BlendQualifier_NonSeparable;
        case Blend_Hue                  : return  BlendQualifier_NonSeparable;
        case Blend_Saturation           : return  BlendQualifier_NonSeparable;
        case Blend_Color                : return  BlendQualifier_NonSeparable;
        case Blend_Luminosity           : return  BlendQualifier_NonSeparable;
        case Blend_PartialDerivative    : return  BlendQualifier_NonSeparable;
        case Blend_Whiteout             : return  BlendQualifier_NonSeparable;
        case Blend_AngleCorrected       : return  BlendQualifier_NonSeparable;
        default                         : return  BlendQualifier_Separable;
    }
}

/////////////////////////////////////////////////////
// Macros
#define ULIS_FOR_ALL_SEPARABLE_BM_DO( X, _E0, _E1, _E2, _E3 )   \
    X( Blend_Normal            , _E0, _E1, _E2, _E3 )           \
    X( Blend_Top               , _E0, _E1, _E2, _E3 )           \
    X( Blend_Back              , _E0, _E1, _E2, _E3 )           \
    X( Blend_Behind            , _E0, _E1, _E2, _E3 )           \
    X( Blend_Darken            , _E0, _E1, _E2, _E3 )           \
    X( Blend_Multiply          , _E0, _E1, _E2, _E3 )           \
    X( Blend_ColorBurn         , _E0, _E1, _E2, _E3 )           \
    X( Blend_LinearBurn        , _E0, _E1, _E2, _E3 )           \
    X( Blend_Lighten           , _E0, _E1, _E2, _E3 )           \
    X( Blend_Screen            , _E0, _E1, _E2, _E3 )           \
    X( Blend_ColorDodge        , _E0, _E1, _E2, _E3 )           \
    X( Blend_LinearDodge       , _E0, _E1, _E2, _E3 )           \
    X( Blend_Overlay           , _E0, _E1, _E2, _E3 )           \
    X( Blend_SoftLight         , _E0, _E1, _E2, _E3 )           \
    X( Blend_HardLight         , _E0, _E1, _E2, _E3 )           \
    X( Blend_VividLight        , _E0, _E1, _E2, _E3 )           \
    X( Blend_LinearLight       , _E0, _E1, _E2, _E3 )           \
    X( Blend_PinLight          , _E0, _E1, _E2, _E3 )           \
    X( Blend_HardMix           , _E0, _E1, _E2, _E3 )           \
    X( Blend_Phoenix           , _E0, _E1, _E2, _E3 )           \
    X( Blend_Reflect           , _E0, _E1, _E2, _E3 )           \
    X( Blend_Glow              , _E0, _E1, _E2, _E3 )           \
    X( Blend_Difference        , _E0, _E1, _E2, _E3 )           \
    X( Blend_Exclusion         , _E0, _E1, _E2, _E3 )           \
    X( Blend_Add               , _E0, _E1, _E2, _E3 )           \
    X( Blend_Substract         , _E0, _E1, _E2, _E3 )           \
    X( Blend_Divide            , _E0, _E1, _E2, _E3 )           \
    X( Blend_Average           , _E0, _E1, _E2, _E3 )           \
    X( Blend_Negation          , _E0, _E1, _E2, _E3 )

#define ULIS_FOR_ALL_NONSEPARABLE_BM_DO( X, _E0, _E1, _E2, _E3 )    \
    X( Blend_DarkerColor       , _E0, _E1, _E2, _E3 )               \
    X( Blend_LighterColor      , _E0, _E1, _E2, _E3 )               \
    X( Blend_Hue               , _E0, _E1, _E2, _E3 )               \
    X( Blend_Saturation        , _E0, _E1, _E2, _E3 )               \
    X( Blend_Color             , _E0, _E1, _E2, _E3 )               \
    X( Blend_Luminosity        , _E0, _E1, _E2, _E3 )               \
    X( Blend_PartialDerivative , _E0, _E1, _E2, _E3 )               \
    X( Blend_Whiteout          , _E0, _E1, _E2, _E3 )               \
    X( Blend_AngleCorrected    , _E0, _E1, _E2, _E3 )

#define ULIS_FOR_ALL_MISC_BM_DO( X, _E0, _E1, _E2, _E3 )    \
    X( Blend_Dissolve          , _E0, _E1, _E2, _E3 )       \
    X( Blend_BayerDither8x8    , _E0, _E1, _E2, _E3 )

#define ULIS_FOR_ALL_BM_DO( X, _E0, _E1, _E2, _E3 )             \
    ULIS_FOR_ALL_SEPARABLE_BM_DO(      X, _E0, _E1, _E2, _E3 )  \
    ULIS_FOR_ALL_NONSEPARABLE_BM_DO(   X, _E0, _E1, _E2, _E3 )  \
    ULIS_FOR_ALL_MISC_BM_DO(           X, _E0, _E1, _E2, _E3 )

#define ULIS_FOR_ALL_AM_DO( X, _E0, _E1, _E2, _E3 ) \
    X( Alpha_Normal        , _E0, _E1, _E2, _E3 )   \
    X( Alpha_Erase         , _E0, _E1, _E2, _E3 )   \
    X( Alpha_Top           , _E0, _E1, _E2, _E3 )   \
    X( Alpha_Back          , _E0, _E1, _E2, _E3 )   \
    X( Alpha_Sub           , _E0, _E1, _E2, _E3 )   \
    X( Alpha_Add           , _E0, _E1, _E2, _E3 )   \
    X( Alpha_Mul           , _E0, _E1, _E2, _E3 )   \
    X( Alpha_Min           , _E0, _E1, _E2, _E3 )   \
    X( Alpha_Max           , _E0, _E1, _E2, _E3 )

ULIS_NAMESPACE_END

