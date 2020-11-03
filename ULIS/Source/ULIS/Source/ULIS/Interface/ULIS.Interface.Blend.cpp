// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.Blend.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definitions for the FBlendingContext class.
 */
#include "ULIS/Interface/ULIS.Interface.Blend.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/Blend/ULIS.Blend.BlendingContext.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FBlendingContext
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Static API
//static
void
FBlendingContext::Blend( IBlock*                    iBlockTop
                       , IBlock*                    iBlockBack
                       , int                        iX
                       , int                        iY
                       , eBlendingMode              iBlendingMode
                       , eAlphaMode                 iAlphaMode
                       , float                      iOpacity
                       , const FPerformanceOptions& iPerformanceOptions
                       , bool                       iCallInvalidCB )
{
    assert( iBlockTop->Id() == iBlockBack->Id() );
    switch( iBlockTop->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                 \
            {                                                                                                                                   \
                TBlendingContext< ULIS_REG[ n ] >::Blend( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlockTop                                           \
                                                        , (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlockBack                                          \
                                                        , iX, iY, iBlendingMode, iAlphaMode, iOpacity, iPerformanceOptions, iCallInvalidCB );   \
                break;                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FBlendingContext::Blend( IBlock*                    iBlockTop
                       , IBlock*                    iBlockBack
                       , const FRect&               iArea
                       , eBlendingMode              iBlendingMode
                       , eAlphaMode                 iAlphaMode
                       , float                      iOpacity
                       , const FPerformanceOptions& iPerformanceOptions
                       , bool                       iCallInvalidCB )
{
    assert( iBlockTop->Id() == iBlockBack->Id() );
    switch( iBlockTop->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                                \
            case ULIS_REG[ n ]:                                                                                                                 \
            {                                                                                                                                   \
                TBlendingContext< ULIS_REG[ n ] >::Blend( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlockTop                                           \
                                                        , (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlockBack                                          \
                                                        , iArea, iBlendingMode, iAlphaMode, iOpacity, iPerformanceOptions, iCallInvalidCB );    \
                break;                                                                                                                          \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}

} // namespace ULIS

