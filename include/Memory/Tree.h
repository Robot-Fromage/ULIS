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

    typedef TNode< Type >           tSelf;
    typedef TAbstractRoot< Type >   tParent;

public:
    virtual ~TNode() {
    }

    TNode( tParent* iParent = nullptr )
        : mParent( iParent )
    {}

    Type* Self() {
        return  this;
    }

    const Type* Self() const {
        return  this;
    }

    tParent* Parent() {
        return  mParent;
    }

    const tParent* Parent() const {
        return  mParent;
    }

    void SetParent( tParent* iParent ) {
        mParent = iParent;
    }

private:
    tParent* mParent;
};

/////////////////////////////////////////////////////
/// @class      TAbstractRoot
/// @brief      Abstract Root
template< class Type >
class TAbstractRoot
    : public virtual TNode< Type >
{
    typedef TAbstractRoot< Type >   tSelf;
    typedef TNode< Type >           tSuperClass;
    typedef TAbstractRoot< Type >   tParent;
    typedef TNode< Type >           tNode;

public:
    virtual ~TAbstractRoot() override {
        Reset();
    }

    TAbstractRoot( tParent* iParent = nullptr )
        : tSuperClass( iParent )
    {}

    void
    Reset()
    {
        for( uint64 i = 0; i < mChildren.Size(); ++i )
            delete  mChildren[i];
        mChildren.Clear();
    }

    TArray< tNode* >& Children() {
        return  mChildren;
    }

    const TArray< tNode* >& Children() const {
        return  mChildren;
    }

    void DeleteChild( int iIndex ) {
        ULIS_ASSERT( iIndex < mChildren.Size(), "Bad Index" );
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
    typedef TConcreteRoot< ConcreteDerived, Type > tSelf;
    typedef TAbstractRoot< Type > tSuperClass;
    typedef TConcreteRoot< ConcreteDerived, Type > tParent;
    typedef TNode< Type > tNode;

public:
    virtual ~TConcreteRoot() override {
    }

    TConcreteRoot( tParent* iParent = nullptr )
        : tSuperClass( iParent )
    {}

    ConcreteDerived& AddChild( tNode* iNode, uint64 iIndex = ULIS_UINT64_MAX ) {
        iNode->SetParent( this );
        if( iIndex >= Children().Size() )
            Children().PushBack( iNode );
        else
            Children().Insert( iIndex, iNode );
        return *static_cast< ConcreteDerived* >( this );
    }
};

ULIS_NAMESPACE_END

