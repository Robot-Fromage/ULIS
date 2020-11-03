// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.GlobalThreadPool.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "ULIS/Global/ULIS.Global.GlobalThreadPool.h"


namespace ULIS {
/////////////////////////////////////////////////////
// FGlobalThreadPool
// Public API
//static
FThreadPool&
FGlobalThreadPool::Get()
{
    static FThreadPool* pool = NULL;
    if( !pool )
        pool = new FThreadPool();

    return  *pool;
}

} // namespace ULIS

