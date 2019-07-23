/*************************************************************************
*
*   ULIS
*__________________
*
* ULIS_Test.cpp
* Clement Berthaud - Layl
* Please refer to LICENSE.md
*/

#include <ULIS_CORE>
#include "ULIS/Base/FeatureDetector/cpu_x86.h"

int main()
{
    //::ULIS::PrintSpecs();
    FeatureDetector::cpu_x86 feature_info;
    feature_info.detect_host();
    feature_info.print();

    return 0;
}

