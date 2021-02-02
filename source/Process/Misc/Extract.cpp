// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Extract API.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Process/Misc/Extract.h"
#include "Image/Block.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Invocations
template< typename T1, typename T2 >
void
InvokeExtractInto(
      const uint8* iSrc
    , uint8* iDst
    , uint8* iStridesSrc
    , uint8* iStridesDst
    , uint8 iSRCSPP
    , uint8 iDSTSPP
    , uint32 iLen
)
{
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

void
InvokeExtract_MEM(
      const FDualBufferJobArgs* jargs
    , const FExtractCommandArgs* cargs
)
{
    cargs->invocation(
          jargs->src
        , jargs->dst
        , cargs->sourceExtractMask
        , cargs->destinationExtractMask
        , cargs->src.SamplesPerPixel()
        , cargs->dst.SamplesPerPixel()
        , static_cast< uint32 >( jargs->size / cargs->dst.BytesPerPixel() )
    );
}

/////////////////////////////////////////////////////
// Dispatch / Schedule
ULIS_DEFINE_COMMAND_SCHEDULER_FORWARD_DUAL( ScheduleExtract, FDualBufferJobArgs, FExtractCommandArgs, &InvokeExtract_MEM )
ULIS_DISPATCHER_NO_SPECIALIZATION_DEFINITION( FDispatchedExtractInvocationSchedulerSelector )

/////////////////////////////////////////////////////
// Extract Dispatcher
fpExtract QueryDispatchedExtractInvokeForParameters( eType iSrcType, eType iDstType ) {
    switch( iSrcType ) {
        case Type_uint8: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< uint8, uint8  >;
                case Type_uint16:   return  InvokeExtractInto< uint8, uint16 >;
                case Type_uint32:   return  InvokeExtractInto< uint8, uint32 >;
                case Type_ufloat:   return  InvokeExtractInto< uint8, ufloat >;
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  InvokeExtractInto< uint8, udouble      >;
        }
        case Type_uint16: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< uint16, uint8  >;
                case Type_uint16:   return  InvokeExtractInto< uint16, uint16 >;
                case Type_uint32:   return  InvokeExtractInto< uint16, uint32 >;
                case Type_ufloat:   return  InvokeExtractInto< uint16, ufloat >;
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  InvokeExtractInto< uint16, udouble     >;
        }
        case Type_uint32: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< uint32, uint8  >;
                case Type_uint16:   return  InvokeExtractInto< uint32, uint16 >;
                case Type_uint32:   return  InvokeExtractInto< uint32, uint32 >;
                case Type_ufloat:   return  InvokeExtractInto< uint32, ufloat >;
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  InvokeExtractInto< uint32, udouble     >;
        }
        case Type_ufloat: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< ufloat, uint8  >;
                case Type_uint16:   return  InvokeExtractInto< ufloat, uint16 >;
                case Type_uint32:   return  InvokeExtractInto< ufloat, uint32 >;
                case Type_ufloat:   return  InvokeExtractInto< ufloat, ufloat >;
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  InvokeExtractInto< ufloat, udouble     >;
        }
        //case TYPE_UDOUBLE: switch( iDstType ) {
        //        case Type_uint8:    return  InvokeExtractInto< udouble, uint8      >;
        //        case Type_uint16:   return  InvokeExtractInto< udouble, uint16     >;
        //        case Type_uint32:   return  InvokeExtractInto< udouble, uint32     >;
        //        case Type_ufloat:   return  InvokeExtractInto< udouble, ufloat     >;
        //        case TYPE_UDOUBLE:  return  InvokeExtractInto< udouble, udouble    >; }
    }

    return  nullptr;
}

/*
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
*/

ULIS_NAMESPACE_END

