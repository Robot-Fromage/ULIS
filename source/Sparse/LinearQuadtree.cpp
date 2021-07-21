// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LinearQuadtree.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FLinearQuadtree class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/LinearQuadtree.h"
#include "Math/Geometry/Morton.h"

ULIS_NAMESPACE_BEGIN
namespace details {
constexpr TMortonEncodeKeys8bit< 16, 0 > sgMortonEncodeKeys8bit_2D_16_X; // 16 bytes
constexpr TMortonEncodeKeys8bit< 16, 1 > sgMortonEncodeKeys8bit_2D_16_Y; // 16 bytes
constexpr TMortonDecodeKeys8bit2D< 256 > sgMortonDecodeKeys8bit_2D_16_XY; // 256 bytes
} // namespace details


FLQTree::~FLQTree() {
    for( uint16 i = 0; i < 256; ++i )
        mBulk[ i ]->DecreaseRefCount();
}

FLQTree::FLQTree()
    : mBulk { 0 }
{
}


FTile*
FLQTree::TileAtLeafCoordinates( uint8 iX, uint8 iY ) {
    return  mBulk[
                  details::sgMortonEncodeKeys8bit_2D_16_X.keys[ iX ]
                | details::sgMortonEncodeKeys8bit_2D_16_Y.keys[ iX ]
    ];
}

const FTile*
FLQTree::TileAtLeafCoordinates( uint8 iX, uint8 iY ) const {
    return  mBulk[
                  details::sgMortonEncodeKeys8bit_2D_16_X.keys[ iX ]
                | details::sgMortonEncodeKeys8bit_2D_16_Y.keys[ iX ]
    ];
}

FTile*
FLQTree::TileAtPixelCoordinates( uint16 iX, uint16 iY ) {
    return  TileAtLeafCoordinates( iX / sm_leaf_size_as_pixels, iY / sm_leaf_size_as_pixels );
}
const FTile*
FLQTree::TileAtPixelCoordinates( uint16 iX, uint16 iY ) const {
    return  TileAtLeafCoordinates( iX / sm_leaf_size_as_pixels, iY / sm_leaf_size_as_pixels );
}

FTile*
FLQTree::TileAtMortonKey( uint8 iMortonKey ) {
    return  mBulk[ iMortonKey ];
}

const FTile*
FLQTree::TileAtMortonKey( uint8 iMortonKey ) const {
    return  mBulk[ iMortonKey ];
}

void
FLQTree::SetTileAtLeafCoordinates( uint8 iX, uint8 iY, FTile* iTile ) {
    uint8 key =
          details::sgMortonEncodeKeys8bit_2D_16_X.keys[ iX ]
        | details::sgMortonEncodeKeys8bit_2D_16_Y.keys[ iX ];
    mBulk[ key ]->DecreaseRefCount();
    mBulk[ key ] = iTile;
    mBulk[ key ]->IncreaseRefCount();
}


void
FLQTree::SetTileAtPixelCoordinates( uint16 iX, uint16 iY, FTile* iTile ) {
    uint8 key =
          details::sgMortonEncodeKeys8bit_2D_16_X.keys[ iX ] / sm_leaf_size_as_pixels
        | details::sgMortonEncodeKeys8bit_2D_16_Y.keys[ iX ] / sm_leaf_size_as_pixels;
    mBulk[ key ]->DecreaseRefCount();
    mBulk[ key ] = iTile;
    mBulk[ key ]->IncreaseRefCount();
}

void
FLQTree::SetTileAtMortonKey( uint8 iMortonKey, FTile* iTile ) {
    mBulk[ iMortonKey ]->DecreaseRefCount();
    mBulk[ iMortonKey ] = iTile;
    mBulk[ iMortonKey ]->IncreaseRefCount();
}

ULIS_NAMESPACE_END

