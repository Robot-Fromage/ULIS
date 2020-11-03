// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.Conv.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definitions for the FConversionContext class.
 */
#include "ULIS/Interface/ULIS.Interface.Conv.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/Conv/ULIS.Conv.ConversionContext.h"

namespace ULIS {
template< uint32 _SHSrc >
void  ConvTypeAndLayoutInto_Imp( const TBlock< _SHSrc >* iBlockSrc
                               , IBlock* iBlockDst
                               , const FPerformanceOptions& iPerformanceOptions )
{
    switch( iBlockDst->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                TConversionContext::ConvertTypeAndLayoutInto< _SHSrc, ULIS_REG[ n ] >(                                                      \
                                                                 iBlockSrc,                                                                 \
                                                                 (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlockDst,                               \
                                                                 iPerformanceOptions);                                                      \
                break;                                                                                                                      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


/////////////////////////////////////////////////////
// FConversionContext
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Static API
//static
void
FConversionContext::ConvTypeAndLayoutInto( const IBlock* iBlockSrc
                                         , IBlock* iBlockDst
                                         , const FPerformanceOptions& iPerformanceOptions )
{
    switch( iBlockSrc->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                        \
            case ULIS_REG[ n ]:                                                                                                         \
            {                                                                                                                           \
                ConvTypeAndLayoutInto_Imp< ULIS_REG[ n ] >( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlockSrc,                                \
                                                                          iBlockDst,                                                    \
                                                                          iPerformanceOptions);                                         \
                break;                                                                                                                  \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}

} // namespace ULIS

