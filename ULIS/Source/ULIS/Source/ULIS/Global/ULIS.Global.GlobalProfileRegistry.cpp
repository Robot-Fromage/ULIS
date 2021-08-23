// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Global.GlobalProfileRegistry.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "ULIS/Global/ULIS.Global.GlobalProfileRegistry.h"


namespace ULIS {
/////////////////////////////////////////////////////
// FGlobalProfileRegistry
// Public API
//static
FProfileRegistry&
FGlobalProfileRegistry::Get()
{
    static FProfileRegistry* pro = NULL;
    if( !pro )
        pro = new FProfileRegistry();

    return  *pro;
}

} // namespace ULIS

