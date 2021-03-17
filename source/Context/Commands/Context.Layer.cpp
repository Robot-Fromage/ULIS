// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.Layer.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the Layer API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Image/Block.h"
#include "Layer/LayerStack.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"
#include "Process/Conv/Conv.h"
#include "Process/Layer/PSD.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Entry points
ulError
FContext::Flatten(
      FLayerStack& iStack
    , FBlock& oDestination
    , const FRectI& iSourceRect
    , const FVec2I& iPosition
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    ULIS_ASSERT_RETURN_ERROR(
          iStack.Format() == oDestination.Format()
        , "Formats mismatch."
        , FinishEventNo_OP( iEvent, ULIS_ERROR_FORMATS_MISMATCH )
    );

    // Sanitize geometry
    const FRectI src_rect = iStack.Rect();
    const FRectI dst_rect = oDestination.Rect();
    const FRectI src_roi = iSourceRect.Sanitized() & src_rect;
    const FRectI dst_aim = FRectI::FromPositionAndSize( iPosition, src_roi.Size() );
    const FRectI dst_roi = dst_aim & dst_rect;

    // Check no-op
    if( dst_roi.Area() <= 0 )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_GEOMETRY );

    // Bake and push multiple subcommands in recursive local lambda.
    //          L1  L2  L3
    //  • root <-
    //  |-0             ]
    //  |-1             ]
    //  |-• 2       ]   ]
    //  | |-3       ]   ]
    //  | |-• a ]   ]   ]
    //  | | |-b ]   ]   ]
    //  | | |-c ]   ]   ]
    //  | | |-d ]   ]   ]
    //  |-5             ]
    //  |-• 6   ]       ]
    //  | |-7   ]       ]
    //  | |-8   ]       ]

    // Bake Policy
    FHardwareMetrics hw;
    FSchedulePolicy policyClear( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Chunks, ScheduleParameter_Length, hw.L1CacheSize() );
    FSchedulePolicy policyBlend( ScheduleTime_Sync, ScheduleRun_Multi, ScheduleMode_Scanlines );

    // Function object declared first
    std::function< void( FLayerRoot&, FBlock& oDst, FEvent& oEvent ) > sched;

    // Lambda definition
    sched =
        [ &sched, policyClear, policyBlend, src_roi, dst_roi, iPosition, this ]
        ( FLayerRoot& iFolder, FBlock& oDst, FEvent& oEvent ) -> void
    {
        TArray< FEvent > events( iFolder.Layers().Size() + 1 );
        Clear( oDst, dst_roi, policyClear, 0, nullptr, &(events.Back()) );

        const int64 max = static_cast< int64 >( iFolder.Layers().Size() ) - 1;
        for( int64 i = max; i >= 0; --i ) {
            eLayerType type = iFolder.Layers()[i]->Type();
            FLayerImage* layer = nullptr;
            switch( type ) {
                case Layer_Image: {
                    layer = dynamic_cast< FLayerImage* >( iFolder.Layers()[i] );
                    break;
                }
                case Layer_Folder: {
                    FLayerFolder* folder = dynamic_cast< FLayerFolder* >( iFolder.Layers()[i] );
                    layer = dynamic_cast< FLayerImage* >( folder );
                    // block = &(folder.Block());
                    // sched( folder, folder.Block(), events[i] );
                    break;
                }
            }

            if( layer )
            {
                Blend(
                      layer->Block()
                    , oDst
                    , src_roi
                    , iPosition
                    , layer->BlendMode()
                    , layer->AlphaMode()
                    , layer->Opacity()
                    , policyBlend
                    , 1
                    , &events[i + 1]
                    , &events[i]
                );
            }
        }
    };

    FEvent event;
    // Call recursive lambda sched.
    sched( iStack, oDestination, event );

    return  ULIS_NO_ERROR;
}

ulError
FContext::XLoadPSDFromDisk(
      FLayerStack& iStack
    , const std::string& iPath
    , const FRectI& iSourceRect
    , const FVec2I& iPosition
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    FPSDOperations op( iPath );
    op.Import();
    return  ULIS_NO_ERROR;
}

ULIS_NAMESPACE_END

