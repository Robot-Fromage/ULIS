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
    eType type = cargs->dst.Type();
    eFormat format = cargs->dst.Format();
    eColorModel model = cargs->dst.Model();

    bool layout_valid = ULIS_R_RS( format ) == 0;
    bool model_valid = model == CM_GREY || model == CM_RGB;
    bool type_valid = ( cargs->fileFormat != FileFormat_hdr && type == Type_uint8 ) ||
                      ( cargs->fileFormat == FileFormat_hdr && type == Type_ufloat && model == CM_RGB );

    int w = cargs->dst.Width();
    int h = cargs->dst.Height();
    int c = cargs->dst.SamplesPerPixel();
    const uint8* dat = cargs->dst.Bits();

    /*
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
        */
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleLoadFromFile_MEM_Generic, FSimpleBufferJobArgs, FDiskIOCommandArgs, &InvokeLoadFromFile_MEM_Generic )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleSaveToFile_MEM_Generic, FSimpleBufferJobArgs, FDiskIOCommandArgs, &InvokeSaveToFile_MEM_Generic )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedLoadFromFileInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedSaveToFileInvocationSchedulerSelector )

ULIS_NAMESPACE_END

