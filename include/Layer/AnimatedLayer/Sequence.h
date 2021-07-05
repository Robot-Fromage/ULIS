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
/////////////////////////////////////////////////////
/// @class      TSequence
/// @brief      Basic Animation Sequence of Cels
template< class Type >
class TSequence {

public:
    virtual ~TSequence() {
        Reset();
    }

    TSequence()
    {}

public:
    void
    Reset()
    {
        for( uint64 i = 0; i < mCels.Size(); ++i )
            delete  mCels[i];
        mCels.Clear();
    }

    TArray< TCel< Type >* >& Cels() {
        return  mCels;
    }

    const TArray< TCel< Type >* >& Cels() const {
        return  mCels;
    }

private:
    TArray< TCel< Type >* > mCels;
};

ULIS_NAMESPACE_END

