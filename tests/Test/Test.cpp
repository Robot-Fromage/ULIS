// Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS2
*__________________
*
* @file         Test.cpp
* @author       Clement Berthaud
* @brief        Test application for ULIS2.
* @copyright    Copyright © 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS2>
using namespace ::ul2;

int
main() {
    FVec2F A[4] = { { 0, 0 }, { 200, 0 }, { 200, 200 }, { 0, 200 } };
    FVec2F B[4] = { { 10, 10 }, { 100, 50 }, { 100, 150 }, { 10, 190 } };
    auto mat = GetPerspectiveMatrix( A, B );
    auto inverse = glm::inverse( mat );

    FVec2F C[4];
    C[0] = HomographyTransform( A[0], mat );
    C[1] = HomographyTransform( A[1], mat );
    C[2] = HomographyTransform( A[2], mat );
    C[3] = HomographyTransform( A[3], mat );

    FVec2F D[4];
    D[0] = HomographyTransform( B[0], inverse );
    D[1] = HomographyTransform( B[1], inverse );
    D[2] = HomographyTransform( B[2], inverse );
    D[3] = HomographyTransform( B[3], inverse );
    auto dummy = 0;
    return  0;
}

