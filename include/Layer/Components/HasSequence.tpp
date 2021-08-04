// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasSequence.tpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the THasSequence class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Layer/Components/HasSequence.h"

#define TEMPLATE template< class SequenceType >
#define CLASS THasSequence< SequenceType >

ULIS_NAMESPACE_BEGIN
TEMPLATE
CLASS::~THasSequence() {
    if( mSequence )
        delete mSequence;

    ULIS_DEBUG_PRINTF( "THasSequence Destroyed" )
}

TEMPLATE
CLASS::THasSequence(
      const TCelFactory<SequenceType>* iCelFactory
    , const TOnSequenceChanged< TSequence<SequenceType> >& iDelegate
)
    : TOnSequenceChanged< TSequence<SequenceType> >( iDelegate )
    , mSequence( new TSequence<SequenceType>(iArgs) )
{
    Invoke( mSequence );
    ULIS_DEBUG_PRINTF( "THasSequence Created" )
}

TEMPLATE
CLASS::THasSequence(
      const TCelFactory<SequenceType>* iCelFactory
    , TSequence<SequenceType>* iSequence
    , const TOnSequenceChanged< TSequence<SequenceType> >& iDelegate
)
    : TOnSequenceChanged< TSequence<SequenceType> >( iDelegate )
    , mSequence( iSequence )
{
    Invoke( mSequence );
    ULIS_DEBUG_PRINTF( "THasSequence Created" )
}

TEMPLATE
TSequence<SequenceType>*
CLASS::Sequence() {
    return  mSequence;
}

TEMPLATE
const
TSequence<SequenceType>*
CLASS::Sequence() const {
    return  mSequence;
}

TEMPLATE
void
CLASS::ReallocSequence(
    const TCelFactory<SequenceType>* iCelFactory
)
{
    if( mSequence )
        delete mSequence;
    mSequence = new TSequence<SequenceType>(iCelFactory);
    Invoke( mSequence );
}

TEMPLATE
void
CLASS::ReplaceSequence(TSequence<SequenceType>* iValue )
{
    if( mSequence )
        delete mSequence;
    mSequence = iValue;
    Invoke( mSequence );
}

ULIS_NAMESPACE_END

// Template Macro Utility
#undef TEMPLATE
#undef CLASS

