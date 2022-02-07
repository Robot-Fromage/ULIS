// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStackRenderer.h
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include "LayerVector.h"

class FLayerStackRenderer
{
private:
    static ::ULIS::FEvent RenderImage(
          ::ULIS::FContext& iCtx
        , ::ULIS::FLayerImage& iLayer
        , ::ULIS::FBlock& ioBlock
        , const ::ULIS::FRectI& iRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPos = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy()
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
    );

    static ::ULIS::FEvent RenderFolder(
          ::ULIS::FContext& iCtx
        , ::ULIS::FLayerFolder& iLayer
        , ::ULIS::FBlock& ioBlock
        , const ::ULIS::FRectI& iRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPos = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy()
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
    );

    static ::ULIS::FEvent RenderText(
          ::ULIS::FContext& iCtx
        , ::ULIS::FLayerText& iLayer
        , ::ULIS::FBlock& ioBlock
        , const ::ULIS::FRectI& iRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPos = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy()
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
    );

    static ::ULIS::FEvent RenderVector(
          ::ULIS::FContext& iCtx
        , FLayerVector& iLayer
        , ::ULIS::FBlock& ioBlock
        , const ::ULIS::FRectI& iRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPos = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy()
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
    );

    static ::ULIS::FEvent RenderStack(
          ::ULIS::FContext& iCtx
        , ::ULIS::FLayerStack& iStack
        , ::ULIS::FBlock& ioBlock
        , const ::ULIS::FRectI& iRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPos = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy()
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
    );

public:
    static ::ULIS::ulError Render(
          ::ULIS::FContext& iCtx
        , ::ULIS::FLayerStack& iStack
        , ::ULIS::FBlock& oDestination
        , const ::ULIS::FRectI& iSourceRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPosition = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy::MultiScanlines
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
        , ::ULIS::FEvent* iEvent = nullptr
    );
};

