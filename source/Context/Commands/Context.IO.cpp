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
#include "Context/Context.h"
#include "Context/ContextualDispatchTable.h"
#include "Process/IO/Disk.h"
#include "Image/Block.h"
#include "Scheduling/Command.h"
#include "Scheduling/CommandQueue.h"
#include "Scheduling/CommandQueue_Private.h"
#include "Scheduling/Event.h"
#include "Scheduling/Event_Private.h"
#include "Scheduling/InternalEvent.h"

#include <fstream>

#include <stb_image.h>

#include <filesystem>
namespace fs = std::filesystem;

ULIS_NAMESPACE_BEGIN
ulError
FContext::XLoadBlockFromDisk(
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
FContext::SaveBlockToDisk(
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

/*
ulError
FContext::LoadProxyFromDisk(
      FBlock& ioBlock
    , eFormat iFormat
    , const std::string& iPath
    , const FSchedulePolicy& iPolicy
    , uint32 iNumWait
    , const FEvent* iWaitList
    , FEvent* iEvent
)
{
    // TODO: Async Block Allocation
    bool disk_exists = false;
    FVec2I disk_size( 0, 0 );
    eFormat disk_format = Format_RGBA8;
    LoadBlockFromDiskMetrics( iPath, &disk_exists, &disk_size, &disk_format );
    ULIS_ASSERT_RETURN_ERROR( disk_exists, "File doesn't exist.", ULIS_ERROR_BAD_INPUT_DATA );

    if( disk_format != iFormat ) {
        ioBlock.ReallocInternalData( disk_size.x, disk_size.y, iFormat, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ) );
        FBlock* src_hollow = new FBlock();

        FEvent subcommand_event;
        ulError err = LoadBlockFromDisk( *src_hollow, iPath, iPolicy, iNumWait, iWaitList, &subcommand_event );
        ULIS_ASSERT_RETURN_ERROR( FinishEventNo_OP( iEvent, err ), "Error occured within subcommand" );

        FEvent maincommand_event( FOnEventComplete( []( const FRectI&, void* iUserData ){ delete  reinterpret_cast< FBlock* >( iUserData ); }, src_hollow ) );
        err = ConvertFormat( *src_hollow, ioBlock, ioBlock.Rect(), FVec2I( 0, 0 ), iPolicy, 1, &subcommand_event, &maincommand_event );
        ULIS_ASSERT_RETURN_ERROR( FinishEventNo_OP( iEvent, err ), "Error occured within maincommand" );

        err = Dummy_OP( 1, &maincommand_event, iEvent );
        ULIS_ASSERT_RETURN_ERROR( FinishEventNo_OP( iEvent, err ), "Error occured within postcommand" );
        return  ULIS_NO_ERROR;
    } else {
        return  LoadBlockFromDisk( ioBlock, iPath, iPolicy, iNumWait, iWaitList, iEvent );
    }
}
*/

/*
ulError
FContext::SaveProxyToDisk(
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

    if( !( layout_valid && model_valid && type_valid ) ) {
        eFormat conv_format = static_cast< eFormat >( 0 );
        if( iFileFormat == FileFormat_hdr ) conv_format = eFormat::Format_RGBF;
        else if( model == CM_GREY )         conv_format = static_cast< eFormat >( eFormat::Format_G8   | ULIS_W_ALPHA( hasAlpha ) );
        else                                conv_format = static_cast< eFormat >( eFormat::Format_RGB8 | ULIS_W_ALPHA( hasAlpha ) );
        FBlock* conv = new FBlock( iBlock.Width(), iBlock.Height(), conv_format );

        FEvent subcommand_event;
        ulError err = ConvertFormat( iBlock, *conv, iBlock.Rect(), FVec2I( 0, 0 ), iPolicy, iNumWait, iWaitList, &subcommand_event );
        ULIS_ASSERT_RETURN_ERROR( FinishEventNo_OP( iEvent, err ), "Error occured within subcommand" );

        // Lambda with no captures can be used as function pointers. The TCallback and reinterpret_cast are used as a primitive capture instead.
        FEvent maincommand_event( FOnEventComplete( []( const FRectI&, void* iUserData ){ delete  reinterpret_cast< FBlock* >( iUserData ); }, conv ) );
        err = SaveBlockToDisk( *conv, iPath, iFileFormat, iQuality, iPolicy, 1, &subcommand_event, &maincommand_event );
        ULIS_ASSERT_RETURN_ERROR( FinishEventNo_OP( iEvent, err ), "Error occured within maincommand" );

        err = Dummy_OP( 1, &maincommand_event, iEvent );
        ULIS_ASSERT_RETURN_ERROR( FinishEventNo_OP( iEvent, err ), "Error occured within postcommand" );
        return  ULIS_NO_ERROR;
    } else {
        return  SaveBlockToDisk( iBlock, iPath, iFileFormat, iQuality, iPolicy, iNumWait, iWaitList, iEvent );
    }
}
*/

//static
void
FContext::LoadBlockFromDiskMetrics(
      const std::string& iPath
    , bool *oFileExists
    , FVec2I *oSize
    , eFormat *oFormat
)
{
    *oFileExists = false;
    *oSize = FVec2I( 0, 0 );
    *oFormat = eFormat::Format_RGBA8;

    // Does the file exist ?
    fs::path path( iPath );
    if( ( !( fs::exists( path ) ) ) || ( !( fs::is_regular_file( path ) ) ) )
       return;

    // Can we read it right now ?
    std::ifstream file( iPath.c_str(), std::ios::binary | std::ios::ate );
    std::streamsize size = file.tellg();
    file.seekg( 0, std::ios::beg );
    std::vector<char> buffer( size );
    if( !file.read( buffer.data(), size ) )
        return;

    // Can we get info about its format ?
    int width, height, numchannels;
    stbi_info_from_memory( (const stbi_uc*)buffer.data(), static_cast< int >( size ), &width, &height, &numchannels );

    eType type = Type_uint8;
    if( stbi_is_16_bit_from_memory( (const stbi_uc*)buffer.data(), static_cast< int >( size ) ) )
        type = Type_uint16;
    else if( stbi_is_hdr_from_memory( (const stbi_uc*)buffer.data(), static_cast< int >( size ) ) )
        type = Type_ufloat;

    int depth = 1;
    bool floating = false;
    switch( type ) {
        case Type_uint8:    depth = 1; floating = false;  break;
        case Type_uint16:   depth = 2; floating = false;  break;
        case Type_ufloat:   depth = 4; floating = true;   break;
    }

    eColorModel model;
    bool hea = false;
    switch( numchannels ) {
        case 1: model = CM_GREY;    hea = false;    break;
        case 2: model = CM_GREY;    hea = true;     break;
        case 3: model = CM_RGB;     hea = false;    break;
        case 4: model = CM_RGB;     hea = true;     break;
    }
    int color_channels = numchannels - hea;

    eFormat fmt =
        static_cast< eFormat >(
              ULIS_W_TYPE( type )
            | ULIS_W_CHANNELS( color_channels )
            | ULIS_W_MODEL( model )
            | ULIS_W_ALPHA( hea )
            | ULIS_W_DEPTH( depth )
            | ULIS_W_FLOATING( floating )
        );

    *oFileExists = true;
    *oSize = FVec2I( width, height );
    *oFormat = fmt;
    return;
}

//static
void
FContext::SaveBlockToDiskMetrics( const FBlock& iBlock, eFileFormat iFileFormat, bool *oCanSaveWithoutProxy )
{
    ULIS_ASSERT( oCanSaveWithoutProxy, "Bad input" );
    *oCanSaveWithoutProxy = false;
    eType type = iBlock.Type();
    eFormat format = iBlock.Format();
    eColorModel model = iBlock.Model();

    bool layout_valid = ULIS_R_RS( format ) == 0;
    bool model_valid = model == CM_GREY || model == CM_RGB;
    bool type_valid = ( iFileFormat != FileFormat_hdr && type == Type_uint8 ) ||
                      ( iFileFormat == FileFormat_hdr && type == Type_ufloat && model == CM_RGB );

    if( ( layout_valid && model_valid && type_valid ) )
        *oCanSaveWithoutProxy = true;
}

ULIS_NAMESPACE_END

