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

    void PushCel();
    void PopCel();
    void InsertNewCelAtIndex( uint32 iIndex, bool iPreserveSequenceTimingIfPossible = true );
    void InsertNewCelAtFrame( uint32 iFrame, bool iPreserveSequenceTimingIfPossible = true );
    void InsertBlankCelAtIndex( uint32 iIndex, bool iPreserveSequenceTimingIfPossible = true );
    void InsertBlankCelAtFrame( uint32 iFrame, bool iPreserveSequenceTimingIfPossible = true );
    void InsertSharedResourceCelAtIndex( uint32 iIndex, TCel< T >* iRefCel, bool iPreserveSequenceTimingIfPossible = true );
    void InsertSharedResourceCelAtFrame( uint32 iFrame, TCel< T >* iRefCel, bool iPreserveSequenceTimingIfPossible = true );
    TCel< T >* CelAtIndex( uint32 iIndex );
    TCel< T >* CelAtFrame( uint32 iFrame );
    const TCel< T >* CelAtIndex( uint32 iIndex ) const;
    const TCel< T >* CelAtFrame( uint32 iFrame ) const;

private:
    TArray< TCel< T >* > mInstances;
};

ULIS_NAMESPACE_END

