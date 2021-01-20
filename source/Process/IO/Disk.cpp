// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Disk.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the disk IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/IO/Disk.h"
#include "Image/Block.h"

#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>

#include <filesystem>
namespace fs = std::filesystem;

ULIS_NAMESPACE_BEGIN
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeLoadFromFile_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FDiskIOCommandArgs* cargs
)
{
    fs::path path( cargs->path );
    if( ( !( fs::exists( path ) ) ) || ( !( fs::is_regular_file( path ) ) ) )
       return;

    std::ifstream file( cargs->path.c_str(), std::ios::binary | std::ios::ate );
    std::streamsize size = file.tellg();
    file.seekg( 0, std::ios::beg );
    std::vector<char> buffer( size );
    if( !file.read( buffer.data(), size ) )
        return;

    eType type = Type_uint8;
    if( stbi_is_16_bit_from_memory( (const stbi_uc*)buffer.data(), static_cast< int >( size ) ) )
        type = Type_uint16;
    else if( stbi_is_hdr_from_memory( (const stbi_uc*)buffer.data(), static_cast< int >( size ) ) )
        type = Type_ufloat;

    uint8* data = nullptr;
    int width, height, channels, depth;
    width = height = channels = depth = 1;
    bool floating;
    switch( type ) {
        case Type_uint8:    data = (uint8*)stbi_load(       cargs->path.c_str(), &width, &height, &channels, 0 ); depth = 1; floating = false;  break;
        case Type_uint16:   data = (uint8*)stbi_load_16(    cargs->path.c_str(), &width, &height, &channels, 0 ); depth = 2; floating = false;  break;
        case Type_ufloat:   data = (uint8*)stbi_loadf(      cargs->path.c_str(), &width, &height, &channels, 0 ); depth = 4; floating = true;   break;
    }

    ULIS_ASSERT( data, "Error bad input file" );

    eColorModel model;
    bool hea;
    switch( channels ) {
        case 1: model = CM_GREY;    hea = false;    break;
        case 2: model = CM_GREY;    hea = true;     break;
        case 3: model = CM_RGB;     hea = false;    break;
        case 4: model = CM_RGB;     hea = true;     break;
    }
    int color_channels = channels - hea;

    eFormat fmt =
        static_cast< eFormat >(
              ULIS_W_TYPE( type )
            | ULIS_W_CHANNELS( color_channels )
            | ULIS_W_MODEL( model )
            | ULIS_W_ALPHA( hea )
            | ULIS_W_DEPTH( depth )
            | ULIS_W_FLOATING( floating )
        );

    cargs->dst.LoadFromData( data, width, height, fmt, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ) );
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeSaveToFile_MEM_Generic(
      const FSimpleBufferJobArgs* jargs
    , const FDiskIOCommandArgs* cargs
)
{
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleLoadFromFile_MEM_Generic, FSimpleBufferJobArgs, FDiskIOCommandArgs, &InvokeLoadFromFile_MEM_Generic )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleSaveToFile_MEM_Generic, FSimpleBufferJobArgs, FDiskIOCommandArgs, &InvokeSaveToFile_MEM_Generic )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedLoadFromFileInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedSaveToFileInvocationSchedulerSelector )

void SaveToFile( FOldThreadPool*           iOldThreadPool
               , bool                   iBlocking
               , uint32                 iPerfIntent
               , const FHardwareMetrics& iHostDeviceInfo
               , bool                   iCallCB
               , const FBlock*          iSource
               , const std::string&     iPath
               , eImageFormat           iImageFormat
               , int                    iQuality )
{
    ULIS_ASSERT( iSource,             "Bad source."                                           );
    ULIS_ASSERT( iOldThreadPool,              "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    //cppfs::FilePath     path( iPath );
    //std::string         ext = path.extension();
    eFormat     format  = iSource->Format();
    eColorModel model   = iSource->Model();
    eType       type    = iSource->Type();

    bool layout_valid   = ULIS_R_RS( format ) == 0;
    bool model_valid    = model == CM_GREY || model == CM_RGB;
    bool type_valid     = ( iImageFormat != IM_HDR && type == Type_uint8 ) || ( iImageFormat == IM_HDR && type == Type_ufloat && model == CM_RGB );

    int w = iSource->Width();
    int h = iSource->Height();
    int c = iSource->SamplesPerPixel();
    const uint8* dat = iSource->Bits();
    FBlock* conv = nullptr;
    if( !( layout_valid && model_valid && type_valid ) ) {
        eFormat dstformat = static_cast< eFormat >( 0 );
        if( iImageFormat == IM_HDR )    dstformat = eFormat::Format_RGBF;
        else if( model == CM_GREY )     dstformat = static_cast< eFormat >( eFormat::Format_G8   | ULIS_W_ALPHA( iSource->HasAlpha() ) );
        else                            dstformat = static_cast< eFormat >( eFormat::Format_RGB8 | ULIS_W_ALPHA( iSource->HasAlpha() ) );
        conv = XConv( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, dstformat );
        dat = conv->Bits();
    }

    switch( iImageFormat ) {
        case IM_PNG: stbi_write_png( iPath.c_str(), w, h, c, dat, 0         );  break; // stride: 0
        case IM_BMP: stbi_write_bmp( iPath.c_str(), w, h, c, dat            );  break;
        case IM_TGA: stbi_write_tga( iPath.c_str(), w, h, c, dat            );  break;
        case IM_JPG: stbi_write_jpg( iPath.c_str(), w, h, c, dat, iQuality  );  break; // Quality: 0 - 100;
        case IM_HDR: stbi_write_hdr( iPath.c_str(), w, h, c, (float*)dat    );  break;
    }

    if( conv )
        delete  conv;
}
*/

ULIS_NAMESPACE_END

