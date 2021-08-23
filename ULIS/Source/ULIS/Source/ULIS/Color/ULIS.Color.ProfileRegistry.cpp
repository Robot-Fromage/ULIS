// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Color.ProfileRegistry.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "ULIS/Color/ULIS.Color.ProfileRegistry.h"
#include "ULIS/Color/ULIS.Color.DefaultCMYK_SNAP.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FProfileRegistry
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
FProfileRegistry::FProfileRegistry()
{
    cmsToneCurve* gamma_2_2     = cmsBuildGamma( 0, 2.2 );
    cmsToneCurve* gamma3_2_2[3] = { gamma_2_2, gamma_2_2, gamma_2_2 };

    cmsCIExyYTRIPLE AdobeRGB_compat_chroma;
    AdobeRGB_compat_chroma.Red      = cmsCIExyY{ 0.6400, 0.3300, 0.0300 };
    AdobeRGB_compat_chroma.Green    = cmsCIExyY{ 0.2100, 0.7100, 0.0800 };
    AdobeRGB_compat_chroma.Blue     = cmsCIExyY{ 0.1500, 0.0600, 0.7900 };

    mLockedSoftwareProfiles[ DefaultProfiles::sRGB ]               = new FColorProfile( DefaultProfiles::sRGB,                cmsCreate_sRGBProfile() );
    mLockedSoftwareProfiles[ DefaultProfiles::AdobeRGB_compat ]    = new FColorProfile( DefaultProfiles::AdobeRGB_compat,     cmsCreateRGBProfile( &::ULIS::Chroma::whitepoint_D65, &AdobeRGB_compat_chroma, gamma3_2_2 ) );
    mLockedSoftwareProfiles[ DefaultProfiles::Lab_D65 ]            = new FColorProfile( DefaultProfiles::Lab_D65,             cmsCreateLab4Profile( &::ULIS::Chroma::whitepoint_D65 ) );
    mLockedSoftwareProfiles[ DefaultProfiles::Lab_D50 ]            = new FColorProfile( DefaultProfiles::Lab_D50,             cmsCreateLab4Profile( &::ULIS::Chroma::whitepoint_D50 ) );
    mLockedSoftwareProfiles[ DefaultProfiles::XYZ ]                = new FColorProfile( DefaultProfiles::XYZ,                 cmsCreateXYZProfile() );
    mLockedSoftwareProfiles[ DefaultProfiles::Grey_Gamma2_2_D65 ]  = new FColorProfile( DefaultProfiles::Grey_Gamma2_2_D65,   cmsCreateGrayProfile( &::ULIS::Chroma::whitepoint_D65, gamma_2_2 ) );
    mLockedSoftwareProfiles[ DefaultProfiles::Default_CMYK ]       = new FColorProfile( DefaultProfiles::Default_CMYK,        cmsOpenProfileFromMem( ::ULIS::details::Default_Profile_CMYK_SNAP, sizeof( ::ULIS::details::Default_Profile_CMYK_SNAP ) ));

    cmsFreeToneCurve( gamma_2_2 );
}


FProfileRegistry::~FProfileRegistry()
{
    for( auto it : mLockedSoftwareProfiles )
        delete  it.second;
    mLockedSoftwareProfiles.clear();

    for( auto it : mFileBasedProfiles )
        delete  it.second;
    mFileBasedProfiles.clear();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------- Public API
FColorProfile*
FProfileRegistry::GetProfile( const std::string iKey )
{
    if ( mLockedSoftwareProfiles.find( iKey ) != mLockedSoftwareProfiles.end() )
        return  mLockedSoftwareProfiles[iKey];

    if ( mFileBasedProfiles.find( iKey ) != mFileBasedProfiles.end() )
        return  mFileBasedProfiles[iKey];

    return  nullptr;
}


std::string
FProfileRegistry::DefaultProfileNameForModel( e_cm iModel )
{
    switch( iModel )
    {
        case e_cm::kG:      return  "Grey_Gamma2.2_D65";
        case e_cm::kRGB:    return  "sRGB";
        case e_cm::kHSL:    return  "sRGB";
        case e_cm::kHSV:    return  "sRGB";
        case e_cm::kCMYK:   return  "Default_CMYK";
        case e_cm::kLab:    return  "Lab_D65";
        case e_cm::kXYZ:    return  "XYZ";
        default:            return  "invalid";
    }
}


FColorProfile*
FProfileRegistry::GetDefaultProfileForModel( e_cm iModel )
{
    return  GetProfile( DefaultProfileNameForModel( iModel ) );
}


} // namespace ULIS
