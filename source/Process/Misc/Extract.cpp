// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Extract.cpp
* @author       Clement Berthaud
* @brief        This file provides the definitions for the Extract API.
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
    , uint8 iNumExtract
    , uint32 iLen
)
{
    const T1*   src = reinterpret_cast< const T1* >( iSrc );
    T2*         dst = reinterpret_cast< T2* >( iDst );
    const size_t len = iNumExtract;
    for( size_t i = 0; i < iLen; ++i ) {
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
        , cargs->numExtract
        , static_cast< uint32 >( jargs->size / cargs->src.BytesPerPixel() )
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
                //DISABLED:UINT32case Type_uint32:   return  InvokeExtractInto< uint8, uint32 >;
                case Type_ufloat:   return  InvokeExtractInto< uint8, ufloat >;
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  InvokeExtractInto< uint8, udouble      >;
                default: return  nullptr;
        }
        case Type_uint16: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< uint16, uint8  >;
                case Type_uint16:   return  InvokeExtractInto< uint16, uint16 >;
                //DISABLED:UINT32case Type_uint32:   return  InvokeExtractInto< uint16, uint32 >;
                case Type_ufloat:   return  InvokeExtractInto< uint16, ufloat >;
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  InvokeExtractInto< uint16, udouble     >;
                default: return  nullptr;
        }
        //DISABLED:UINT32case Type_uint32: switch( iDstType ) {
        //DISABLED:UINT32        case Type_uint8:    return  InvokeExtractInto< uint32, uint8  >;
        //DISABLED:UINT32        case Type_uint16:   return  InvokeExtractInto< uint32, uint16 >;
        //DISABLED:UINT32        case Type_uint32:   return  InvokeExtractInto< uint32, uint32 >;
        //DISABLED:UINT32        case Type_ufloat:   return  InvokeExtractInto< uint32, ufloat >;
        //DISABLED:UINT32        //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  InvokeExtractInto< uint32, udouble     >;
        //DISABLED:UINT32}
        case Type_ufloat: switch( iDstType ) {
                case Type_uint8:    return  InvokeExtractInto< ufloat, uint8  >;
                case Type_uint16:   return  InvokeExtractInto< ufloat, uint16 >;
                //DISABLED:UINT32case Type_uint32:   return  InvokeExtractInto< ufloat, uint32 >;
                case Type_ufloat:   return  InvokeExtractInto< ufloat, ufloat >;
                //DISABLED:DOUBLEcase TYPE_UDOUBLE:  return  InvokeExtractInto< ufloat, udouble     >;
                default: return  nullptr;
        }
        //case TYPE_UDOUBLE: switch( iDstType ) {
        //        case Type_uint8:    return  InvokeExtractInto< udouble, uint8      >;
        //        case Type_uint16:   return  InvokeExtractInto< udouble, uint16     >;
        //        case Type_uint32:   return  InvokeExtractInto< udouble, uint32     >;
        //        case Type_ufloat:   return  InvokeExtractInto< udouble, ufloat     >;
        //        case TYPE_UDOUBLE:  return  InvokeExtractInto< udouble, udouble    >; }
        default: return  nullptr;
    }

    return  nullptr;
}

ULIS_NAMESPACE_END

