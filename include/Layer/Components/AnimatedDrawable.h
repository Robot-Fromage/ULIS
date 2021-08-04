// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedDrawable.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the TAnimatedDrawable class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Scheduling/Event.h"
#include "Scheduling/SchedulePolicy.h"
#include "Layer/AnimatedLayer/Cel.h"

ULIS_NAMESPACE_BEGIN

template< class BlockType > class TAnimatedDrawable;
template< class BlockType > using TOnFrameChangedDelegate = TLambdaCallback< void, const TAnimatedDrawable< BlockType >*, uint32 >;
template< class BlockType > using TOnCelAddedDelegate = TLambdaCallback< void, const TAnimatedDrawable< BlockType >*, uint32 >;
template< class BlockType > using TOnCelChangedDelegate = TLambdaCallback< void, const TAnimatedDrawable< BlockType >*, uint32 >;
template< class BlockType > using TOnCelRemovedDelegate = TLambdaCallback< void, const TAnimatedDrawable< BlockType >*, uint32 >;

template< class BlockType > using TOnFrameChanged = TCallbackCapable< TOnFrameChangedDelegate< BlockType >, 0 >;
template< class BlockType > using TOnCelAdded = TCallbackCapable< TOnCelAddedDelegate< BlockType >, 1 >;
template< class BlockType > using TOnCelChanged = TCallbackCapable< TOnCelChangedDelegate< BlockType >, 2 >;
template< class BlockType > using TOnCelRemoved = TCallbackCapable< TOnCelRemovedDelegate< BlockType >, 3 >;


/////////////////////////////////////////////////////
/// @class      TAnimatedDrawable
/// @brief      The TAnimatedDrawable class provides a template interface to
///             perform cached renders of contents for layers
template< class BlockType >
class TAnimatedDrawable
{
protected:
    TAnimatedDrawable();

public:
    virtual FEvent RenderImageCache( FContext& iCtx, uint32 iFrame );
    virtual FEvent RenderImage(
          FContext& iCtx
        , BlockType& ioBlock
        , uint32 iFrame
        , const FRectI& iRect = FRectI::Auto
        , const FVec2I& iPos = FVec2I( 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
    ) = 0;


    bool IsImageCacheValid(uint32 iFrame) const;
    virtual void InvalidImageCache(uint32 iFrame);

    virtual const TArray<FCelInfo> GetDrawableCelInfos(uint32* oFirstFrame) const;

protected:
    void ValidateImageCache(uint32 iFrame);

private:
    uint32 mCacheValid;
};

ULIS_NAMESPACE_END

#include "Layer/Components/AnimatedDrawable.tpp"

