// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasTimeSettings.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasTimeSettings class.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasTimeSettings.h"
#include "Math/Math.h"

ULIS_NAMESPACE_BEGIN

IHasTimeSettings::IHasTimeSettings(
      ufloat iFps
    , int64 iBeginFrame
    , int64 iEndFrame
    , int64 iCurrentFrame
    , const FOnTimeSettingsChanged& iDelegate
)
    : FOnTimeSettingsChanged( iDelegate )
    , mInfo{
          FMath::Max( iFps, FMath::kEpsilonf )
        , iBeginFrame
        , iEndFrame
        , iCurrentFrame
    }
{}

ufloat IHasTimeSettings::Fps()
{
    return mInfo.fps;
}

int64 IHasTimeSettings::BeginFrame()
{
    return mInfo.beginFrame;
}

int64 IHasTimeSettings::EndFrame()
{
    return mInfo.endFrame;
}

int64 IHasTimeSettings::CurrentFrame()
{
    return mInfo.currentFrame;
}

int64 IHasTimeSettings::GetFrameAtTime( ufloat iTimeSeconds ) {
    return  static_cast< int64 >( FMath::RoundToNegativeInfinity( mInfo.fps * iTimeSeconds ) );
}

ufloat IHasTimeSettings::GetIntervalSeconds() const {
    return  1.f / mInfo.fps;
}

void IHasTimeSettings::SetFps( ufloat iValue )
{
    mInfo.fps = FMath::Max( iValue, FMath::kEpsilonf );
}

void IHasTimeSettings::SetBeginFrame( int64 iValue )
{
    mInfo.beginFrame = iValue;
}

void IHasTimeSettings::SetEndFrame( int64 iValue )
{
    mInfo.endFrame = iValue;
}

void IHasTimeSettings::SetCurrentFrame( int64 iValue )
{
    mInfo.currentFrame = iValue;
}

ULIS_NAMESPACE_END

