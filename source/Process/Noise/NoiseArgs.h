// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         NoiseArgs.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for NoiseArgs.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Process/Conv/Conv.h"
#include "Scheduling/Dispatcher.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Process/Noise/Func/NoiseFunc.h"
#include "Image/Block.h"
#include "Scheduling/ScheduleArgs.h"
#include "Scheduling/SimpleBufferArgs.h"
#include <vector>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// FNoiseCommandArgs
class FBrownianNoiseCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FBrownianNoiseCommandArgs() override
    {
    }

    FBrownianNoiseCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , int iSeed
        , float iFrequency
        , float iFrequencyMult
        , float iAmplitudeMult
        , uint8 iNumLayers
        , float iAmplitudeMax
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , noise( iSeed )
        , frequency( iFrequency )
        , frequencyMult( iFrequencyMult )
        , amplitudeMult( iAmplitudeMult )
        , numLayers( iNumLayers )
        , amplitudeMax( iAmplitudeMax )
    {}

    FValueNoise noise;
    float frequency;
    float frequencyMult;
    float amplitudeMult;
    uint8 numLayers;
    float amplitudeMax;
};

/////////////////////////////////////////////////////
// FValueNoiseCommandArgs
class FValueNoiseCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FValueNoiseCommandArgs() override
    {
    }

    FValueNoiseCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , int iSeed
        , float iFrequency
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , noise( iSeed )
        , frequency( iFrequency )
    {}

    FValueNoise noise;
    float frequency;
};

/////////////////////////////////////////////////////
// FVoronoiNoiseCommandArgs
class FVoronoiNoiseCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FVoronoiNoiseCommandArgs() override
    {
    }

    FVoronoiNoiseCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , int iSeed
        , float iNormalisationFactor
        , std::vector< FVec2F >&& iPoints
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , seed( iSeed )
        , normalisationFactor( iNormalisationFactor )
        , points( std::move( iPoints ) )
    {}

    int seed;
    float normalisationFactor;
    std::vector< FVec2F > points;
};

/////////////////////////////////////////////////////
// FWhiteNoiseCommandArgs
class FWhiteNoiseCommandArgs final
    : public FSimpleBufferCommandArgs
{
public:
    ~FWhiteNoiseCommandArgs() override
    {
    }

    FWhiteNoiseCommandArgs(
          FBlock& iBlock
        , const FRectI& iRect
        , int iSeed
    )
        : FSimpleBufferCommandArgs( iBlock, iRect )
        , seed( iSeed )
    {}

    int seed;
};

ULIS_NAMESPACE_END

