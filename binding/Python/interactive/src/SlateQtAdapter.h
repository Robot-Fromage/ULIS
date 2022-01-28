// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         SlateQtAdapter.h
* @author       Clement Berthaud
* @brief        pyULIS_Interactive application for testing pyULIS.
* @license      Please refer to LICENSE.md
*/
#pragma once

template< typename T >
class SSlateQtAdapter {
public:
    SSlateQtAdapter< T >( QWidget* iParent )
        : m( new T( iParent ) )
    {}

    SSlateQtAdapter< T >( T* iElem, QWidget* iParent )
        : m( iElem )
    {
        m->setParent( iParent );
    }

    SSlateQtAdapter< T >& Def( std::function< void( T* ) > iFunc ) {
        iFunc( m );
        return  *this;
    }

    template< typename U >
    SSlateQtAdapter< T >& AddWidget( SSlateQtAdapter< U >& iObj ) {
        m->addWidget( iObj.m );
        return  *this;
    }

    SSlateQtAdapter< T >& AddWidget( QWidget* iObj ) {
        m->addWidget( iObj );
        return  *this;
    }

    template< typename U >
    SSlateQtAdapter< T >& AddLayout( SSlateQtAdapter< U >& iObj ) {
        m->setLayout( iObj.m );
        return  *this;
    }

    SSlateQtAdapter< T >& AddLayout( QLayout* iObj ) {
        m->setLayout( iObj );
        return  *this;
    }

public:
    T* m;
};

#define SCreateRoot( _Class_, _Parent_ )            SSlateQtAdapter< _Class_ >( _Parent_ )
#define SAssignRoot( _Class_, _Elem_, _Parent_ )    SSlateQtAdapter< _Class_ >( _Elem_, _Parent )
#define SCreateChild( _Class_ )                     SSlateQtAdapter< _Class_ >( nullptr )
#define SExistingChild( _Class_, _Child_ )          SSlateQtAdapter< _Class_ >( _Child_, nullptr )
#define SDef( ... )                                 .Def( [&]( auto i ){ i-> __VA_ARGS__ ; } )
#define SAddWidget( _Elem_ )                        .AddWidget( _Elem_ )
#define SAddLayout( _Elem_ )                        .AddLayout( _Elem_ )
#define SReturn( ... )                              return  ( __VA_ARGS__ ).m

