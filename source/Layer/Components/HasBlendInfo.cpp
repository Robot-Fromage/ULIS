// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         IHasBlendInfo.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasBlendInfo class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasBlendInfo.h"

ULIS_NAMESPACE_BEGIN

IHasBlendInfo::IHasBlendInfo(
      eBlendMode iBlendMode
    , eAlphaMode iAlphaMode
    , ufloat iOpacity
    , const FOnBlendInfoChanged& iDelegate
)
    : TCallbackCapable< FOnBlendInfoChanged >( iDelegate )
    , mInfo{
          iBlendMode
        , iAlphaMode
        , iOpacity
    }
{
    OnChanged( mInfo );
}

eBlendMode
IHasBlendInfo::BlendMode() const {
    return mInfo.blendMode;
}

eAlphaMode
IHasBlendInfo::AlphaMode() const {
    return mInfo.alphaMode;
}

ufloat
IHasBlendInfo::Opacity() const {
    return mInfo.opacity;
}

void
IHasBlendInfo::SetBlendMode( eBlendMode iValue ) {
    mInfo.blendMode = iValue;
    OnChanged( mInfo );
}

void
IHasBlendInfo::SetAlphaMode( eAlphaMode iValue ) {
     mInfo.alphaMode = iValue;
     OnChanged( mInfo );
}

void
IHasBlendInfo::SetOpacity( ufloat iValue ) {
     mInfo.opacity = iValue;
     OnChanged( mInfo );
}

ULIS_NAMESPACE_END

