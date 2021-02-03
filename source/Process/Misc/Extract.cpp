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
fpExtract QueryDispatchedExtractInvocation( eType iSrcType, eType iDstType ) {
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

ULIS_NAMESPACE_END

