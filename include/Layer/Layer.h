// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Layer.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the ILayer class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "String/String.h"
#include "Image/Color.h"
#include "Memory/Tree.h"
#include "String/CRC32.h"

ULIS_NAMESPACE_BEGIN
class ILayer;
template class ULIS_API TNode< ILayer >;
template class ULIS_API TArray< TNode< ILayer >* >;
template class ULIS_API TRoot< ILayer >;
typedef TRoot< ILayer > ILayerRoot;

/////////////////////////////////////////////////////
/// @class      ILayer
/// @brief      The ILayer class provides a base abstract class to store a layer
///             in a layer stack for painting applications.
class ULIS_API ILayer
    : public virtual TNode< ILayer >
{
    typedef TRoot< ILayer > tParent;
    typedef TNode< ILayer > tSuperClass;

public:
    virtual ~ILayer() override = 0;

    ILayer(
          const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , tParent* iParent = nullptr
    );

    const FString& Name() const;
    bool Locked() const;
    bool Visible() const;
    const FColor& Color() const;

    void SetName( const FString& iName );
    void SetLocked( bool iValue );
    void SetVisible( bool iValue );
    void SetColor( const FColor& iColor );

    virtual const FString Type() const = 0;
    virtual const uint32 TypeID() const = 0;

private:
    FString mName;
    bool mLocked;
    bool mVisible;
    FColor mColor;
};

template< typename T >
class FLayerBuilder {
public:

    FLayerBuilder< T >( T* iElem, ILayerRoot* iParent = nullptr )
        : m( iElem )
    {
        m->SetParent( iParent );
    }

    template< class ... Args >
    static
    FLayerBuilder
    Assign( T** ioElem, Args&& ... args ) {
        T* ptr = new T( std::forward< Args >( args ) ... );
        *ioElem = ptr;
        return  FLayerBuilder( ptr );
    }

    template< class ... Args >
    static
    FLayerBuilder
    Create( Args&& ... args ) {
        T* ptr = new T( std::forward< Args >( args ) ... );
        return  FLayerBuilder( ptr );
    }

    FLayerBuilder< T >& Def( std::function< void( T* ) > iFunc ) {
        iFunc( m );
        return  *this;
    }

    template< typename U >
    FLayerBuilder< T >& AddChild( FLayerBuilder< U >& iObj ) {
        m->AddChild( iObj.m );
        return  *this;
    }

    template< typename U >
    FLayerBuilder< T >& operator[]( FLayerBuilder< U >& iObj ) {
        m->AddChild( iObj.m );
        return  *this;
    }

public:
    T* m;
};

#define ULAssociateStack( _Elem_ )              FLayerBuilder< FLayerStack >( & _Elem_ )
#define ULAssignStack( _Elem_, ... )            FLayerBuilder< FLayerStack >::Assign( & _Elem_, __VA_ARGS__ )
#define ULCreateChild( _Class_, ... )           FLayerBuilder< _Class_ >::Create( __VA_ARGS__ )
#define ULAssignChild( _Class_, _Elem_, ... )   FLayerBuilder< _Class_ >::Assign( & _Elem_, __VA_ARGS__ )
#define ULAddLayer( _Elem_ )                    .AddChild( _Elem_ )
#define ULDef( ... )                            .Def( [&]( auto i ){ i-> __VA_ARGS__ ; } )

ULIS_NAMESPACE_END

