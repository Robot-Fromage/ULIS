// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
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
    constexpr auto bb = sizeof( FFormatMetrics );
    constexpr auto zz = offsetof( FFormatMetrics, SWA );
    FFormatMetrics ll( static_cast< eFormat >( 0 ) );
    FFormatMetrics* LL = &ll;
    uint8*      * IDT   = &ll.IDT;
    eFormat     * FMT   = &ll.FMT;
    eType       * TP    = &ll.TP ;
    eColorModel * CM    = &ll.CM ;
    uint8       * BPC   = &ll.BPC;
    uint8       * NCC   = &ll.NCC;
    uint8       * HEA   = &ll.HEA;
    uint8       * RSC   = &ll.RSC;
    uint8       * SPP   = &ll.SPP;
    uint8       * BPP   = &ll.BPP;
    uint8       * AID   = &ll.AID;
    uint8       * REV   = &ll.REV;
    uint8       * SWA   = &ll.SWA;

    FVec2F A[4] = { { 0, 0 }, { 200, 0 }, { 200, 200 }, { 0, 200 } };
    FVec2F B[4] = { { 10, 10 }, { 100, 50 }, { 100, 150 }, { 10, 190 } };
    auto mat = FTransformation2D::GetPerspectiveTransform( A, B );
    auto inverse = mat.Inverse();

    FBlock PP( 0, 0, 0 );
    FBlock MM( 0, 0, 0 );
    PP = MM;

    FVec2F C[4];
    C[0] = FTransformation2D::DoHomographyTransform( A[0], mat );
    C[1] = FTransformation2D::DoHomographyTransform( A[1], mat );
    C[2] = FTransformation2D::DoHomographyTransform( A[2], mat );
    C[3] = FTransformation2D::DoHomographyTransform( A[3], mat );

    FVec2F D[4];
    D[0] = FTransformation2D::DoHomographyTransform( B[0], inverse );
    D[1] = FTransformation2D::DoHomographyTransform( B[1], inverse );
    D[2] = FTransformation2D::DoHomographyTransform( B[2], inverse );
    D[3] = FTransformation2D::DoHomographyTransform( B[3], inverse );
    auto dummy = 0;
    return  0;
}

