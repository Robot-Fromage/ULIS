// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasBlendInfo.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the IHasBlendInfo class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Components/BlendInfo.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      IHasBlendInfo
/// @brief      Simple Blendable class.
class ULIS_API IHasBlendInfo
{
protected:
    IHasBlendInfo( eBlendMode iBlendMode, eAlphaMode iAlphaMode, ufloat iOpacity );

public:
    eBlendMode BlendMode() const;
    void SetBlendMode( eBlendMode iValue );
    eAlphaMode AlphaMode() const;
    void SetAlphaMode( eAlphaMode iValue );
    ufloat Opacity() const;
    void SetOpacity( ufloat iValue );

private:
    FBlendInfo mInfo;
};

ULIS_NAMESPACE_END

