// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LinearQuadtree.h
* @author       Clement Berthaud
* @brief        This file provides declaration for the FLinearQuadtree class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FLinearQuadtree
/// @brief      The FLinearQuadtree class provides a quadtree that uses an
///             efficient linear scheme to store data and avoid many pointers
///             indirection, overhead, and memory cost.
/// @details    A linear quadtree is a typical way to optimize a quadtree and
///             guarantee O(1) constant time access to leafs and neighbours.
///             It also allows to remove the actual memory cost of a pointer
///             base structure, and access to leafs elements is computed from
///             a simple space-filling curve.
///
///             Although, to be faire, the structure resembles a raw linearized
///             2D array, and all leaves need to be stored, even empty ones from
///             a higher depth. Yet, it is still more efficient than its non linear
///             counterpart in every aspect, and requires less code and maintenance.
///
///             Deep copy and Shallow copy are explicitely forbidden.
///
///             Tile entries are simply morton codes called attributes associated with
///             a leaf type code, the corresponding attribute value is searched in
///             an attribute list associated with the leaf type.
///             Attribute lists are TArrays with a reserved size that is a multiple of
///             the maximum depth at wich one or more leaf exist.
///
///             We could abstract many things about the leaf types and morton codes,
///             but it looks like too much work and i won't use the Linear Quadtree in
///             many places anyway, so it will remain somewhat hardcoded for the pseudo
///             infinite tiled images ATM.
//template<
//    uint8 N // Num Leaf Types
//>
class ULIS_API FLinearQuadtree
{
    //typedef FLinearQuadtree< N > tSelf;
    typedef FLinearQuadtree tSelf;

public:
    /*! Destructor, cleanup bulk and attributes. */
    ~FLinearQuadtree();

    /*! Constructor, initialize empty quadtree structure. */
    FLinearQuadtree();

    /*! Explicitely deleted copy constructor */
    FLinearQuadtree( const tSelf& ) = delete;

    /*! Explicitely deleted copy assignment operator */
    tSelf& operator=( const tSelf& ) = delete;

public:
    // Public API

private:
    // Private Data Members
    uint8* mBulk;
};

ULIS_NAMESPACE_END

