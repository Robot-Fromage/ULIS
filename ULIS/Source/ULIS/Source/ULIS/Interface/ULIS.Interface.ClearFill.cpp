// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.ClearFill.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definitions for the FClearFillContext class.
 */

#include "ULIS/Interface/ULIS.Interface.ClearFill.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"
#include "ULIS/Data/ULIS.Data.Block.h"
#include "ULIS/ClearFill/ULIS.ClearFIll.ClearFillContext.h"

namespace ULIS {
/////////////////////////////////////////////////////
// FClearFillContext
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Static API
//static
void
FClearFillContext::Fill( IBlock* iBlock
                       , const CColor& iColor
                       , const FPerformanceOptions& iPerformanceOptions
                       , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                TClearFillContext< ULIS_REG[ n ] >::Fill( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                          \
                                                        , iColor, iPerformanceOptions, iCallInvalidCB );                                    \
                break;                                                                                                                      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FClearFillContext::FillPreserveAlpha( IBlock* iBlock
                                    , const CColor& iColor
                                    , const FPerformanceOptions& iPerformanceOptions
                                    , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                            \
            case ULIS_REG[ n ]:                                                                                                             \
            {                                                                                                                               \
                TClearFillContext< ULIS_REG[ n ] >::FillPreserveAlpha( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                             \
                                                        , iColor, iPerformanceOptions, iCallInvalidCB );                                    \
                break;                                                                                                                      \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FClearFillContext::FillRect( IBlock* iBlock
                           , const CColor& iColor
                           , const FRect& iRect
                           , const FPerformanceOptions& iPerformanceOptions
                           , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                        \
            case ULIS_REG[ n ]:                                                                                                         \
            {                                                                                                                           \
                TClearFillContext< ULIS_REG[ n ] >::FillRect( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                  \
                                                            , iColor, iRect, iPerformanceOptions, iCallInvalidCB );                     \
                break;                                                                                                                  \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FClearFillContext::Clear( IBlock* iBlock
                        , const FPerformanceOptions& iPerformanceOptions
                        , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                        \
            case ULIS_REG[ n ]:                                                                                                         \
            {                                                                                                                           \
                TClearFillContext< ULIS_REG[ n ] >::Clear( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                     \
                                                         , iPerformanceOptions, iCallInvalidCB );                                       \
                break;                                                                                                                  \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}


//static
void
FClearFillContext::ClearRect( IBlock* iBlock
                            , const FRect& iRect
                            , const FPerformanceOptions& iPerformanceOptions
                            , bool iCallInvalidCB )
{
    switch( iBlock->Id() )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data )                                                                                        \
            case ULIS_REG[ n ]:                                                                                                         \
            {                                                                                                                           \
                TClearFillContext< ULIS_REG[ n ] >::ClearRect( (::ULIS::TBlock< ULIS_REG[ n ] >*)iBlock                                 \
                                                             , iRect,  iPerformanceOptions, iCallInvalidCB );                           \
                break;                                                                                                                  \
            }
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, void )
        #undef ULIS_REG_SWITCH_OP
    }
}

} // namespace ULIS

