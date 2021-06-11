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
#include <functional>

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
// Forward declaration
template< class Type >
class TRoot;

/////////////////////////////////////////////////////
/// @class      TNode
/// @brief      Basic node
template< class Type >
class ULIS_API TNode {
    typedef TRoot< Type > tParent;
    friend class TRoot< Type >;

public:
    virtual ~TNode() {
    }

    TNode( const tParent* iParent = nullptr )
        : mParent( iParent )
    {}

    Type& Self() {
        //return  *reinterpret_cast< Type* >( __GetDerivedPtr__() ); // No RTTI
        return  *dynamic_cast< Type* >( this ); // Yes RTTI
    }

    const Type& Self() const {
        //return  *reinterpret_cast< const Type* >( __GetDerivedPtr__() ); // No RTTI
        return  *dynamic_cast< const Type* >( this ); // Yes RTTI
    }

    Type& Def( std::function< void( Type& ) > iFunc ) {
        iFunc( Self() );
        return  Self();
    }

    const tParent* Parent() const {
        return  mParent;
    }

    void SetParent( const tParent* iParent ) {
        mParent = iParent;
    }

    const tParent* TopLevelParent() const {
        if( mParent == nullptr )
            return  dynamic_cast< const tParent* >( this );

        const tParent* parent = mParent;
        while( parent->mParent )
            parent = parent->mParent;
        return  parent;
    }

private:
    // In case RTTI is disabled this cas enable virtual dynamic casting
    // alternative for derived classes.
    /*
    #define ULIS_DERIVED_FROM_NODE                      \
        virtual void* __GetDerivedPtr__() override {    \
            return this;                                \
        }

    virtual void* __GetDerivedPtr__() = 0 {
        return nullptr;
    }
    */

    virtual void InitFromParent( const tParent* iParent ) {
    }

private:
    const tParent* mParent;
};

/////////////////////////////////////////////////////
/// @class      TRoot
/// @brief      Abstract Root
template< class Type >
class ULIS_API TRoot
    : public virtual TNode< Type >
{
    typedef TNode< Type >   tSuperClass;
    typedef TRoot< Type >   tParent;
    typedef TRoot< Type >   tSelf;
    typedef TNode< Type >   tNode;

public:
    virtual ~TRoot() override {
        Reset();
    }

    TRoot( tParent* iParent = nullptr )
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

    Type& operator[]( uint64 iIndex ) {
        return  mChildren[ iIndex ]->Self();
    }

    const Type& operator[]( uint64 iIndex ) const {
        return  mChildren[ iIndex ]->Self();
    }

    void DeleteChild( int iIndex ) {
        ULIS_ASSERT( iIndex < mChildren.Size(), "Bad Index" );
        delete mChildren[ iIndex ];
        mChildren.Erase( iIndex, 1 );
    }

    tSelf& AddChild( tNode* iNode, uint64 iIndex = ULIS_UINT64_MAX ) {
        iNode->SetParent( this );
        iNode->InitFromParent( this );
        if( iIndex >= Children().Size() )
            Children().PushBack( iNode );
        else
            Children().Insert( iIndex, iNode );
        return  *this;
    }

    template< typename ChildType, class ... Args >
    tSelf& AddChild( Args&& ... args ) {
        return  AddChild( new ChildType( std::forward< Args >(args) ... ), ULIS_UINT64_MAX );
    }

    template< typename ChildType, class ... Args >
    tSelf& AddChild( uint64 iIndex, Args&& ... args ) {
        return  AddChild( new ChildType( std::forward< Args >(args) ... ), iIndex );
    }

private:
    TArray< tNode* > mChildren;
};

ULIS_NAMESPACE_END

