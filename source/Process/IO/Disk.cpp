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

#if defined( ULIS_GCC ) || defined( ULIS_MINGW64 )
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

ULIS_NAMESPACE_BEGIN
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeLoadFromFile_MEM(
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
    uint8 cs = 0;
    switch( channels ) {
        case 1: model = CM_GREY;    hea = false;    cs = ULIS_sGrey;    break;
        case 2: model = CM_GREY;    hea = true;     cs = ULIS_sGrey;    break;
        case 3: model = CM_RGB;     hea = false;    cs = ULIS_sRGB;     break;
        case 4: model = CM_RGB;     hea = true;     cs = ULIS_sRGB;     break;
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
            | ULIS_W_PROFILE( cs )
        );

    cargs->dst.LoadFromData( data, width, height, fmt, nullptr, FOnInvalidBlock(), FOnCleanupData( &OnCleanup_FreeMemory ) );
}

//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------- MEM
void
InvokeSaveToFile_MEM(
      const FSimpleBufferJobArgs* jargs
    , const FDiskIOCommandArgs* cargs
)
{
    int w = cargs->dst.Width();
    int h = cargs->dst.Height();
    int c = cargs->dst.SamplesPerPixel();
    const uint8* dat = cargs->dst.Bits();

    switch( cargs->fileFormat ) {
        case FileFormat_png: stbi_write_png( cargs->path.c_str(), w, h, c, dat, 0               );  break; // stride: 0
        case FileFormat_bmp: stbi_write_bmp( cargs->path.c_str(), w, h, c, dat                  );  break;
        case FileFormat_tga: stbi_write_tga( cargs->path.c_str(), w, h, c, dat                  );  break;
        case FileFormat_jpg: stbi_write_jpg( cargs->path.c_str(), w, h, c, dat, cargs->quality  );  break; // Quality: 0 - 100;
        case FileFormat_hdr: stbi_write_hdr( cargs->path.c_str(), w, h, c, (float*)dat          );  break;
    }
}

/////////////////////////////////////////////////////
// Dispatch
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleLoadFromFile_MEM, FSimpleBufferJobArgs, FDiskIOCommandArgs, &InvokeLoadFromFile_MEM )
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_SIMPLE( ScheduleSaveToFile_MEM, FSimpleBufferJobArgs, FDiskIOCommandArgs, &InvokeSaveToFile_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedLoadFromFileInvocationSchedulerSelector )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedSaveToFileInvocationSchedulerSelector )

ULIS_NAMESPACE_END

