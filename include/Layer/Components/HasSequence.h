// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         HasSequence.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the THasSequence class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Core/CallbackCapable.h"
#include "Layer/AnimatedLayer/Sequence.h"

ULIS_NAMESPACE_BEGIN
template< class SequenceType > using TSequenceChangedDelegate = TLambdaCallback< void, const TSequence<SequenceType>* >;
template< class SequenceType > using TOnSequenceChanged = TCallbackCapable< TSequenceChangedDelegate< SequenceType > >;
/////////////////////////////////////////////////////
/// @class      THasSequence
/// @brief      Simple HasSequence class.
template< class SequenceType >
class THasSequence
    : public TOnSequenceChanged< SequenceType >
{
protected:
    ~THasSequence();
    THasSequence(
          const TCelFactory<SequenceType>* iCelFactory
        , const TOnSequenceChanged< TSequence<SequenceType> >& iDelegate = TOnSequenceChanged< TSequence<SequenceType>>()
    );

    THasSequence(
          const TCelFactory<SequenceType>* iCelFactory
        , TSequence<SequenceType>* iSequence
        , const TOnSequenceChanged< TSequence<SequenceType> >& iDelegate = TOnSequenceChanged< TSequence<SequenceType> >()
    );

public:
    TSequence<SequenceType>* Sequence();
    const TSequence<SequenceType>* Sequence() const;

protected:
    void ReallocSequence(
        const TCelFactory<SequenceType>* iCelFactory
    );
    void ReplaceSequence( TSequence<SequenceType>* iValue );

private:
    TSequence<SequenceType>* mSequence;
};

ULIS_NAMESPACE_END

#include "Layer/Components/HasSequence.tpp"

