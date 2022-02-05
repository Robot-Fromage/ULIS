// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Sequence.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for a basic animation Sequence.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/AnimatedLayer/Cel.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TSequence
/// @brief      Basic Animation Sequence of Cels
template< class T, class CelTypeFactory >
class TSequence
    : public CelTypeFactory
{

public:
    virtual ~TSequence() {
        Reset();
    }

    template< typename ... Args >
    TSequence( Args&& ... args )
        : CelTypeFactory( std::forward< Args >(args)... )
    {}

public:
    void
    Reset()
    {
        for( uint64 i = 0; i < mInstances.Size(); ++i )
            delete  mInstances[i];
        mInstances.Clear();
    }

    TArray< TCel< T >* >& Instances() {
        return  mInstances;
    }

    const TArray< TCel< T >* >& Instances() const {
        return  mInstances;
    }

    void PushNewCel() {
        mInstances.PushBack( CelTypeFactory::MakeNew() );
    }

    void PopCel() {
        delete  mInstances.Back();
        mInstances.PopBack();
    }

    void InsertNewCelAtFrame( uint64 iFrame ) {
        mInstances.Insert( IndexAtFrame( iFrame ), CelTypeFactory::MakeNew() );
    }

    void InsertBlankCelAtFrame( uint64 iFrame ) {
        mInstances.Insert( IndexAtFrame( iFrame ), CelTypeFactory::MakeBlank() );
    }
    void InsertSharedResourceCelAtFrame( uint64 iFrame, TCel< T >* iRefCel ) {
        mInstances.Insert( IndexAtFrame( iFrame ), CelTypeFactory::MakeSharedFrom( iRefCel ) );
    }

    TCel< T >* CelAtFrame( uint64 iFrame ) {
        return  CelAtIndex( IndexAtFrame( iFrame ) );
    }

    const TCel< T >* CelAtFrame( uint64 iFrame ) const {
        return  CelAtIndex( IndexAtFrame( iFrame ) );
    }

private:
    TCel< T >* CelAtIndex( uint64 iIndex ) {
        return  mInstances[iIndex];
    }

    const TCel< T >* CelAtIndex( uint64 iIndex ) const {
        return  mInstances[iIndex];
    }

    uint64 IndexAtFrame( uint64 iFrame ) const {
        const uint64 size = mInstances.Size();
        uint64 counter = 0;
        for( uint64 i = 0; i < size; ++i ) {
            const uint64 exposure = mInstances[i]->Exposure();
            if( counter + exposure >= iFrame )
                return  i;
            counter += exposure + 1;
        }
    }

private:
    TArray< TCel< T >* > mInstances;
};

ULIS_NAMESPACE_END

