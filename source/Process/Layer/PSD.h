// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         PSD.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the PSD tools.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Layer/LayerTypedefs.h"
#include "Scheduling/Command.h"
#include <fstream>

ULIS_NAMESPACE_BEGIN
struct FPSDLayerInfo
{
    uint32 mLeft = 0, mTop = 0, mBottom = 0, mRight = 0;
    int16_t mID[32];
    uint16 mNumChannels;
    int32_t mChannelSize[32];
    uint32 mStartChannelPos[32];
    char mBlendModeKey[5];
    uint8 mOpacity;
    uint8 mFlags;
    uint32 mExtraSize;
    uint32 mExtraPosition;
    uint32 mExtraRead;

    uint32 mLayerMaskSize;
    uint32 mYMask;
    uint32 mXMask;
    uint32 mWMask;
    uint32 mHMask;
    uint8  mColorMask;
    uint8  mFlagsMask;

    uint32 mLayerBlendingSize;

    uint8 mNameSize;
    char mName[257];

    uint32 mSizeLayerImage = 0;
    uint8* mLayerImageDst = nullptr;
    uint16* mLayerImageDst16 = nullptr;
    uint32* mLayerImageDst32 = nullptr;

    uint32 mDividerType;
    eFormat mLayerFormat;
};

class FPSDOperations
{
public:
    // Construction / Destruction
    ~FPSDOperations();
    FPSDOperations( const std::string& iFilename, FLayerStack& iStack );

public:
    // Static utility functions
    static void GetContextFormatFromFile( const std::string& iFilename, bool *oFileExists, eFormat *oFormat );
    static void GetContextFormatFromBitDepthColorMode( uint16 bitDepth, uint16 ColorMode, eFormat *oFormat );

private:
    /** The header of the file, with basic info about the image and verification that it's indeed a psd file */
    bool ReadFileHeader();
    
    /** Used for indexed colored images (look up tables), mostly useless for our purposes for now */
    bool ReadFileColorMode();

    /** Image resources, stores paths of pen strokes and such, mostly useless for our purposes for now */
    bool ReadImageResources();

    bool ReadLayerAndMaskInfo();

    bool ReadLayerInfo();

    bool ReadMaskInfo();

    bool ReadLayers();

    bool ReadAdditionalLayerInfoSignature();

    bool ReadAdditionalLayerInfo( uint32 sectionEnd );

    //bool ReadImageData();

    bool ReadLayerStackData();
    bool ReadLayerStackData16();
    bool ReadLayerStackData32();

    bool SetLayerStackFormatAndSize();
    bool SetLayersFormat();

    void GenerateLayerStackFromLayerStackData();

    void CopyUncompressed(uint32* dst,uint32 length);
    void CopyUncompressed(uint16* dst,uint32 length);
    void CopyUncompressed(uint8* dst,uint32 length);

    void DecodeAndCopyRLE(uint32* dst,uint32 length);
    void DecodeAndCopyRLE(uint16* dst,uint32 length);
    void DecodeAndCopyRLE(uint8* dst,uint32 length);

    void UnpredictZip16(uint8* dst,uint32 length, uint32 numColumns, uint32 rowSize);
    void UnpredictZip32(uint8* src, uint8* dst, uint32 length,uint32 numColumns, uint32 numRows, uint32 rowSize);

    void PlanarByteConvertBitMapToBGRA8(uint8* src,uint8* dst,uint32 length);
    void PlanarByteConvertBitMapToG8(uint8* src, uint8* dst, uint32 length);


    void PlanarByteConvertOrdered(uint8* src,uint8* dst,uint32 length, uint8 numChannels, uint8 channelsOrder[]);
    void PlanarByteConvertOrdered(uint16* src,uint16* dst,uint32 length,uint8 numChannels,uint8 channelsOrder[]);

    void PlanarByteConvert( uint8* src, uint8* dst, uint32 length, uint8 numChannels );
    void PlanarByteConvert(uint16* src,uint16* dst,uint32 length,uint8 numChannels);
    void PlanarByteConvert(uint32* src,uint32* dst,uint32 length,uint8 numChannels);

    void NegateImage( uint8* ioSrc, uint32 length );
    void NegateImage( uint16* ioSrc, uint32 length );
    void NegateImage( uint32* ioSrc,uint32 length );

    void lerp24BitsInto32Bits( uint32* ioSrc,uint32 length );


public:
    eBlendMode GetBlendingModeFromPSD(char iBlendModeKey[5]);

    //-- Getters / Setters
    uint16 GetChannelsNumber();
    uint32 GetImageHeight();
    uint32 GetImageWidth();
    uint16 GetBitDepth();
    uint16 GetColorMode();

    uint8* GetImageDst();
    uint16* GetImageDst16();

    TArray<FPSDLayerInfo>& GetLayersInfo();

    //FLayerStack* GetLayerStack();

    //-- Import / Export
    /** Tries to import the file passed in the constructor, return true if succeeded */
    bool Import();

private:
    std::ifstream mFileHandle;

    uint16 mChannelsNumber;
    uint32 mImageHeight;
    uint32 mImageWidth;
    uint16 mBitDepth;
    uint16 mColorMode;

    uint32 mImageStart;

    FLayerStack& mLayerStack; //Not owned, got from outside

    uint8* mImageDst;
    uint16* mImageDst16;
    uint32* mImageDst32;

    TArray<FPSDLayerInfo> mLayersInfo;
};

ULIS_NAMESPACE_END

