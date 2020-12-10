// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Extract entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Extract.h"
#include "System/Device.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include "Math/Geometry/Vector.h"
#include "Math/Math.h"
#include "Thread/ThreadPool.h"
#include <vector>

ULIS_NAMESPACE_BEGIN
template< typename T1, typename T2 >
void InvokeExtractInto( size_t iW, const uint8* iSrc, uint8* iDst, std::vector< uint8 > iStridesSrc, std::vector< uint8 > iStridesDst, uint8 iSRCSPP, uint8 iDSTSPP ) {
    const T1*   src = reinterpret_cast< const T1* >( iSrc );
    T2*         dst = reinterpret_cast< T2* >( iDst );
    const size_t len = iStridesSrc.size();
    for( size_t i = 0; i < iW; ++i ) {
        for( size_t j = 0; j < len; ++j )
            *( dst+ iStridesDst[j] ) = ConvType< T1, T2 >( *( src+ iStridesSrc[j] ) );
        src+= iSRCSPP;
        dst+= iDSTSPP;
    }
}

typedef void (*fpDispatchedExtractInvoke)( size_t iW, const uint8* iSrc, uint8* iDst, std::vector< uint8 > iStridesSrc, std::vector< uint8 > iStridesDst, uint8 iSRCSPP, uint8 iDSTSPP );
fpDispatchedExtractInvoke QueryDispatchedExtractInvokeForParameters( eType iSrcType, eType iDstType );

fpDispatchedExtractInvoke QueryDispatchedExtractInvokeForParameters( eType iSrcType, eType iDstType ) {
    switch( iSrcType ) {
        case Type_uint8: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< uint8, uint8        >;
                case Type_uint16:   return  InvokeExtractInto< uint8, uint16       >;
                case Type_uint32:   return  InvokeExtractInto< uint8, uint32       >;
                case Type_ufloat:   return  InvokeExtractInto< uint8, ufloat       >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< uint8, udouble      >; }
        case Type_uint16: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< uint16, uint8       >;
                case Type_uint16:   return  InvokeExtractInto< uint16, uint16      >;
                case Type_uint32:   return  InvokeExtractInto< uint16, uint32      >;
                case Type_ufloat:   return  InvokeExtractInto< uint16, ufloat      >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< uint16, udouble     >; }
        case Type_uint32: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< uint32, uint8       >;
                case Type_uint16:   return  InvokeExtractInto< uint32, uint16      >;
                case Type_uint32:   return  InvokeExtractInto< uint32, uint32      >;
                case Type_ufloat:   return  InvokeExtractInto< uint32, ufloat      >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< uint32, udouble     >; }
        case Type_ufloat: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< ufloat, uint8       >;
                case Type_uint16:   return  InvokeExtractInto< ufloat, uint16      >;
                case Type_uint32:   return  InvokeExtractInto< ufloat, uint32      >;
                case Type_ufloat:   return  InvokeExtractInto< ufloat, ufloat      >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< ufloat, udouble     >; }
        case TYPE_UDOUBLE: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< udouble, uint8      >;
                case Type_uint16:   return  InvokeExtractInto< udouble, uint16     >;
                case Type_uint32:   return  InvokeExtractInto< udouble, uint32     >;
                case Type_ufloat:   return  InvokeExtractInto< udouble, ufloat     >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< udouble, udouble    >; }
    }
    return  nullptr;
}


void
Extract( FOldThreadPool*           iOldThreadPool
       , bool                   iBlocking
       , uint32                 iPerfIntent
       , const FHardwareMetrics& iHostDeviceInfo
       , bool                   iCallCB
       , const FBlock*          iSource
       , bool                   iSourceRawIndicesFlag
       , uint8                  iSourceExtractMask
       , FBlock*                iDestination
       , bool                   iDestinationRawIndicesFlag
       , uint8                  iDestinationExtractMask )
{
    // Assertions
    ULIS_ASSERT( iOldThreadPool,                                  "Bad pool."                                             );
    ULIS_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS_ASSERT( iSource != iDestination,                      "Cannot extract a block to itself, use swap instead."   );
    ULIS_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS_ASSERT( iSourceExtractMask,                          "Empty extract mask provided."                           );
    ULIS_ASSERT( iDestinationExtractMask,                     "Empty extract mask provided."                           );
    ULIS_ASSERT( iSource->Width()  == iDestination->Width(),   "Blocks sizes don't match"                              );
    ULIS_ASSERT( iSource->Height() == iDestination->Height(),  "Blocks sizes don't match"                              );

    // Check no-op
    if( !iSourceExtractMask || !iDestinationExtractMask )
        return;

    // Format info
    const FFormatMetrics& srcFormatMetrics( iSource->FormatMetrics() );
    const FFormatMetrics& dstFormatMetrics( iDestination->FormatMetrics() );

    // Channels
    std::vector< uint8 > sourceChannelsToExtract;
    std::vector< uint8 > destinationChannelsToExtract;
    uint8 max_channels_both = FMath::Min( FMath::Max( srcFormatMetrics.SPP, dstFormatMetrics.SPP ), static_cast< uint8 >( ULIS_MAX_CHANNELS ) );
    sourceChannelsToExtract.reserve( max_channels_both );
    destinationChannelsToExtract.reserve( max_channels_both );
    for( int i = 0; i < max_channels_both; ++i ) {
        if( iSourceExtractMask & ( 1 << i ) )
            sourceChannelsToExtract.push_back( iSourceRawIndicesFlag ? i : srcFormatMetrics.IDT[i] );

        if( iDestinationExtractMask & ( 1 << i ) )
            destinationChannelsToExtract.push_back( iDestinationRawIndicesFlag ? i : dstFormatMetrics.IDT[i] );
    }

    ULIS_ASSERT( sourceChannelsToExtract.size() == destinationChannelsToExtract.size(), "Extract masks don't map" );
    ULIS_ASSERT( sourceChannelsToExtract.size() && destinationChannelsToExtract.size(), "Bad Extraction parameters" );

    // Strides
    std::vector< uint8 >    sourceStrides;
    std::vector< uint8 >    destinationStrides;
    sourceStrides.reserve(      sourceChannelsToExtract.size()      );
    destinationStrides.reserve( destinationChannelsToExtract.size() );
    sourceStrides.push_back(        sourceChannelsToExtract[0] );
    destinationStrides.push_back(   destinationChannelsToExtract[0] );
    for( int i = 1; i < sourceChannelsToExtract.size(); ++i ) {
        sourceStrides.push_back(        ( sourceChannelsToExtract[i]        - sourceChannelsToExtract[i-1]      ) );
        destinationStrides.push_back(   ( destinationChannelsToExtract[i]   - destinationChannelsToExtract[i-1] ) );
    }

    // Bake Params
    uint8           src_spp = iSource->SamplesPerPixel();
    uint8           dst_spp = iDestination->SamplesPerPixel();
    const uint8*    srb = iSource->Bits();
    uint8*          dsb = iDestination->Bits();
    size_t          src_bps = iSource->BytesPerScanLine();
    size_t          dst_bps = iDestination->BytesPerScanLine();
    #define SRC srb + ( pLINE * src_bps )
    #define DST dsb + ( pLINE * dst_bps )
    const int       max = iSource->Height();
    const size_t    len = iSource->Width();
    fpDispatchedExtractInvoke fptr = QueryDispatchedExtractInvokeForParameters( srcFormatMetrics.TP, dstFormatMetrics.TP );
    ULIS_ASSERT( fptr, "No dispatch invocation found." );
    ULIS_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iOldThreadPool, iBlocking
                                   , max
                                   , fptr, len, SRC, DST, sourceStrides, destinationStrides, src_spp, dst_spp )

    iDestination->Dirty( iCallCB );
}


FBlock* XExtract( FOldThreadPool*              iOldThreadPool
                , bool                      iBlocking
                , uint32                    iPerfIntent
                , const FHardwareMetrics&    iHostDeviceInfo
                , bool                      iCallCB
                , const FBlock*             iSource
                , bool                      iSourceRawIndicesFlag
                , uint8                     iSourceExtractMask
                , eFormat                   iDestinationFormat
                , bool                      iDestinationRawIndicesFlag
                , uint8                     iDestinationExtractMask )
{
    // Assertions
    ULIS_ASSERT( iSource, "Bad source." );
    FBlock* ret = new  FBlock( iSource->Width(), iSource->Height(), iDestinationFormat );
    Extract( iOldThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, iSourceRawIndicesFlag, iSourceExtractMask, ret, iDestinationRawIndicesFlag, iDestinationExtractMask );
    return  ret;
}

ULIS_NAMESPACE_END

