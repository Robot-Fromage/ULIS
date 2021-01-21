// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Context.IO.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation of the IO API entry
*               points in the FContext class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/IO/Disk.h"
//#include "Process/IO/Clipboard.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

#include <filesystem>
namespace fs = std::filesystem;

ULIS_NAMESPACE_BEGIN
ulError
FContext::FileLoad(
      FBlock& ioBlock
    , const std::string& iPath
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    fs::path path( iPath );
    if( ( !( fs::exists( path ) ) ) || ( !( fs::is_regular_file( path ) ) ) )
       return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_BAD_INPUT_DATA );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleFileLoad
            , new FDiskIOCommandArgs(
                  ioBlock
                , FRectI( 0, 0, INT_MAX, INT_MAX )
                , iPath
            )
            , iPolicy
            , true
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , FRectI( 0, 0, INT_MAX, INT_MAX )
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::FileSave(
      const FBlock& iBlock
    , const std::string& iPath
    , eFileFormat iFileFormat
    , int iQuality
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    eType type = iBlock.Type();
    eFormat format = iBlock.Format();
    eColorModel model = iBlock.Model();

    bool layout_valid = ULIS_R_RS( format ) == 0;
    bool model_valid = model == CM_GREY || model == CM_RGB;
    bool type_valid = ( iFileFormat != FileFormat_hdr && type == Type_uint8 ) ||
                      ( iFileFormat == FileFormat_hdr && type == Type_ufloat && model == CM_RGB );

    if( !( layout_valid && model_valid && type_valid ) )
        return  FinishEventNo_OP( iEvent, ULIS_WARNING_NO_OP_BAD_FILE_FORMAT );

    // Bake and push command
    mCommandQueue.d->Push(
        new FCommand(
              mContextualDispatchTable->mScheduleFileSave
            , new FDiskIOCommandArgs(
                  const_cast< FBlock& >( iBlock )
                , iBlock.Rect()
                , iPath
                , iFileFormat
                , iQuality
            )
            , iPolicy
            , true
            , true
            , iNumWait
            , iWaitList
            , iEvent
            , iBlock.Rect()
        )
    );

    return  ULIS_NO_ERROR;
}

ulError
FContext::FileSaveConvSafe(
      const FBlock& iBlock
    , const std::string& iPath
    , eFileFormat iFileFormat
    , int iQuality
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    eType type = iBlock.Type();
    eFormat format = iBlock.Format();
    eColorModel model = iBlock.Model();
    bool hasAlpha = iBlock.HasAlpha();

    bool layout_valid = ULIS_R_RS( format ) == 0;
    bool model_valid = model == CM_GREY || model == CM_RGB;
    bool type_valid = ( iFileFormat != FileFormat_hdr && type == Type_uint8 ) ||
                      ( iFileFormat == FileFormat_hdr && type == Type_ufloat && model == CM_RGB );

    FEvent preprocess_conv_event;
    const FBlock* target = &iBlock;
    if( !( layout_valid && model_valid && type_valid ) ) {
        eFormat conv_format = static_cast< eFormat >( 0 );
        if( iFileFormat == FileFormat_hdr ) conv_format = eFormat::Format_RGBF;
        else if( model == CM_GREY )         conv_format = static_cast< eFormat >( eFormat::Format_G8   | ULIS_W_ALPHA( hasAlpha ) );
        else                                conv_format = static_cast< eFormat >( eFormat::Format_RGB8 | ULIS_W_ALPHA( hasAlpha ) );
        FBlock* conv = new FBlock( iBlock.Width(), iBlock.Height() );
        target = conv;
        ulError err = ConvertFormat( iBlock, *conv, iBlock.Rect(), FVec2I( 0, 0 ), iPolicy, iNumWait, iWaitList, &preprocess_conv_event );
        if( err )
            ULIS_ASSERT( false, "Error occured within subcommand" );

    } else {
        FinishEventNo_OP( &preprocess_conv_event, ULIS_NO_ERROR );
    }

    // TODO: extra wait event preprocess_conv_event
    // TODO: extra cleanup conv after FileSave is finished
    return  FileSave( *target, iPath, iFileFormat, iQuality, iPolicy, iNumWait, iWaitList, iEvent );
}

ulError
FContext::ClipboardLoad(
      FBlock& ioBlock
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    return  ULIS_NO_ERROR;
}

ulError
FContext::ClipboardSave(
      const FBlock& iBlock
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    return  ULIS_NO_ERROR;
}

//static
bool
ClipboardCanBeLoadedFrom()
{
    return  false;
}

//static
bool
ClipboardCanBeSavedTo()
{
    return  false;
}

//static
bool
FileCanBeLoadedFrom()
{
    return  false;
}

//static
bool
FileCanBeSavedTo()
{
    return  false;
}

ULIS_NAMESPACE_END

