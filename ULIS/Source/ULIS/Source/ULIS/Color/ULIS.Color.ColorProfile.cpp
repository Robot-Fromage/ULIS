// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Color.ColorProfile.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "ULIS/Color/ULIS.Color.ColorProfile.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FColorProfile
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FColorProfile::FColorProfile( const std::string& iName, cmsHPROFILE iProfile )
    : profile( iProfile )
    , model( ColorModelFromColorSpaceSignature( cmsGetColorSpace( iProfile ) ) )
    , name( iName )
{
}


FColorProfile::~FColorProfile()
{
    cmsCloseProfile( profile );
    profile = nullptr;
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
e_cm
FColorProfile::ModelSignature()  const
{
    return  model;
}


bool
FColorProfile::ModelSupported( e_cm iModel ) const
{
    return  ColorModelCompatFallback( iModel ) == model;
}


const std::string&
FColorProfile::Name() const
{
    return  name;
}


cmsHPROFILE
FColorProfile::ProfileHandle()
{
    return  profile;
}


} // namespace ULIS
