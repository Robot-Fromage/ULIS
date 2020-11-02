// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*
*   ULIS
*__________________
*
* @file         Disk.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the disk IO entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "IO/Disk.h"
#include "System/Device.h"
#include "String/Utils.h"
#include "Image/Block.h"
#include "Conv/Conv.h"

// STD
#include <fstream>

// STB
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// LCMS
#include "lcms2.h"

// CPPFS
#include <cppfs/fs.h>
#include <cppfs/FileHandle.h>
#include <cppfs/FilePath.h>

ULIS_NAMESPACE_BEGIN
FBlock*
XLoadFromFile( FOldThreadPool*             iOldThreadPool
             , bool                     iBlocking
             , uint32                   iPerfIntent
             , const FHostDeviceInfo&   iHostDeviceInfo
             , bool                     iCallCB
             , const std::string&       iPath
             , eFormat                  iDesiredFormat )
{
    // Assertions
    ULIS_ASSERT( iOldThreadPool,              "Bad pool."                                             );
    ULIS_ASSERT( !iCallCB || iBlocking,    "Callback flag is specified on non-blocking operation." );

    cppfs::FileHandle   fh = cppfs::fs::open( iPath );

    if( ( !fh.exists() ) || ( !fh.isFile() ) )
        return  nullptr;

    std::ifstream file( iPath.c_str(), std::ios::binary | std::ios::ate );
    std::streamsize size = file.tellg();
    file.seekg( 0, std::ios::beg );
    std::vector<char> buffer(size);
    if( !file.read( buffer.data(), size ) )
        return  nullptr;

    eType type = TYPE_UINT8;
    type = stbi_is_16_bit_from_memory( (const stbi_uc*)buffer.data(), static_cast< int >( size ) )  ? TYPE_UINT16 : type;
    type = stbi_is_hdr_from_memory( (const stbi_uc*)buffer.data(), static_cast< int >( size ) )     ? TYPE_UFLOAT : type;

    int desiredChannels = STBI_default;
    eColorModel desiredModel = static_cast< eColorModel >( ULIS_R_MODEL( iDesiredFormat ) );
    bool needgrey   = desiredModel == CM_GREY;
    bool needrgb    = desiredModel == CM_RGB;
    bool needalpha  = ULIS_R_ALPHA( iDesiredFormat );
    if( needgrey )  desiredChannels = needalpha ? 2 : 1;
    if( needrgb )   desiredChannels = needalpha ? 4 : 3;

    uint8* data = nullptr;
    int width, height, channels, depth;
    width = height = channels = depth = 1;
    bool floating;
    switch( type ) {
        case TYPE_UINT8:    data = (uint8*)stbi_load(       iPath.c_str(), &width, &height, &channels, desiredChannels ); depth = 1;    floating = false;   break;
        case TYPE_UINT16:   data = (uint8*)stbi_load_16(    iPath.c_str(), &width, &height, &channels, desiredChannels ); depth = 2;    floating = false;   break;
        case TYPE_UFLOAT:   data = (uint8*)stbi_loadf(      iPath.c_str(), &width, &height, &channels, desiredChannels ); depth = 4;    floating = true;    break;
    }

    ULIS_ASSERT( data, "Error bad input file" )

    if( desiredChannels != STBI_default )
        channels = desiredChannels;

    eColorModel model;
    bool hea;
    switch( channels ) {
        case 1: model = CM_GREY;    hea = false;    break;
        case 2: model = CM_GREY;    hea = true;     break;
        case 3: model = CM_RGB;     hea = false;    break;
        case 4: model = CM_RGB;     hea = true;     break;
    }
    channels = channels - hea;

    eFormat fmt = static_cast< eFormat >( ULIS_W_TYPE( type ) | ULIS_W_CHANNELS( channels ) | ULIS_W_MODEL( model ) | ULIS_W_ALPHA( hea ) | ULIS_W_DEPTH( depth ) | ULIS_W_FLOATING( floating ) );
    FBlock* ret = new FBlock( data, width, height, fmt, nullptr, FOnInvalid(), FOnCleanup( &OnCleanup_FreeMemory ) );

    if( iDesiredFormat != 0 && iDesiredFormat != fmt ) {
        FBlock* conv = XConv( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, ret, iDesiredFormat );
        delete ret;
        ret = conv;
    }

    return  ret;
}

void SaveToFile( FOldThreadPool*           iOldThreadPool
               , bool                   iBlocking
               , uint32                 iPerfIntent
               , const FHostDeviceInfo& iHostDeviceInfo
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
    bool type_valid     = ( iImageFormat != IM_HDR && type == TYPE_UINT8 ) || ( iImageFormat == IM_HDR && type == TYPE_UFLOAT && model == CM_RGB );

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

ULIS_NAMESPACE_END

