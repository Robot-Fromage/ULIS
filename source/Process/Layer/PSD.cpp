// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PSD.cpp
* @author       Clement Berthaud
* @brief        This file provides the implementation for the PSD tools.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Layer/PSD.h"
#include "Context/Context.h"
#include "Image/Block.h"
#include "Process/Conv/Conv.h"
#include "Math/Math.h"
#include "zlib.h"

#if ( defined( ULIS_GCC ) || defined( ULIS_MINGW ) ) && __GNUC__ < 8
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

ULIS_NAMESPACE_BEGIN
FPSDOperations::~FPSDOperations()
{
    mFileHandle.close();
    
    // The destruction of imageDst and layers are made with FOnCleanupData in XLoadPSDFromDisk(), since they can be used longer that the lifetime of FPSDOperations

    if( mImageDst )
        mImageDst = nullptr;

    if( mImageDst16 )
        mImageDst16 = nullptr;

    if(mImageDst32)
        mImageDst32 = nullptr;

    for( int i = 0; i < mLayersInfo.Size(); i++)
    {
        if( mLayersInfo[i].mLayerImageDst )
            mLayersInfo[i].mLayerImageDst = nullptr;

        if( mLayersInfo[i].mLayerImageDst16 )
            mLayersInfo[i].mLayerImageDst16 = nullptr;

        if(mLayersInfo[i].mLayerImageDst32)
            mLayersInfo[i].mLayerImageDst32 = nullptr;
    }
}

FPSDOperations::FPSDOperations(const std::string& iFilename, FLayerStack& iStack)
    : mFileHandle( iFilename.c_str(), std::ios::in | std::ios::binary )
    , mChannelsNumber( 0 )
    , mImageHeight( 0 )
    , mImageWidth( 0 )
    , mBitDepth( 0 )
    , mColorMode( 0 )
    , mImageStart( 0 )
    , mLayerStack( iStack )
    , mImageDst(nullptr)
    , mImageDst16( nullptr )
    , mImageDst32( nullptr )
    , mLayersInfo()
{
     mFileHandle.seekg(0,std::ios::beg);
}

//static 
void FPSDOperations::GetContextFormatFromFile( const std::string& iFilename, bool *oFileExists, eFormat *oFormat)
{
    std::ifstream fileHandle = std::ifstream( iFilename.c_str(), std::ios::in | std::ios::binary );
    
    if (!oFileExists || !oFormat)
        return;

    if( fileHandle.tellg() == -1 )
    {
        *oFileExists = false;
        return;
    }

    fileHandle.seekg( 22, std::ios::cur );

    uint16 bitDepth;
    uint16 colorMode;

    if (!fileHandle.read((char*)&bitDepth, 2))
    {
        *oFileExists = false;
        return;
    }
    FMath::ByteSwap(&bitDepth, 2);

    if (!fileHandle.read((char*)&colorMode, 2))
    {
        *oFileExists = false;
        return;
    }
    FMath::ByteSwap(&colorMode, 2);

    *oFileExists = true;

    GetContextFormatFromBitDepthColorMode( bitDepth, colorMode, oFormat );
}

void FPSDOperations::GetContextFormatFromBitDepthColorMode( uint16 bitDepth, uint16 colorMode, eFormat *oFormat )
{
    //Special case: bitmap
    if( colorMode == 0 && bitDepth == 1)
    {
        *oFormat = Format_G8;
        return;
    }

    if (bitDepth == 32)
    {
        switch (colorMode)
        {
            case 1: //GrayScale
                *oFormat = Format_AGF;
                break;
            case 3: //RGB
                *oFormat = Format_ARGBF;
                break;
            case 4: //CMYK
                *oFormat = Format_ACMYKF;
                break;
            case 9: //LAB
                *oFormat = Format_ALabF;
                break;
            default: //ERROR
                return;
        }
    }
    else if (bitDepth == 16)
    {
        switch (colorMode)
        {
            case 1: //GrayScale
                *oFormat = Format_AG16;
                break;
            case 3: //RGB
                *oFormat = Format_ARGB16;
                break;
            case 4: //CMYK
                *oFormat = Format_ACMYK16;
                break;
            case 9: //LAB
                *oFormat = Format_ALab16;
                break;
            default: //ERROR
                return;
        }
    }
    else if (bitDepth == 8)
    {
        switch (colorMode)
        {
            case 1: //GrayScale
                *oFormat = Format_AG8;
                break;
            case 3: //RGB
                *oFormat = Format_ARGB8;
                break;
            case 4: //CMYK
                *oFormat = Format_ACMYK8;
                break;
            case 9: //LAB
                *oFormat = Format_ALab8;
                break;
            default: //ERROR
                return;
        }
    }
    else
    {
        ULIS_ASSERT( false, "Error: we don't handle this bit depth, format unknown");
        return;
    }
}


bool FPSDOperations::ReadFileHeader()
{    
    mFileHandle.seekg(0,std::ios::beg);

    char psdValidator[5] = { 0 }; // 4 characters + null termination

    if(!mFileHandle.read(psdValidator,4))
        return false;

    ULIS_ASSERT( strcmp(psdValidator, "8BPS") == 0, "This is likely not a PSD file, import failed" )

    //---
    
    uint16 versionNumber;
    if(!mFileHandle.read((char*)&versionNumber,2))
        return false;

    FMath::ByteSwap(&versionNumber,2);
    ULIS_ASSERT( versionNumber == 1, "Unknown psd file version, import failed" )

    //---

    mFileHandle.seekg( 6, std::ios::cur ); // There are 6 zeroed bytes here, we don't care about them
    
    //---

    if(!mFileHandle.read((char*)&mChannelsNumber,2))
        return false;
    FMath::ByteSwap(&mChannelsNumber,2);

    if(!mFileHandle.read((char*)&mImageHeight,4))
        return false;
    FMath::ByteSwap(&mImageHeight,4);

    if(!mFileHandle.read((char*)&mImageWidth,4))
        return false;
    FMath::ByteSwap(&mImageWidth,4);

    if(!mFileHandle.read((char*)&mBitDepth,2))
        return false;
    FMath::ByteSwap(&mBitDepth,2);

    if(!mFileHandle.read((char*)&mColorMode,2))
        return false;
    FMath::ByteSwap(&mColorMode,2);
    
    return true;
}

bool FPSDOperations::ReadFileColorMode()
{
    uint32 colorModeSize;
    if(!mFileHandle.read((char*)&colorModeSize,4))
        return false;
    FMath::ByteSwap(&colorModeSize,4);
    mFileHandle.seekg( colorModeSize, std::ios::cur );

    return true;
}

bool FPSDOperations::ReadImageResources()
{
    uint32 imageResourcesSize;
    if(!mFileHandle.read((char*)&imageResourcesSize,4))
        return false;
    FMath::ByteSwap( &imageResourcesSize, 4 );

    mFileHandle.seekg( imageResourcesSize, std::ios::cur );
    return true;
}

bool FPSDOperations::ReadLayerAndMaskInfo()
{
    uint32 layerAndMaskInfoSize;
    if(!mFileHandle.read((char*)&layerAndMaskInfoSize,4))
        return false;
    FMath::ByteSwap(&layerAndMaskInfoSize,4);

    uint32 position = uint32(mFileHandle.tellg());

    bool isAnyLayer = true;
    if( layerAndMaskInfoSize > 0) 
    {
        isAnyLayer = ReadLayerInfo();

        if( isAnyLayer )
            isAnyLayer = ReadMaskInfo();

        ReadAdditionalLayerInfo(position + layerAndMaskInfoSize);
    }

    mFileHandle.seekg( layerAndMaskInfoSize, std::ios::cur );

    mImageStart = position + layerAndMaskInfoSize;
    return isAnyLayer;
}

bool FPSDOperations::ReadLayerInfo()
{
    uint32 layerInfoSize;
    if(!mFileHandle.read((char*)&layerInfoSize,4))
        return false;
    FMath::ByteSwap(&layerInfoSize,4);

    uint32 position = uint32(mFileHandle.tellg());

    ULIS_ASSERT( mBitDepth < 16 || layerInfoSize == 0, "Shouldn't have any info for 16+ bit depth, import failed" );

    if(layerInfoSize != 0)
        if (!ReadLayers())
            return false;

    mFileHandle.seekg( position + layerInfoSize );
    return true;
}

bool FPSDOperations::ReadMaskInfo()
{
    uint32 maskInfoSize;
    if(!mFileHandle.read((char*)&maskInfoSize,4))
        return false;
    FMath::ByteSwap(&maskInfoSize,4);
    
    mFileHandle.seekg( maskInfoSize, std::ios::cur );
    return true;
}

bool FPSDOperations::ReadLayers()
{
    int16 numLayers;
    if(!mFileHandle.read((char*)&numLayers, 2))
        return false;

    FMath::ByteSwap( &numLayers, 2);
    
    if( numLayers < 0 )
        numLayers = -numLayers;

    //UE_LOG(LogTemp,Display,TEXT("numLayers: %d"),numLayers);
    for (int currLayer = 0; currLayer < numLayers; currLayer++) 
    {
        mLayersInfo.PushBack( FPSDLayerInfo() );

        if (!mFileHandle.read((char*)&mLayersInfo[currLayer].mTop, 4))
            return false;
        FMath::ByteSwap(&mLayersInfo[currLayer].mTop,4);

        if (!mFileHandle.read((char*)&mLayersInfo[currLayer].mLeft, 4))
            return false;
        FMath::ByteSwap(&mLayersInfo[currLayer].mLeft,4);

        if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mBottom,4))
            return false;
        FMath::ByteSwap(&mLayersInfo[currLayer].mBottom,4);

        if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mRight,4))
            return false;
        FMath::ByteSwap(&mLayersInfo[currLayer].mRight,4);

        if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mNumChannels,2))
            return false;
        FMath::ByteSwap(&mLayersInfo[currLayer].mNumChannels,2);

        for(int currChannel = 0; currChannel < mLayersInfo[currLayer].mNumChannels; currChannel++) 
        {
            if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mID[currChannel],2))
                return false;
            FMath::ByteSwap(&mLayersInfo[currLayer].mID[currChannel],2);

            if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mChannelSize[currChannel],4))
                return false;
            FMath::ByteSwap(&mLayersInfo[currLayer].mChannelSize[currChannel],4);
        }

        char psdValidator[5] = {0}; // 4 characters + null termination
        if(!mFileHandle.read((char*)psdValidator,4))
            return false;

        ULIS_ASSERT( strcmp(psdValidator, "8BIM") == 0, "This is likely not a PSD file, import failed" )

        if(!mFileHandle.read((char*) mLayersInfo[currLayer].mBlendModeKey, 4))
            return false;

        if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mOpacity,1))
            return false;

        mFileHandle.seekg( 1, std::ios::cur ); //Clipping byte

        if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mFlags,1))
            return false;

        mFileHandle.seekg( 1, std::ios::cur ); //Filler byte

        if(!mFileHandle.read((char*) &mLayersInfo[currLayer].mExtraSize, 4 ))
            return false;
        FMath::ByteSwap( &mLayersInfo[currLayer].mExtraSize, 4);

        mLayersInfo[currLayer].mExtraPosition = uint32(mFileHandle.tellg());
        mLayersInfo[currLayer].mExtraRead = 0;

        if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mLayerMaskSize,4))
            return false;
        FMath::ByteSwap(&mLayersInfo[currLayer].mLayerMaskSize,4);

        uint32 position = uint32(mFileHandle.tellg());

        if(mLayersInfo[currLayer].mLayerMaskSize != 0) 
        {
            if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mYMask,4))
                return false;
            FMath::ByteSwap(&mLayersInfo[currLayer].mYMask,4);

            if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mXMask,4))
                return false;
            FMath::ByteSwap(&mLayersInfo[currLayer].mXMask,4);

            if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mWMask,4))
                return false;
            FMath::ByteSwap(&mLayersInfo[currLayer].mWMask,4);

            if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mHMask,4))
                return false;
            FMath::ByteSwap(&mLayersInfo[currLayer].mHMask,4);

            if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mColorMask,1))
                return false;

            if(!mFileHandle.read((char*)&mLayersInfo[currLayer].mFlagsMask,1))
                return false;
        }

        mFileHandle.seekg( position + mLayersInfo[currLayer].mLayerMaskSize );
        mLayersInfo[currLayer].mExtraRead += 4 + mLayersInfo[currLayer].mLayerMaskSize;

        if(!mFileHandle.read((char*) &mLayersInfo[currLayer].mLayerBlendingSize, 4 ))
            return false;
        FMath::ByteSwap( &mLayersInfo[currLayer].mLayerBlendingSize, 4 );

        mLayersInfo[currLayer].mExtraRead += 4 + mLayersInfo[currLayer].mLayerBlendingSize;
        mFileHandle.seekg( mLayersInfo[currLayer].mLayerBlendingSize, std::ios::cur);

        if(!mFileHandle.read((char*) &mLayersInfo[currLayer].mNameSize, 1 ))
            return false;

        mLayersInfo[currLayer].mName[256] = {0};
        if(!mFileHandle.read((char*) mLayersInfo[currLayer].mName, mLayersInfo[currLayer].mNameSize ))
            return false;

        mFileHandle.seekg( 3 - mLayersInfo[currLayer].mNameSize % 4, std::ios::cur ); //4 bytes increments for the name
        mLayersInfo[currLayer].mExtraRead  += mLayersInfo[currLayer].mNameSize + 4 - mLayersInfo[currLayer].mNameSize % 4;

        if(mLayersInfo[currLayer].mName[0] == 0)
            strcpy( mLayersInfo[currLayer].mName,"background" );

        position = uint32(mFileHandle.tellg());

        while( mLayersInfo[currLayer].mExtraRead < mLayersInfo[currLayer].mExtraSize ) 
        {
            if(!ReadAdditionalLayerInfoSignature() )
                break;

            char lsctKey[5] = {0}; // 4 characters + null termination
            if(!mFileHandle.read((char*)lsctKey,4))
                return false;

            uint32 len;
            if(!mFileHandle.read((char*) &len, 4))
                return false;
            FMath::ByteSwap( &len, 4);

            position = uint32(mFileHandle.tellg());
            mLayersInfo[currLayer].mExtraRead += 12 + len;

            if(strcmp(lsctKey,"lsct") == 0)
            {
                if(!mFileHandle.read((char*) &mLayersInfo[currLayer].mDividerType,4))
                    return false;
                FMath::ByteSwap(&mLayersInfo[currLayer].mDividerType,4);
            }
            mFileHandle.seekg( std::streampos(position + len) );
        }
        mFileHandle.seekg( std::streampos(mLayersInfo[currLayer].mExtraPosition + mLayersInfo[currLayer].mExtraSize) );
    }


    uint32 imgData = uint32(mFileHandle.tellg());
    for(int i = 0; i < numLayers; i++) 
    {
        for(int c = 0; c < mLayersInfo[i].mNumChannels; c++) 
        {
            mLayersInfo[i].mStartChannelPos[c] = imgData;
            imgData += mLayersInfo[i].mChannelSize[c];
        }
    }

    return true;
}

bool FPSDOperations::ReadAdditionalLayerInfoSignature()
{
    char psdValidator[5] = {0}; // 4 characters + null termination
    if(!mFileHandle.read((char*)psdValidator,4))
        return false;

    //ULIS_ASSERT( strcmp(psdValidator, "8BIM") == 0 || strcmp(psdValidator, "8B64") == 0, "No additionnal layer info Signature" );

    return true;
}

bool FPSDOperations::ReadAdditionalLayerInfo(uint32 sectionEnd)
{
    uint32 position = uint32(mFileHandle.tellg());

    ULIS_ASSERT( position <= sectionEnd, "Error while loading data: out of bounds" );

    while( position < sectionEnd ) 
    {
        if( !ReadAdditionalLayerInfoSignature() )
            return false;

        char key[5] ={0}; // 4 characters + null termination
        if(!mFileHandle.read((char*)key,4))
            return false;

        uint32 len;
        if(!mFileHandle.read((char*) &len, 4))
            return false;
        FMath::ByteSwap( &len, 4);

        position = uint32(mFileHandle.tellg());

        if(strcmp(key,"Lr16") == 0 || strcmp(key,"Lr32") == 0)
        {
            ReadLayers();
            break;
        } 
        else if( strcmp(key,"Mt32") == 0 )
        {
            mFileHandle.seekg( len, std::ios::cur);
            ReadAdditionalLayerInfo(sectionEnd);
        }
        else 
        {
            mFileHandle.seekg( len, std::ios::cur);
            position += len + 4 + 4 + 4;
        }
    }
    return true;
}

/*bool FOdysseyPsdOperations::ReadImageData()
{
    if( mImageStart == 0 )
        return false;
    if( mImageDst != nullptr )
    {
        delete [] mImageDst;
        mImageDst = nullptr;
    }
    mFileHandle.seekg( mImageStart );
    uint16 compressionType;
    if(!mFileHandle.read((char*) &compressionType, 2 ));
    FMath::ByteSwap( &compressionType, 2 );
    int srcChannelsNumber = mChannelsNumber;
    if( mChannelsNumber > 4 )
        srcChannelsNumber = 4; //We're limiting the channels to 4 and try to translate it into RGBA
    if( mBitDepth > 8 )
    {
        uint32 size = mImageWidth * mImageHeight * srcChannelsNumber;
        mImageDst16 = new uint16[size];
        if( compressionType == 0 ) //Uncompressed
        {
            uint16* planarDst = new uint16[size];
            CopyUncompressed(planarDst, size);
            PlanarByteConvert(planarDst,mImageDst16,size,mChannelsNumber);
            delete [] planarDst;
        }
        else if(compressionType == 1) //RLE 16 Bits
        {
            uint16* planarDst = new uint16[size];
            mFileHandle.seekg( mFileHandle.tellg() + mImageHeight * srcChannelsNumber * 2 );
            DecodeAndCopyRLE(planarDst,size);
            PlanarByteConvert(planarDst,mImageDst16,size,mChannelsNumber);
            delete [] planarDst;
        }
        else
        {
            std::cout << "Compression type unknown, import failed" << std::endl;
            return false;
        }
    }
    else 
    {
        uint32 size =  mImageWidth * mImageHeight * srcChannelsNumber;
        mImageDst = new uint8[ size ];
        if( compressionType == 0 ) // uncompressed
        {
            uint8* planarDst = new uint8[size];
            CopyUncompressed( mImageDst, size );
            PlanarByteConvert(planarDst,mImageDst,size,mChannelsNumber);
            delete [] planarDst;
        } 
        else if(compressionType == 1) //RLE
        {
            uint8* planarDst = new uint8[size];
            mFileHandle.seekg( mFileHandle.tellg() + mImageHeight * mChannelsNumber * 2 );
            DecodeAndCopyRLE( planarDst, size );
            PlanarByteConvert( planarDst, mImageDst, size, mChannelsNumber );
            delete [] planarDst;
        } 
        else 
        {
            std::cout << "Compression type unknown, import failed" << std::endl;
            return false;
        }
    }
    return true;
}*/


bool FPSDOperations::ReadLayerStackData()
{
    for(uint8 i = 0; i < mLayersInfo.Size(); i++) 
    {
        uint8** channelContents = new uint8*[mLayersInfo[i].mNumChannels];

        int lx = mLayersInfo[i].mLeft;
        int ly = mLayersInfo[i].mTop;
        int lw = mLayersInfo[i].mRight;
        int lh = mLayersInfo[i].mBottom;
        uint32 channelSize =  (lw - lx) * (lh - ly);

        for(uint8 j = 0; j < mLayersInfo[i].mNumChannels; j++) 
        {
            //Future TODO: add masks gestion here when we'll have them

            mFileHandle.seekg( mLayersInfo[i].mStartChannelPos[j] );
            uint16 cp;
            if(!mFileHandle.read((char*) &cp, 2))
                return false;
            FMath::ByteSwap( &cp, 2 );

            channelContents[j] = new uint8[channelSize];

            if( cp == 0 ) //No compression
            {
                CopyUncompressed( channelContents[j], channelSize );
                mLayersInfo[i].mSizeLayerImage += channelSize;
            }
            else if ( cp == 1 ) //RLE
            {
                mFileHandle.seekg( (lh - ly) * 2, std::ios::cur);
                DecodeAndCopyRLE(channelContents[j],channelSize);
                mLayersInfo[i].mSizeLayerImage += channelSize;
            }
            else
            {
                ULIS_ASSERT( false, "Unknown or unsupported Compression, import failed" );
            }

            if(mColorMode == 4 && j != 0)//CMYK, we negate everything but the alpha
                NegateImage(channelContents[j],channelSize);
        }

        uint8* planarDst = new uint8[mLayersInfo[i].mSizeLayerImage];
        mLayersInfo[i].mLayerImageDst = new uint8[ mLayersInfo[i].mSizeLayerImage ];

        uint32 numBytesWritten = 0;
        for ( uint16 currChannel = 0; currChannel < mLayersInfo[i].mNumChannels; currChannel++ )
        {
            for( uint32 currWrite = 0; currWrite < channelSize; currWrite++)
            {
                planarDst[numBytesWritten] = channelContents[currChannel][currWrite];
                numBytesWritten++;
            }
        }

        PlanarByteConvert(planarDst,mLayersInfo[i].mLayerImageDst,mLayersInfo[i].mSizeLayerImage,uint8(mLayersInfo[i].mNumChannels));

        delete [] planarDst;

        for( ::ULIS::uint16 chan = 0; chan < mLayersInfo[i].mNumChannels; chan++)
            delete [] channelContents[chan];

        delete [] channelContents;
    }

    return true;
}

bool FPSDOperations::ReadLayerStackData16()
{
    for(uint8 i = 0; i < mLayersInfo.Size(); i++)
    {
        uint16** channelContents = new uint16*[mLayersInfo[i].mNumChannels];

        int ll = mLayersInfo[i].mLeft;
        int lt = mLayersInfo[i].mTop;
        int lr = mLayersInfo[i].mRight;
        int lb = mLayersInfo[i].mBottom;
        uint32 channelSize =  (lr - ll) * (lb - lt);

        for(uint8 j = 0; j < mLayersInfo[i].mNumChannels; j++)
        {
            //Future TODO: add masks gestion here when we'll have them

            mFileHandle.seekg(mLayersInfo[i].mStartChannelPos[j]);
            uint16 cp;
            if(!mFileHandle.read((char*)&cp,2))
                return false;
            FMath::ByteSwap(&cp,2);

            channelContents[j] = new uint16[channelSize];

            if(cp == 0) //No compression
            {
                CopyUncompressed(channelContents[j],channelSize);
                mLayersInfo[i].mSizeLayerImage += channelSize;
            } 
            else if(cp == 1) //RLE
            {
                mFileHandle.seekg((lb - lt) * 2, std::ios::cur);
                DecodeAndCopyRLE(channelContents[j],channelSize);
                mLayersInfo[i].mSizeLayerImage += channelSize;
            } 
            else if( (cp == 2 || cp == 3) )
            {
                uLongf dstSize = channelSize * sizeof( uint16 );
                uLongf srcSize = mLayersInfo[i].mChannelSize[j];
                char* srcData = new char[srcSize]{0};

                if(!mFileHandle.read( srcData, srcSize ))
                    return false;

                /*int zResult = */uncompress( (uint8*)channelContents[j], &dstSize, (uint8*)srcData, srcSize );

                if( cp == 3 )
                    UnpredictZip16( (uint8*)channelContents[j], channelSize * sizeof( uint16 ), lr - ll, (lr - ll) * sizeof( uint16) );

                for( uint32 currShort = 0; currShort < channelSize; currShort++ )
                {
                    FMath::ByteSwap( &channelContents[j][currShort], 2 );
                }

                mLayersInfo[i].mSizeLayerImage += channelSize;

                delete [] srcData;
            }
            else
            {
                ULIS_ASSERT( false, "Unknown or unsupported Compression, import failed" )
            }

            if(mColorMode == 4 && j != 0)//CMYK, we negate everything but the alpha
                NegateImage(channelContents[j],channelSize);
        }

        uint16* planarDst = new uint16[mLayersInfo[i].mSizeLayerImage];
        mLayersInfo[i].mLayerImageDst16 = new uint16[mLayersInfo[i].mSizeLayerImage];

        uint32 numShortWritten = 0;
        for(uint16 currChannel = 0; currChannel < mLayersInfo[i].mNumChannels; currChannel++)
        {
            for(uint32 currWrite = 0; currWrite < channelSize; currWrite++)
            {
                planarDst[numShortWritten] = channelContents[currChannel][currWrite];
                numShortWritten++;
            }
        }

        PlanarByteConvert(planarDst,mLayersInfo[i].mLayerImageDst16,mLayersInfo[i].mSizeLayerImage,uint8(mLayersInfo[i].mNumChannels));

        delete[] planarDst;

        for(int chan = 0; chan < mLayersInfo[i].mNumChannels; chan++)
            delete[] channelContents[chan];

        delete[] channelContents;
    }

    return true;
}

bool FPSDOperations::ReadLayerStackData32()
{
    for(uint8 i = 0; i < mLayersInfo.Size(); i++)
    {
        uint32** channelContents = new uint32*[mLayersInfo[i].mNumChannels];

        int ll = mLayersInfo[i].mLeft;
        int lt = mLayersInfo[i].mTop;
        int lr = mLayersInfo[i].mRight;
        int lb = mLayersInfo[i].mBottom;
        uint32 channelSize =  (lr - ll) * (lb - lt);

        for(uint8 j = 0; j < mLayersInfo[i].mNumChannels; j++)
        {
            //Future TODO: add masks gestion here when we'll have them

            mFileHandle.seekg(mLayersInfo[i].mStartChannelPos[j]);
            uint16 cp;
            if(!mFileHandle.read((char*)&cp,2))
                return false;
            FMath::ByteSwap(&cp,2);

            channelContents[j] = new uint32[channelSize];

            if(cp == 0) //No compression
            {
                CopyUncompressed(channelContents[j],channelSize);
                mLayersInfo[i].mSizeLayerImage += channelSize;
            } 
            else if(cp == 1) //RLE
            {
                mFileHandle.seekg((lb - lt) * 2, std::ios::cur);
                DecodeAndCopyRLE(channelContents[j],channelSize);
                mLayersInfo[i].mSizeLayerImage += channelSize;
            } 
            else if((cp == 2 || cp == 3))
            {
                uLongf dstSize = channelSize * sizeof(uint32);
                uLongf srcSize = mLayersInfo[i].mChannelSize[j];
                char* srcData = new char[srcSize];
                uint32* dstData = new uint32[channelSize];

                if(!mFileHandle.read(srcData,srcSize))
                    return false;

                /*int zResult = */uncompress((uint8*)dstData,&dstSize,(uint8*)srcData,srcSize);

                if(cp == 3)
                    UnpredictZip32((uint8*)dstData, (uint8*)channelContents[j], channelSize * sizeof(uint32),lr - ll, lb - lt, (lr - ll) * sizeof(uint32));

                for(uint32 currLong = 0; currLong < channelSize; currLong++)
                {
                    FMath::ByteSwap(&channelContents[j][currLong],4);
                }

                mLayersInfo[i].mSizeLayerImage += channelSize;

                delete[] srcData;
                delete[] dstData;
            } 
            else
            {
                ULIS_ASSERT( false, "Unknown or unsupported Compression, import failed" )
            }

            if(mColorMode == 4 && mLayersInfo[i].mID[j] != -1)//CMYK, we negate everything but the alpha
                NegateImage(channelContents[j],channelSize);
        }

        uint32* planarDst = new uint32[mLayersInfo[i].mSizeLayerImage];
        mLayersInfo[i].mLayerImageDst32 = new uint32[mLayersInfo[i].mSizeLayerImage];

        uint32 numLongWritten = 0;
        for(uint16 currChannel = 0; currChannel < mLayersInfo[i].mNumChannels; currChannel++)
        {
            for(uint32 currWrite = 0; currWrite < channelSize; currWrite++)
            {
                planarDst[numLongWritten] = channelContents[currChannel][currWrite];
                numLongWritten++;
            }
        }

        PlanarByteConvert(planarDst,mLayersInfo[i].mLayerImageDst32,mLayersInfo[i].mSizeLayerImage,uint8(mLayersInfo[i].mNumChannels));

        delete[] planarDst;

        for(int chan = 0; chan < mLayersInfo[i].mNumChannels; chan++)
            delete[] channelContents[chan];

        delete[] channelContents;
    }

    return true;
}



bool FPSDOperations::SetLayerStackFormatAndSize()
{
    eFormat formatStack = Format_RGBA8;
    GetContextFormatFromBitDepthColorMode(mBitDepth, mColorMode, &formatStack);

    if( mLayersInfo.Size() == 0 && formatStack == Format_RGBA8 ) //We have 0 layer in case of bitmap format, but GetContextFormatFromBitDepthColorMode is supposed to change formatStack to G8 in this case. So if it's not G8, then we have a problem
        return false;

    mLayerStack.Reset(mImageWidth, mImageHeight, formatStack);

    return true;
}


bool FPSDOperations::SetLayersFormat()
{
    //Special case: bitmap
    if( mColorMode == 0 && mBitDepth == 1 )
        return true;

    if( mLayersInfo.Size() == 0 )
        return false;

    for( int i = 0; i < mLayersInfo.Size(); i++ )
    {
        if (mBitDepth == 32)
        {
            switch (mColorMode)
            {
                case 1: //GrayScale
                {
                    if (mLayersInfo[i].mNumChannels == 2)
                        mLayersInfo[i].mLayerFormat = Format_AGF;
                    else
                        mLayersInfo[i].mLayerFormat = Format_GF;
                    break;
                }
                case 3: //RGB
                {
                    if (mLayersInfo[i].mNumChannels == 4)
                        mLayersInfo[i].mLayerFormat = Format_ARGBF;
                    else
                        mLayersInfo[i].mLayerFormat = Format_RGBF;
                    break;
                }
                case 4: //CMYK
                {
                    if (mLayersInfo[i].mNumChannels == 5)
                        mLayersInfo[i].mLayerFormat = Format_ACMYKF;
                    else
                        mLayersInfo[i].mLayerFormat = Format_CMYKF;
                    break;
                }
                case 9: //LAB
                {
                    if (mLayersInfo[i].mNumChannels == 4)
                        mLayersInfo[i].mLayerFormat = Format_ALabF;
                    else
                        mLayersInfo[i].mLayerFormat = Format_LabF;
                    break;
                }
                default: //ERROR
                    return false;
            }
        }
        else if (mBitDepth == 16)
        {
            switch (mColorMode)
            {
                case 1: //GrayScale
                {
                    if (mLayersInfo[i].mNumChannels == 2)
                        mLayersInfo[i].mLayerFormat = Format_AG16;
                    else
                        mLayersInfo[i].mLayerFormat = Format_G16;
                    break;
                }
                case 3: //RGB
                {
                    if (mLayersInfo[i].mNumChannels == 4)
                        mLayersInfo[i].mLayerFormat = Format_ARGB16;
                    else
                        mLayersInfo[i].mLayerFormat = Format_RGB16;
                    break;
                }
                case 4: //CMYK
                {
                    if (mLayersInfo[i].mNumChannels == 5)
                        mLayersInfo[i].mLayerFormat = Format_ACMYK16;
                    else
                        mLayersInfo[i].mLayerFormat = Format_CMYK16;
                    break;
                }
                case 9: //LAB
                {
                    if (mLayersInfo[i].mNumChannels == 4)
                        mLayersInfo[i].mLayerFormat = Format_ALab16;
                    else
                        mLayersInfo[i].mLayerFormat = Format_Lab16;
                    break;
                }
                default: //ERROR
                    return false;
            }
        }
        else if (mBitDepth == 8)
        {
            switch (mColorMode)
            {
                case 1: //GrayScale
                {
                    if (mLayersInfo[i].mNumChannels == 2)
                        mLayersInfo[i].mLayerFormat = Format_AG8;
                    else
                        mLayersInfo[i].mLayerFormat = Format_G8;
                    break;
                }
                case 3: //RGB
                {
                    if (mLayersInfo[i].mNumChannels == 4)
                        mLayersInfo[i].mLayerFormat = Format_ARGB8;
                    else
                        mLayersInfo[i].mLayerFormat = Format_RGB8;
                    break;
                }
                case 4: //CMYK
                {
                    if (mLayersInfo[i].mNumChannels == 5)
                        mLayersInfo[i].mLayerFormat = Format_ACMYK8;
                    else
                        mLayersInfo[i].mLayerFormat = Format_CMYK8;
                    break;
                }
                case 9: //LAB
                {
                    if (mLayersInfo[i].mNumChannels == 4)
                        mLayersInfo[i].mLayerFormat = Format_ALab8;
                    else
                        mLayersInfo[i].mLayerFormat = Format_Lab8;
                    break;
                }
                default: //ERROR
                    return false;
            }
        }
        else
        {
            ULIS_ASSERT( false, "Error: we don't handle this bit depth, import failed" )
        }
    }
    return true;
}

void FPSDOperations::GenerateLayerStackFromLayerStackData()
{
    //Special case: bitmap --------------------------------------
    if( mColorMode == 0  /*BitMap*/ ) //If we're dealing with bitmap, the data is at ImgDst, and not in the layers info
    {
        if( mImageStart == 0 )
            return;

        if(mImageDst != nullptr)
        {
            delete[] mImageDst;
            mImageDst = nullptr;
        }

        mFileHandle.seekg(mImageStart);

        uint16 compressionType;
        if(!mFileHandle.read((char*)&compressionType,2))
            return;
        FMath::ByteSwap(&compressionType,2);

        uint32 size =  mImageWidth * mImageHeight;
        mImageDst = new uint8[size];

        if(compressionType == 0) // uncompressed
        {
            uint32 sizeBitmap = ( mImageWidth * mImageHeight ) / 8 + 1;
            uint8* planar = new uint8[sizeBitmap];
            CopyUncompressed( planar, sizeBitmap );
            PlanarByteConvertBitMapToG8( planar, mImageDst, sizeBitmap );
            delete[] planar;
        } 
        else if(compressionType == 1) //RLE
        {
            uint32 sizeBitmap = (mImageWidth * mImageHeight) / 8 + 1;
            uint8* planar = new uint8[sizeBitmap];
            mFileHandle.seekg( mImageHeight * 2, std::ios::cur ); 
            DecodeAndCopyRLE( planar, sizeBitmap );
            PlanarByteConvertBitMapToG8( planar, mImageDst, sizeBitmap );
            delete[] planar;
        }
        FBlock* srcblock = new FBlock( mImageDst, mImageWidth, mImageHeight, eFormat::Format_G8);

        mLayerStack.AddChild(
            new FLayerImage(
                  srcblock
                , FString("Layer1")
                , false
                , true
                , FColor::Transparent
                , eBlendMode::Blend_Normal
                , eAlphaMode::Alpha_Normal
                , 1.f
                , false
                , &mLayerStack
            )
        );
    }
    //-----------------------------------------------------------

}

void FPSDOperations::CopyUncompressed(uint32* dst,uint32 length)
{
    for(uint32 i = 0; i < length; i++)
    {
        if(!mFileHandle.read((char*) &(dst[i]),4))
            return ;
        FMath::ByteSwap(&(dst[i]),4);
    }
}

void FPSDOperations::CopyUncompressed(uint16* dst, uint32 length)
{
    for( uint32 i = 0; i < length; i++ )
    {
        if(!mFileHandle.read((char*) &(dst[i]), 2 ))
            return;
        FMath::ByteSwap( &(dst[i]), 2 );
    }
}

void FPSDOperations::CopyUncompressed(uint8* dst, uint32 length)
{
    if(!mFileHandle.read((char*) dst, length))
        return;
}

void FPSDOperations::DecodeAndCopyRLE(uint32* dst,uint32 length)
{
    while(length > 0)
    {
        int32_t k;
        if(!mFileHandle.read((char*)&k,4))
            return;
        FMath::ByteSwap(&k,4);

        if(k >= 0)
        {
            uint32 n = k + 1;
            if(n > length)
                n = length;

            for(uint32 i = 0; i < n; i++)
            {
                if(!mFileHandle.read((char*)dst,n))
                    return;
                FMath::ByteSwap(&dst,4);
                dst++;
            }
            length -= n;
        } else
        {
            uint32 n = -k + 1;
            if(n > length)
                n = length;

            uint32 fileLong;
            if(!mFileHandle.read((char*)&fileLong,4))
                return;
            FMath::ByteSwap(&fileLong,4);

            for(uint32 i = 0; i < n; i++)
                *dst++ = fileLong;

            length -= n;
        }
    }
}

void FPSDOperations::DecodeAndCopyRLE(uint16* dst,uint32 length)
{
    while( length > 0 )
    {
        int16_t k;
        if(!mFileHandle.read((char*) &k, 2 ))
            return;
        FMath::ByteSwap( &k, 2 );

        if(k >= 0)
        {
            uint32 n = k + 1;
            if( n > length )
                n = length;

            for( uint32 i = 0; i < n; i++ )
            {
                if(!mFileHandle.read((char*)dst,n))
                    return;
                FMath::ByteSwap( &dst, 2 );
                dst++;
            }
            length -= n;
        }
        else
        {
            uint32 n = -k + 1;
            if(n > length)
                n = length;

            uint16 fileShort;
            if(!mFileHandle.read((char*)&fileShort,2))
                return;
            FMath::ByteSwap( &fileShort, 2 );

            for(uint32 i = 0; i < n; i++)
                *dst++ = fileShort; 

            length -= n;
        }
    }
}

void FPSDOperations::DecodeAndCopyRLE(uint8* dst, uint32 length)
{
    while( length > 0 )
    {
        char k;
        if(!mFileHandle.read((char*) &k, 1 ))
            return;

        if(k >= 0) 
        {
            uint32 n = k + 1;
            if(n > length)
                n = length;

            if(!mFileHandle.read((char*) dst, n ))
                return;

            dst += n;
            length -= n;
        } 
        else 
        {
            uint32 n = -k + 1;
            if(n > length)
                n = length;

            uint8 fileByte;
            if(!mFileHandle.read((char*) &fileByte, 1 ))
                return;

            memset(dst,fileByte,n);

            dst += n;
            length -= n;
        }
    }
}

void FPSDOperations::UnpredictZip16(uint8* dst,uint32 length, uint32 numColumns, uint32 rowSize)
{
    while (length > 0)
    {
        uint32 c = numColumns;
        while( --c )
        {
            dst[2]+=dst[0]+((dst[1]+dst[3]) >> 8);
            dst[3]+=dst[1];
            dst+=2;
        }
        dst+=2;
        length-=rowSize;
    }
}

void FPSDOperations::UnpredictZip32(uint8* src, uint8* dst, uint32 length,uint32 numColumns, uint32 numRows, uint32 rowSize)
{
    uint32 remaining;
    uint8* start;

    for(uint32 y = 0; y < numRows; y++)
    {
        start=src;
        remaining=rowSize;
        while(--remaining)
        {
            *(src+1)+=*src;
            src++;
        }

        src=start;
        remaining=numColumns;
        while(remaining--)
        {
            *(dst++)=*src;
            *(dst++)=*(src + numColumns);
            *(dst++)=*(src + numColumns * 2);
            *(dst++)=*(src + numColumns * 3);

            src++;
        }
        src=start+rowSize;
    }
}

void FPSDOperations::PlanarByteConvertBitMapToBGRA8(uint8* src,uint8* dst,uint32 length)
{
    unsigned int mask = 1U << 7;
    for( uint32 i = 0; i < length; i++ )
    {
        for(int j = 0; j < 8; j++)
        {
            dst[i*8*4 + j*4] = ((src[i] & mask) ? 0 : 1) * 255;
            dst[i*8*4 + j*4 + 1] = dst[i*8*4 + j*4];
            dst[i*8*4 + j*4 + 2] = dst[i*8*4 + j*4];
            dst[i*8*4 + j*4 + 3] = 255;

            src[i] <<= 1;
        }
    }
}

void FPSDOperations::PlanarByteConvertBitMapToG8(uint8* src, uint8* dst, uint32 length)
{
    unsigned int mask = 1U << 7;
    for( uint32 i = 0; i < length; i++ )
    {
        for(int j = 0; j < 8; j++)
        {
            dst[i*8 + j] = ((src[i] & mask) ? 0 : 1) * 255;
            src[i] <<= 1;
        }
    }
}

void FPSDOperations::PlanarByteConvertOrdered(uint8* src,uint8* dst,uint32 length,uint8 numChannels,uint8 channelsOrder[])
{
    uint32 maxByChannel = length / numChannels;
    for(uint32 i = 0; i < maxByChannel; i++)
    {
        for(uint16 j = 0; j < numChannels; j++)
        {
            dst[i * numChannels + channelsOrder[j]] = src[i + j * maxByChannel];
        }
    }
}

void FPSDOperations::PlanarByteConvertOrdered(uint16* src,uint16* dst,uint32 length,uint8 numChannels,uint8 channelsOrder[])
{
    uint32 maxByChannel = length / numChannels;
    for(uint32 i = 0; i < maxByChannel; i++)
    {
        for(uint16 j = 0; j < numChannels; j++)
        {
            dst[i * numChannels + channelsOrder[j]] = src[i + j * maxByChannel];
        }
    }
}

void FPSDOperations::PlanarByteConvert( uint8* src, uint8* dst, uint32 length, uint8 numChannels )
{
    uint32 maxByChannel = length / numChannels;
    for( uint32 i = 0; i < maxByChannel; i++ )
    {
        for (uint16 j = 0; j < numChannels; j++)
        {
            dst[i * numChannels + j] = src[i + j * maxByChannel];
        }
    }
}

void FPSDOperations::PlanarByteConvert(uint16* src,uint16* dst,uint32 length,uint8 numChannels)
{
    uint32 maxByChannel = length / numChannels;
    for(uint32 i = 0; i < maxByChannel; i++)
    {
        for(uint16 j = 0; j < numChannels; j++)
        {
            dst[i * numChannels + j] = src[i + j * maxByChannel];
        }
    }
}

void FPSDOperations::PlanarByteConvert(uint32* src,uint32* dst,uint32 length,uint8 numChannels)
{
    uint32 maxByChannel = length / numChannels;
    for(uint32 i = 0; i < maxByChannel; i++)
    {
        for(uint16 j = 0; j < numChannels; j++)
        {
            dst[i * numChannels + j] = src[i + j * maxByChannel];
        }
    }
}

void FPSDOperations::NegateImage(uint8* ioSrc,uint32 length )
{
    for( uint32 i = 0; i < length; i++ )
        ioSrc[i] = UINT8_MAX - ioSrc[i];
}

void FPSDOperations::NegateImage(uint16* ioSrc,uint32 length )
{
    for(uint32 i = 0; i < length; i++)
        ioSrc[i] = UINT16_MAX - ioSrc[i];
}

void FPSDOperations::NegateImage(uint32* ioSrc,uint32 length)
{
    for(uint32 i = 0; i < length; i++)
        ioSrc[i] = UINT32_MAX - ioSrc[i];
}

void FPSDOperations::lerp24BitsInto32Bits(uint32* ioSrc,uint32 length)
{
    for(uint32 i = 0; i < length; i++)
    {
        ioSrc[i] = ioSrc[i] << 8;
    }
}

eBlendMode FPSDOperations::GetBlendingModeFromPSD(char iBlendModeKey[5])
{
    if( strcmp( iBlendModeKey, "norm" ) == 0 ) { return eBlendMode::Blend_Normal; }
    if( strcmp( iBlendModeKey, "diss" ) == 0 ) { return eBlendMode::Blend_Dissolve; }
    if( strcmp( iBlendModeKey, "dark" ) == 0 ) { return eBlendMode::Blend_Darken; }
    if( strcmp( iBlendModeKey, "mul " ) == 0 ) { return eBlendMode::Blend_Multiply; }
    if( strcmp( iBlendModeKey, "idiv" ) == 0 ) { return eBlendMode::Blend_ColorBurn; }
    if( strcmp( iBlendModeKey, "lbrn" ) == 0 ) { return eBlendMode::Blend_LinearBurn; }
    if( strcmp( iBlendModeKey, "dkCl" ) == 0 ) { return eBlendMode::Blend_DarkerColor; }
    if( strcmp( iBlendModeKey, "lite" ) == 0 ) { return eBlendMode::Blend_Lighten; }
    if( strcmp( iBlendModeKey, "scrn" ) == 0 ) { return eBlendMode::Blend_Screen; }
    if( strcmp( iBlendModeKey, "div " ) == 0 ) { return eBlendMode::Blend_ColorDodge; }
    if( strcmp( iBlendModeKey, "lddg" ) == 0 ) { return eBlendMode::Blend_LinearDodge; }
    if( strcmp( iBlendModeKey, "lgCl" ) == 0 ) { return eBlendMode::Blend_LighterColor; }
    if( strcmp( iBlendModeKey, "over" ) == 0 ) { return eBlendMode::Blend_Overlay; }
    if( strcmp( iBlendModeKey, "sLit" ) == 0 ) { return eBlendMode::Blend_SoftLight; }
    if( strcmp( iBlendModeKey, "hLit" ) == 0 ) { return eBlendMode::Blend_HardLight; }
    if( strcmp( iBlendModeKey, "vLit" ) == 0 ) { return eBlendMode::Blend_VividLight; }
    if( strcmp( iBlendModeKey, "lLit" ) == 0 ) { return eBlendMode::Blend_LinearLight; }
    if( strcmp( iBlendModeKey, "pLit" ) == 0 ) { return eBlendMode::Blend_PinLight; }
    if( strcmp( iBlendModeKey, "hMix" ) == 0 ) { return eBlendMode::Blend_HardMix; }
    if( strcmp( iBlendModeKey, "diff" ) == 0 ) { return eBlendMode::Blend_Difference; }
    if( strcmp( iBlendModeKey, "smud" ) == 0 ) { return eBlendMode::Blend_Exclusion; }
    if( strcmp( iBlendModeKey, "fsub" ) == 0 ) { return eBlendMode::Blend_Substract; }
    if( strcmp( iBlendModeKey, "fdiv" ) == 0 ) { return eBlendMode::Blend_Divide; }
    if( strcmp( iBlendModeKey, "hue " ) == 0 ) { return eBlendMode::Blend_Hue; }
    if( strcmp( iBlendModeKey, "sat " ) == 0 ) { return eBlendMode::Blend_Saturation; }
    if( strcmp( iBlendModeKey, "colr" ) == 0 ) { return eBlendMode::Blend_Color; }
    if( strcmp( iBlendModeKey, "lum " ) == 0 ) { return eBlendMode::Blend_Luminosity; }

    //unknown blending mode, we return the normal one by default
    return  eBlendMode::Blend_Normal;
}

uint16 FPSDOperations::GetChannelsNumber()
{
    return mChannelsNumber;
}

uint32 FPSDOperations::GetImageHeight()
{
    return mImageHeight;
}

uint32 FPSDOperations::GetImageWidth()
{
    return mImageWidth;
}

uint16 FPSDOperations::GetBitDepth()
{
    return mBitDepth;
}

uint16 FPSDOperations::GetColorMode()
{
    return mColorMode;
}

uint8* FPSDOperations::GetImageDst()
{
    return mImageDst;
}

uint16* FPSDOperations::GetImageDst16()
{
    return mImageDst16;
}

TArray<FPSDLayerInfo>& FPSDOperations::GetLayersInfo()
{
    return mLayersInfo;
}

bool FPSDOperations::Import()
{
    if(!ReadFileHeader() )
        return false;

    if(!ReadFileColorMode())
        return false;

    if(!ReadImageResources())
        return false;

    if(!ReadLayerAndMaskInfo())
        return false;

    //We can get it faster by blending the whole layer stack
    /*if( !ReadImageData() )
        return false;*/

    if( mBitDepth == 32)
    {
        if(!ReadLayerStackData32())
            return false;
    }
    else if( mBitDepth == 16 )
    {
        if( !ReadLayerStackData16() )
            return false;
    }
    else if( mBitDepth == 8 )
    {
        if(!ReadLayerStackData())
            return false;
    }
    else if( mBitDepth != 1 )
    {
        ULIS_ASSERT( false, "Unsupported bit depth, Import failed" );
    }

    if ( !SetLayersFormat() )
        return false;

    if (!SetLayerStackFormatAndSize())
        return false;

    GenerateLayerStackFromLayerStackData();
    return true;
}

ULIS_NAMESPACE_END

