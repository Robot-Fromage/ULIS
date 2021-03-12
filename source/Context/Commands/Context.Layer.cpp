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

    // Bake and push multiple subcommands
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
    std::function< void( FLayerRoot&, FBlock& oDst, FEvent& oEvent ) > sched;
    sched = [&sched, this]( FLayerRoot& iFolder, FBlock& oDst, FEvent& oEvent )->void {
        TArray< FEvent > events( iFolder.Layers().Size() );
        TArray< FBlock* > blocks( iFolder.Layers().Size() );
        for( uint64 i = 0; i < iFolder.Layers().Size(); ++i ) {
            eLayerType type = iFolder.Layers()[i]->Type();
            switch( type ) {
                case Layer_Image: {
                    FLayerImage& img = dynamic_cast< FLayerImage& >( *( iFolder.Layers()[i] ) );
                    blocks[i] = &(img.Block());
                    break;
                }
                case Layer_Folder: {
                    FLayerFolder& folder = dynamic_cast< FLayerFolder& >( *( iFolder.Layers()[i] ) );
                    blocks[i] = &(folder.Block());
                    sched( folder, folder.Block(), events[i] );
                    break;
                }
            }
        }
        for( uint64 i = 1; i < iFolder.Layers().Size(); ++i ) {
            Blend(
                  *blocks[i]
                , oDst
                , blocks[i]->Rect()
                , FVec2I()
                , Blend_Normal
                , Alpha_Normal
                , 1.f
                , FSchedulePolicy()
                , 1
                , &events[i + 1]
                , &events[i]
            );
        }
    };
    FEvent event;
    sched( iStack.Root(), oDestination, event );

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

