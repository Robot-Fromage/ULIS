// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Sequence.h
* @author       Clement Berthaud, Eric Scholl
* @brief        This file provides the declaration for a basic animation Sequence.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN

template< class CelType >
class TCelFactory {

public:
    virtual ~TCelFactory() = 0;
    TCelFactory() {};

public:
    virtual TCel<CelType>* CreateCel(const FCelInfo& iCelInfo) = 0;
};

/////////////////////////////////////////////////////
/// @class      TSequence
/// @brief      Basic Animation Sequence of Cels
template< class Type >
class TSequence {

public:
    virtual ~TSequence() {
        Reset();
    }

    TSequence(const TCelFactory<Type>* iCelFactory)
        : mCelFactory(iCelFactory)
        , mFirstFrame(0)
    {}

    //constructor with predefined sequence of empty cels
    TSequence(const TCelFactory<Type>* iCelFactory, const TArray<FCelInfo> iCelInfos, uint32 iFirstFrame)
        : mCelFactory(iCelFactory)
        , mFirstFrame(iFirstFrame)
    {
        for (int i = 0; i < iCelInfos.Size(); i++)
        {
            mCels.PushBack(iCelFactory->CreateCel(iCelInfos[i]));
        }
    }

public:
    void
    Reset()
    {
        for( uint64 i = 0; i < mCels.Size(); ++i )
            delete  mCels[i];
        mCels.Clear();
    }

    const TArray< TCel< Type >* >& Cels() const {
        return  mCels; 
    }

    TArray< TCel< Type >* >& Cels() {
        return  mCels;
    }

    void AddCel(const FCelInfo& iInfo) {
        mCels.PushBack(mCelFactory->CreateCel(iInfo));
    }

private:
    TCelFactory<Type>* mCelFactory;
    TArray< TCel< Type >* > mCels;
    uint32 mFirstFrame;
};

ULIS_NAMESPACE_END

