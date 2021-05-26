// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Tree.h
* @author       Clement Berthaud
* @brief        This file provides the definition for a basic Tree.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/Array.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Forward declaration
template< class Type >
class TAbstractRoot;

/////////////////////////////////////////////////////
/// @class      TNode
/// @brief      Basic node
template< class Type >
class TNode {

    friend class TAbstractRoot< Type >;
    typedef TNode< Type > tSelf;

private:
    virtual ~TNode() {
        if( mData )
            delete  mData;
    }

    TNode()
        : mData( new Type() )
        , mParent( nullptr )
    {}

    TNode( Type* iData, tSelf* iParent = nullptr )
        : mData( iData )
        , mParent( iParent )
    {}

    virtual bool CanHaveChildren() const {
        return  false;
    }

private:
    Type* mData;
    tSelf* mParent;
};

/////////////////////////////////////////////////////
/// @class      TAbstractRoot
/// @brief      Abstract Root
template< class Type >
class TAbstractRoot
    : public TNode< Type >
{
    typedef TNode< Type > tNode;

public:
    virtual ~TAbstractRoot() override {
        Reset();
    }

    void
    Reset()
    {
        for( uint64 i = 0; i < mChilds.Size(); ++i )
            delete  mChilds[i];
        mChilds.Clear();
    }

    TAbstractRoot()
        : tNode()
    {}

    TAbstractRoot( Type* iData, tNode* iParent = nullptr )
        : tNode( iData, iParent )
    {}

    virtual bool CanHaveChildren() const override {
        return  true;
    }

    TArray< tNode* >& Children() {
        return  mChildren;
    }

    const TArray< ILayer* >& Children() const {
        return  mChildren;
    }

    void DeleteChild( int iIndex ) {
        ULIS_ASSERT( iIndex < mChildren.Size() );
        delete mChildren[ iIndex ];
        mChildren.Erase( iIndex, 1 );
    }

private:
    TArray< tNode* > mChildren;
};

/////////////////////////////////////////////////////
/// @class      TConcreteRoot
/// @brief      Concrete Root with CRTP
template< class ConcreteDerived, class Type >
class TConcreteRoot
    : public TAbstractRoot< Type >
{
    typedef TNode< Type > tNode;
    typedef TConcreteRoot< ConcreteDerived, Type > tSelf;
    typedef TAbstractRoot< Type > tParent;

public:
    virtual ~TConcreteRoot() override {
    }

    TConcreteRoot()
        : tParent()
    {}

    TConcreteRoot( Type* iData, tNode* iParent = nullptr )
        : tParent( iData, iParent )
    {}

    ConcreteDerived& AddChild( Type* iData, uint64 iIndex = ULIS_UINT64_MAX ) {
        tNode* node = new tNode( iData, this );
        if( iIndex >= Children().Size() )
            Children().PushBack( node );
        else
            Children().Insert( iIndex, node );
        return *this;
    }
};

ULIS_NAMESPACE_END

