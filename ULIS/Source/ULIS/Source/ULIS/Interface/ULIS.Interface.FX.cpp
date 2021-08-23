// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.FX.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definitions for the ULIS.Interface.FX.h class.
 */
#include "ULIS/Interface/ULIS.Interface.FX.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/FX/ULIS.FX.FXContext.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FFXContext
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Static API
//static
void
FFXContext::WhiteNoise( IBlock* iBlock, int iSeed, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                \
            case ULIS_REG[ n ]:                                                                                 \
            {                                                                                                   \
                return  TFXContext< ULIS_REG[ n ] >::WhiteNoise( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock       \
                                                               , iSeed, iPerformanceOptions );                  \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FFXContext::ValueNoise( IBlock* iBlock, float iFrequency, int iSeed, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                \
            case ULIS_REG[ n ]:                                                                                 \
            {                                                                                                   \
                return  TFXContext< ULIS_REG[ n ] >::ValueNoise( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock       \
                                                               , iFrequency, iSeed, iPerformanceOptions );      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}



//static
void
FFXContext::BrownianNoise( IBlock* iBlock
                         , float iFrequency
                         , float iFrequencyMult
                         , float iAmplitudeMult
                         , uint8 iNumLayers
                         , int iSeed
                         , const FPerformanceOptions& iPerformanceOptions
                         , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                \
            case ULIS_REG[ n ]:                                                                                 \
            {                                                                                                   \
                return  TFXContext< ULIS_REG[ n ] >::BrownianNoise( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock    \
                                                                  , iFrequency                                  \
                                                                  , iFrequencyMult                              \
                                                                  , iAmplitudeMult                              \
                                                                  , iNumLayers                                  \
                                                                  , iSeed                                       \
                                                                  , iPerformanceOptions );                      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FFXContext::VoronoiNoise( IBlock* iBlock, uint32 iCount, int iSeed, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                \
            case ULIS_REG[ n ]:                                                                                 \
            {                                                                                                   \
                return  TFXContext< ULIS_REG[ n ] >::VoronoiNoise( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock     \
                                                               , iCount, iSeed, iPerformanceOptions );          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FFXContext::Clouds( IBlock* iBlock, int iSeed, const FPerformanceOptions& iPerformanceOptions, bool iCallInvalidCB )
{
    BrownianNoise( iBlock, 0.02f, 2.f, 0.5f, 5, iSeed, iPerformanceOptions, iCallInvalidCB );
}


//static
void
FFXContext::Convolution( IBlock* iSrc, IBlock* iDst, const FKernel& iKernel, bool iConvolveAlpha, const FPerformanceOptions& iPerformanceOptions )
{
    assert( iSrc->Id() == iDst->Id() );
    switch( iSrc->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                    \
            case ULIS_REG[ n ]:                                                                                     \
            {                                                                                                       \
                TFXContext< ULIS_REG[ n ] >::Convolution( (::ULIS::TBlock< ULIS_REG[ n ] >*)iSrc                    \
                                                                , (::ULIS::TBlock< ULIS_REG[ n ] >*)iDst            \
                                                               , iKernel, iConvolveAlpha, iPerformanceOptions );    \
                break;                                                                                              \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


} // namespace ULIS

