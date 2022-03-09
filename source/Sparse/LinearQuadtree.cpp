// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LinearQuadtree.cpp
* @author       Clement Berthaud
* @brief        This file provides definition for the FLinearQuadtree class.
* @license      Please refer to LICENSE.md
*/
#include "Sparse/LinearQuadtree.h"
#include "Sparse/TilePool.h"
#include "Math/Geometry/Morton.h"

ULIS_NAMESPACE_BEGIN
namespace details {
// Using LUT Encode / Decode Keys seems to be the simplest efficient option.
// Computing the bitshifts on the fly could be inneficient
// Using MBI intrinsics is a pain if we have to check support for the intrinsic set
// The LUT are small enough with the hardcoded tree size to allow embedding them within the program.
// IF we need larger trees, then going for a more complex solution could be more relevant.
// Either: MBI or AVX shuffle with fallback to manual magic bits shuffle in 64bits keys.
// Or: Computing cached LUTs at Runtime.
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
    , mNumEntries( 0 )
{
}

void
FLQTree::Erase( uint16 iX, uint16 iY ) {
    uint8 key =
          details::sgMortonEncodeKeys8bit_2D_16_X.keys[ iX / sm_leaf_size_as_pixels ]
        | details::sgMortonEncodeKeys8bit_2D_16_Y.keys[ iY / sm_leaf_size_as_pixels ];
    if( mBulk[ key ] ) {
        --mNumEntries;
        mBulk[ key ]->DecreaseRefCount();
        mBulk[ key ] = nullptr;
    }
}

void
FLQTree::Clear() {
    mNumEntries = 0;
    for( uint16 i = 0; i < 256; ++i ) {
        mBulk[ i ]->DecreaseRefCount();
        mBulk[ i ] = nullptr;
    }
}

FRectI
FLQTree::LeafGeometry() const {
    FRectI ret;
    for( uint16 i = 0; i < 256; ++i ) {
        if( mBulk[ i ] ) {
            int x = details::sgMortonDecodeKeys8bit_2D_16_XY.keys[i].x;
            int y = details::sgMortonDecodeKeys8bit_2D_16_XY.keys[i].y;
            ret = ret.UnionLeaveEmpty( FRectI( x, y, 1, 1 ) );
        }
    }
    ret.x *= sm_leaf_size_as_pixels;
    ret.y *= sm_leaf_size_as_pixels;
    ret.w *= sm_leaf_size_as_pixels;
    ret.h *= sm_leaf_size_as_pixels;
    return  ret;
}

const uint8*
FLQTree::QueryConst( FTilePool& iPool, uint8 iX, uint8 iY ) const {
    uint8 key =
          details::sgMortonEncodeKeys8bit_2D_16_X.keys[ iX / sm_leaf_size_as_pixels ]
        | details::sgMortonEncodeKeys8bit_2D_16_Y.keys[ iY / sm_leaf_size_as_pixels ];
    if( mBulk[ key ] )
        return  *( mBulk[ key ]->mClient );
    else
        return  iPool.EmptyTile();
}

FTile**
FLQTree::QueryMutable( FTilePool& iPool, uint8 iX, uint8 iY ) {
    uint8 key =
          details::sgMortonEncodeKeys8bit_2D_16_X.keys[ iX / sm_leaf_size_as_pixels ]
        | details::sgMortonEncodeKeys8bit_2D_16_Y.keys[ iY / sm_leaf_size_as_pixels ];
    // Perform data copy for imminent mutable change if needed
    if( !mBulk[ key ] )
        ++mNumEntries;
    mBulk[ key ] = iPool.SplitMutable( mBulk[ key ] );
    mBulk[ key ]->mDirty = true;
    return  &( mBulk[ key ] );
}

bool
FLQTree::IsEmpty() const {
    return  mNumEntries == 0;
}

void
FLQTree::SanitizeNow( FTilePool& iPool ) {
    if( IsEmpty() )
        return;

    for( uint16 i = 0; i < 256; ++i ) {
        FTile* prev = mBulk[ i ];
        FTile* next = iPool.RedundantHashMerge( prev );
        mBulk[ i ] = next;
        if( prev != nullptr && next == nullptr )
            --mNumEntries;
    }
}

ULIS_NAMESPACE_END

