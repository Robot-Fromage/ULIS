// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS.Interface.GlobalCPUConfig.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "ULIS/Global/ULIS.Global.GlobalCPUConfig.h"


namespace ULIS {
/////////////////////////////////////////////////////
// FGlobalThreadPool
// Public API
//static
FCPUConfig&
FGlobalCPUConfig::Get()
{
    static FCPUConfig* conf = NULL;
    if( !conf )
        conf = new FCPUConfig();

    return  *conf;
}

} // namespace ULIS

