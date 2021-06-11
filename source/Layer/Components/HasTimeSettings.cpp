// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasTimeSettings.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the IHasTimeSettings class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/Components/HasTimeSettings.h"

ULIS_NAMESPACE_BEGIN

IHasTimeSettings::IHasTimeSettings( uint16 iFps, int64 iBeginFrame, int64 iEndFrame, int64 iCurrentFrame )
    :mInfo( { iFps, iBeginFrame, iEndFrame, iCurrentFrame } )
{}

uint16 IHasTimeSettings::Fps()
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

void IHasTimeSettings::SetFps( uint16 iValue )
{
    mInfo.fps = iValue;
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

