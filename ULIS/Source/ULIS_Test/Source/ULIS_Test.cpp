// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_Test.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include <ULIS_CORE>

int main()
{
    using namespace ::ULIS;
    IBlock* blockA = FMakeContext::MakeBlock( 500, 500, FBlockRGBA8::TypeId() );
    IBlock* blockB = FMakeContext::MakeBlock( 500, 500, FBlockRGBA8::TypeId() );
    FClearFillContext::Fill( blockA, CColor( 255, 0, 0 ) );
    FClearFillContext::Clear( blockB );
    FBlendingContext::Blend( blockA, blockB, eBlendingMode::kNormal );
    return 0;
}
