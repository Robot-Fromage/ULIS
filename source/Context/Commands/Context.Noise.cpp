// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Clear.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the clear API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/Noise/Noise.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Scheduling/SimpleBufferArgs.h"
#include <ctime>

ULIS_NAMESPACE_BEGIN
ulError
FContext::WhiteNoise(
      FBlock& iBlock
    , int iSeed
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    return  ULIS_NO_ERROR;
}

ulError
FContext::ValueNoise(
      FBlock& iBlock
    , float iFreq
    , int iSeed
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR( iBlock.Format() == Format(), "Bad format", ULIS_ERROR_FORMATS_MISMATCH )

    // Sanitize geometry
    const FRectI rect = iBlock.Rect();
    const FRectI roi = iRect.Sanitized() & rect;

    // Check no-op
    if( roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    int seed = iSeed < 0 ? time( NULL ) : iSeed;

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleValueNoise
            , new FValueNoiseCommandArgs(
                  iBlock
                , roi
                , seed
                , iFreq
            )
            , iPolicy
            , false
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::BrownianNoise(
      FBlock& iBlock
    , float iFrequency
    , float iFrequencyMult
    , float iAmplitudeMult
    , uint8 iNumLayers
    , int iSeed
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR( iBlock.Format() == Format(), "Bad format", ULIS_ERROR_FORMATS_MISMATCH )

    // Sanitize geometry
    const FRectI rect = iBlock.Rect();
    const FRectI roi = iRect.Sanitized() & rect;

    // Check no-op
    if( roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    int seed = iSeed < 0 ? time( NULL ) : iSeed;
    float amplitudeMax = 0;
    {
        float amplitude = 1.0f;
        for( int i = 0; i < iNumLayers; ++i )
        {
            amplitudeMax += amplitude;
            amplitude *= iAmplitudeMult;
        }
    }

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleBrownianNoise
            , new FBrownianNoiseCommandArgs(
                  iBlock
                , roi
                , seed
                , iFrequency
                , iFrequencyMult
                , iAmplitudeMult
                , iNumLayers
                , amplitudeMax
            )
            , iPolicy
            , false
            , false
            , iNumWait
            , iWaitList
            , iEvent
            , roi
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::VoronoiNoise(
      FBlock& iBlock
    , uint32 iCount
    , int iSeed
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    return  ULIS_NO_ERROR;
}

ulError
FContext::Clouds(
      FBlock& iBlock
    , int iSeed
    , const FRectI& iRect
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    return  BrownianNoise( iBlock, 0.02f, 2.f, 0.5f, 5, iSeed, iRect, iPolicy, iNumWait, iWaitList, iEvent );
}

ULIS_NAMESPACE_END

