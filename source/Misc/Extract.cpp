// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/*
*
*   ULIS3
*__________________
*
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Extract entry point functions.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Misc/Extract.h"
#include "Base/HostDeviceInfo.h"
#include "Data/Block.h"
#include "Maths/Geometry.h"
#include "Maths/Maths.h"
#include "Thread/ThreadPool.h"
#include <vector>

ULIS3_NAMESPACE_BEGIN
template< typename T1, typename T2 >
void InvokeExtractInto( size_t iW, const tByte* iSrc, tByte* iDst, std::vector< uint8 > iStridesSrc, std::vector< uint8 > iStridesDst, uint8 iSRCSPP, uint8 iDSTSPP ) {
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

typedef void (*fpDispatchedExtractInvoke)( size_t iW, const tByte* iSrc, tByte* iDst, std::vector< uint8 > iStridesSrc, std::vector< uint8 > iStridesDst, uint8 iSRCSPP, uint8 iDSTSPP );
fpDispatchedExtractInvoke QueryDispatchedExtractInvokeForParameters( eType iSrcType, eType iDstType );

fpDispatchedExtractInvoke QueryDispatchedExtractInvokeForParameters( eType iSrcType, eType iDstType ) {
    switch( iSrcType ) {
        case TYPE_UINT8: switch( iDstType ) {
                case TYPE_UINT8:    return  InvokeExtractInto< uint8, uint8        >;
                case TYPE_UINT16:   return  InvokeExtractInto< uint8, uint16       >;
                case TYPE_UINT32:   return  InvokeExtractInto< uint8, uint32       >;
                case TYPE_UFLOAT:   return  InvokeExtractInto< uint8, ufloat       >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< uint8, udouble      >; }
        case TYPE_UINT16: switch( iDstType ) {
                case TYPE_UINT8:    return  InvokeExtractInto< uint16, uint8       >;
                case TYPE_UINT16:   return  InvokeExtractInto< uint16, uint16      >;
                case TYPE_UINT32:   return  InvokeExtractInto< uint16, uint32      >;
                case TYPE_UFLOAT:   return  InvokeExtractInto< uint16, ufloat      >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< uint16, udouble     >; }
        case TYPE_UINT32: switch( iDstType ) {
                case TYPE_UINT8:    return  InvokeExtractInto< uint32, uint8       >;
                case TYPE_UINT16:   return  InvokeExtractInto< uint32, uint16      >;
                case TYPE_UINT32:   return  InvokeExtractInto< uint32, uint32      >;
                case TYPE_UFLOAT:   return  InvokeExtractInto< uint32, ufloat      >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< uint32, udouble     >; }
        case TYPE_UFLOAT: switch( iDstType ) {
                case TYPE_UINT8:    return  InvokeExtractInto< ufloat, uint8       >;
                case TYPE_UINT16:   return  InvokeExtractInto< ufloat, uint16      >;
                case TYPE_UINT32:   return  InvokeExtractInto< ufloat, uint32      >;
                case TYPE_UFLOAT:   return  InvokeExtractInto< ufloat, ufloat      >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< ufloat, udouble     >; }
        case TYPE_UDOUBLE: switch( iDstType ) {
                case TYPE_UINT8:    return  InvokeExtractInto< udouble, uint8      >;
                case TYPE_UINT16:   return  InvokeExtractInto< udouble, uint16     >;
                case TYPE_UINT32:   return  InvokeExtractInto< udouble, uint32     >;
                case TYPE_UFLOAT:   return  InvokeExtractInto< udouble, ufloat     >;
                case TYPE_UDOUBLE:  return  InvokeExtractInto< udouble, udouble    >; }
    }
    return  nullptr;
}


void
Extract( FThreadPool*           iThreadPool
       , bool                   iBlocking
       , uint32                 iPerfIntent
       , const FHostDeviceInfo& iHostDeviceInfo
       , bool                   iCallCB
       , const FBlock*          iSource
       , bool                   iSourceRawIndicesFlag
       , uint8                  iSourceExtractMask
       , FBlock*                iDestination
       , bool                   iDestinationRawIndicesFlag
       , uint8                  iDestinationExtractMask )
{
    // Assertions
    ULIS3_ASSERT( iThreadPool,                                  "Bad pool."                                             );
    ULIS3_ASSERT( iSource,                                      "Bad source."                                           );
    ULIS3_ASSERT( iDestination,                                 "Bad destination."                                      );
    ULIS3_ASSERT( iSource != iDestination,                      "Cannot extract a block to itself, use swap instead."   );
    ULIS3_ASSERT( !iCallCB || iBlocking,                        "Callback flag is specified on non-blocking operation." );
    ULIS3_ASSERT( iSourceExtractMask,                          "Empty extract mask provided."                           );
    ULIS3_ASSERT( iDestinationExtractMask,                     "Empty extract mask provided."                           );
    ULIS3_ASSERT( iSource->Width()  == iDestination->Width(),   "Blocks sizes don't match"                              );
    ULIS3_ASSERT( iSource->Height() == iDestination->Height(),  "Blocks sizes don't match"                              );

    // Check no-op
    if( !iSourceExtractMask || !iDestinationExtractMask )
        return;

    // Format info
    const FFormatInfo& srcFormatInfo( iSource->FormatInfo() );
    const FFormatInfo& dstFormatInfo( iDestination->FormatInfo() );

    // Channels
    std::vector< uint8 > sourceChannelsToExtract;
    std::vector< uint8 > destinationChannelsToExtract;
    uint8 max_channels_both = FMaths::Min( FMaths::Max( srcFormatInfo.SPP, dstFormatInfo.SPP ), static_cast< uint8 >( ULIS3_MAX_CHANNELS ) );
    sourceChannelsToExtract.reserve( max_channels_both );
    destinationChannelsToExtract.reserve( max_channels_both );
    for( int i = 0; i < max_channels_both; ++i ) {
        if( iSourceExtractMask & ( 1 << i ) )
            sourceChannelsToExtract.push_back( iSourceRawIndicesFlag ? i : srcFormatInfo.IDT[i] );

        if( iDestinationExtractMask & ( 1 << i ) )
            destinationChannelsToExtract.push_back( iDestinationRawIndicesFlag ? i : dstFormatInfo.IDT[i] );
    }

    ULIS3_ASSERT( sourceChannelsToExtract.size() == destinationChannelsToExtract.size(), "Extract masks don't map" );
    ULIS3_ASSERT( sourceChannelsToExtract.size() && destinationChannelsToExtract.size(), "Bad Extraction parameters" );

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
    const tByte*    srb = iSource->DataPtr();
    tByte*          dsb = iDestination->DataPtr();
    size_t          src_bps = iSource->BytesPerScanLine();
    size_t          dst_bps = iDestination->BytesPerScanLine();
    #define SRC srb + ( pLINE * src_bps )
    #define DST dsb + ( pLINE * dst_bps )
    const int       max = iSource->Height();
    const size_t    len = iSource->Width();
    fpDispatchedExtractInvoke fptr = QueryDispatchedExtractInvokeForParameters( srcFormatInfo.TP, dstFormatInfo.TP );
    ULIS3_ASSERT( fptr, "No dispatch invocation found." );
    ULIS3_MACRO_INLINE_PARALLEL_FOR( iPerfIntent, iThreadPool, iBlocking
                                   , max
                                   , fptr, len, SRC, DST, sourceStrides, destinationStrides, src_spp, dst_spp )

    iDestination->Invalidate( iCallCB );
}


FBlock* XExtract( FThreadPool*              iThreadPool
                , bool                      iBlocking
                , uint32                    iPerfIntent
                , const FHostDeviceInfo&    iHostDeviceInfo
                , bool                      iCallCB
                , const FBlock*             iSource
                , bool                      iSourceRawIndicesFlag
                , uint8                     iSourceExtractMask
                , tFormat                   iDestinationFormat
                , bool                      iDestinationRawIndicesFlag
                , uint8                     iDestinationExtractMask )
{
    // Assertions
    ULIS3_ASSERT( iSource, "Bad source." );
    FBlock* ret = new  FBlock( iSource->Width(), iSource->Height(), iDestinationFormat );
    Extract( iThreadPool, iBlocking, iPerfIntent, iHostDeviceInfo, iCallCB, iSource, iSourceRawIndicesFlag, iSourceExtractMask, ret, iDestinationRawIndicesFlag, iDestinationExtractMask );
    return  ret;
}

ULIS3_NAMESPACE_END

