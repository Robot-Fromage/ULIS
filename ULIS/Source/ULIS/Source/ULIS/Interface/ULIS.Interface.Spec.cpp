// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.Spec.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definitions for the FSpecContext class.
 */
#include "ULIS/Interface/ULIS.Interface.Spec.h"
#include "ULIS/Interface/ULIS.Interface.Decl.h"
#include <iostream>

namespace ULIS {
/////////////////////////////////////////////////////
// FSpecContext
//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------- Public Static API
//static
FSpec
FSpecContext::BlockSpecAtIndex( uint32 iIndex )
{
    switch( iIndex )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data ) case n: return  ::ULIS::TBlockInfo< ULIS_REG[ n ] >:: data;
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, _nf )
        #undef ULIS_REG_SWITCH_OP
        default: return  ::ULIS::FSpec{};
    }
}


//static
FSpec
FSpecContext::BlockSpecFromHash( uint32 iHash )
{
    switch( iHash )
    {
        #define ULIS_REG_SWITCH_OP( z, n, data ) case ULIS_REG[ n ]: return  ::ULIS::TBlockInfo< ULIS_REG[ n ] >:: data;
        ULIS_REPEAT( ULIS_REG_SIZE, ULIS_REG_SWITCH_OP, _nf )
        #undef ULIS_REG_SWITCH_OP
        default: return  ::ULIS::FSpec{};
    }
}


//static
void
FSpecContext::PrintSpecs()
{
    for( int i = 0; i < ULIS_REG_SIZE; ++i )
    {
        auto spec = BlockSpecAtIndex( i );
        std::cout << "======================================"                   << std::endl;
        std::cout << "spec-str          :   " << spec._ss                       << std::endl;
        std::cout << "spec-hash         :   " << (uint32_t)spec._sh             << std::endl;
        std::cout << "type              :   " << ::ULIS::kw_tp[ (int)spec._tp ] << std::endl;
        std::cout << "color_model       :   " << ::ULIS::kw_cm[ (int)spec._cm ] << std::endl;
        std::cout << "extra_alpha       :   " << ::ULIS::kw_ea[ (int)spec._ea ] << std::endl;
        std::cout << "channel_layout    :   " << spec._cl                       << std::endl;
        std::cout << "normal_mode       :   " << ::ULIS::kw_nm[ (int)spec._nm ] << std::endl;
        std::cout << "decimal           :   " << spec._dm                       << std::endl;
        std::cout << "real_channels     :   " << (int)spec._rc                  << std::endl;
        std::cout << "num_channels      :   " << (int)spec._nc                  << std::endl;
        std::cout << "pixel_depth       :   " << (int)spec._pd                  << std::endl;
        std::cout << "type_max          :   " << (double)spec._tm               << std::endl;
        std::cout << "======================================"                   << std::endl;
    }
}


//static
void
FSpecContext::PrintShortSpecs()
{
    for( int i = 0; i < ULIS_REG_SIZE; ++i )
    {
        auto spec = BlockSpecAtIndex( i );
        std::cout << (uint32_t)spec._sh << "    " << spec._ss << std::endl;
    }
}

} // namespace ULIS

