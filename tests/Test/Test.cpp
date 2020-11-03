// Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000
/**
*
*   ULIS3
*__________________
*
* @file         Test.cpp
* @author       Clement Berthaud
* @brief        Test application for ULIS3.
* @copyright    Copyright 2018-2020 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include <ULIS3>
using namespace ::ul3;

int
main() {
    FVec2F A[4] = { { 0, 0 }, { 200, 0 }, { 200, 200 }, { 0, 200 } };
    FVec2F B[4] = { { 10, 10 }, { 100, 50 }, { 100, 150 }, { 10, 190 } };
    auto mat = FTransform2D::GetPerspectiveTransform( A, B );
    auto inverse = mat.Inverse();

    FVec2F C[4];
    C[0] = FTransform2D::DoHomographyTransform( A[0], mat );
    C[1] = FTransform2D::DoHomographyTransform( A[1], mat );
    C[2] = FTransform2D::DoHomographyTransform( A[2], mat );
    C[3] = FTransform2D::DoHomographyTransform( A[3], mat );

    FVec2F D[4];
    D[0] = FTransform2D::DoHomographyTransform( B[0], inverse );
    D[1] = FTransform2D::DoHomographyTransform( B[1], inverse );
    D[2] = FTransform2D::DoHomographyTransform( B[2], inverse );
    D[3] = FTransform2D::DoHomographyTransform( B[3], inverse );
    auto dummy = 0;
    return  0;
}

