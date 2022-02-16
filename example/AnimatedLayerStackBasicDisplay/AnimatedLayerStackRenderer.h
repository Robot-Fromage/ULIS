// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerStackRenderer.h
* @author       Clement Berthaud
* @brief        AnimatedLayerStackBasicDisplay application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>

class FAnimatedLayerStackRenderer
{
private:
    static ::ULIS::FEvent RenderImage(
          ::ULIS::FContext& iCtx
        , ::ULIS::FAnimatedLayerImage& iLayer
        , ::ULIS::FBlock& ioBlock
        , uint64_t iFrame
        , const ::ULIS::FRectI& iRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPos = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy()
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
    );

    static ::ULIS::FEvent RenderFolder(
          ::ULIS::FContext& iCtx
        , ::ULIS::FAnimatedLayerFolder& iLayer
        , ::ULIS::FBlock& ioBlock
        , uint64_t iFrame
        , const ::ULIS::FRectI& iRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPos = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy()
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
    );

    static ::ULIS::FEvent RenderText(
          ::ULIS::FContext& iCtx
        , ::ULIS::FAnimatedLayerText& iLayer
        , ::ULIS::FBlock& ioBlock
        , uint64_t iFrame
        , const ::ULIS::FRectI& iRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPos = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy()
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
    );

    static ::ULIS::FEvent RenderStack(
          ::ULIS::FContext& iCtx
        , ::ULIS::FAnimatedLayerStack& iStack
        , ::ULIS::FBlock& ioBlock
        , uint64_t iFrame
        , const ::ULIS::FRectI& iRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPos = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy()
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
    );

public:
    static ::ULIS::ulError Render(
          ::ULIS::FContext& iCtx
        , ::ULIS::FAnimatedLayerStack& iStack
        , ::ULIS::FBlock& oDestination
        , ::ULIS::ufloat iTime
        , const ::ULIS::FRectI& iSourceRect = ::ULIS::FRectI::Auto
        , const ::ULIS::FVec2I& iPosition = ::ULIS::FVec2I( 0 )
        , const ::ULIS::FSchedulePolicy& iPolicy = ::ULIS::FSchedulePolicy::MultiScanlines
        , ::ULIS::uint32 iNumWait = 0
        , const ::ULIS::FEvent* iWaitList = nullptr
        , ::ULIS::FEvent* iEvent = nullptr
    );
};

