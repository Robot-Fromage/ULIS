// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        ULIS_Features.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 */

#include "ULIS/Base/FeatureDetector/cpu_x86.h"

int main()
{
    FeatureDetector::cpu_x86 feature_info;
    feature_info.detect_host();
    feature_info.print();
    return 0;
}

