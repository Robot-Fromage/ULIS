// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Cel.h
* @author       Clement Berthaud
* @brief        This file provides the definition for a basic animation CEL.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"

ULIS_NAMESPACE_BEGIN
enum eCelExtensionBehaviour {
      CelExtensionBehaviour_None
    , CelExtensionBehaviour_Keep
    , CelExtensionBehaviour_Repeat
    , CelExtensionBehaviour_PingPong
    , CelExtensionBehaviour_Shaking
};

enum eCelExtensionExposureType {
      CelExtensionExposureType_Frames
    , CelExtensionExposureType_Instances
};

class FCelExtension {
public:
    FCelExtension(
          uint32 iExposure = ULIS_UINT32_MAX
        , eCelExtensionExposureType iType = CelExtensionExposureType_Frames
        , eCelExtensionBehaviour iBehaviour = CelExtensionBehaviour_None
    );

public:
    uint32 Exposure() const;
    eCelExtensionExposureType Type() const;
    eCelExtensionBehaviour Behaviour() const;
    void SetExposure( uint32 iValue );
    void SetType( eCelExtensionExposureType iValue );
    void SetBehaviour( eCelExtensionBehaviour iValue );

private:
    uint32 mExposure;
    eCelExtensionExposureType mType;
    eCelExtensionBehaviour mBehaviour;
};

/////////////////////////////////////////////////////
/// @class      FCelInfo
/// @brief      Basic Animation Cel Info
class FCelInfo {
    FCelInfo(
          uint32 iExposure = 1
        , const FCelExtension& iPreBehaviour = FCelExtension()
        , const FCelExtension& iPostBehaviour = FCelExtension()
    );

    uint32 Exposure() const;
    FCelExtension& PreBehaviour();
    const FCelExtension& PreBehaviour() const;
    FCelExtension& PostBehaviour();
    const FCelExtension& PostBehaviour() const;

private:
    uint32 mExposure;
    FCelExtension mPreBehaviour;
    FCelExtension mPostBehaviour;
};

/////////////////////////////////////////////////////
/// @class      TCel
/// @brief      Basic Animation Cel
template< class Type >
class ULIS_API TCel {

public:
    virtual ~TCel() {
        delete  mData;
    }

    template< class ... Args >
    TCel( const FCelInfo& iInfo, Args&& ... args )
        : mInfo( iInfo )
        , mData( new Type( std::forward< Args >( args ) ... ) )
    {}

    Type& Data() {
        return  mData;
    }

    const Type& Data() const {
        return  mData;
    }

private:
    FCelInfo mInfo;
    Type* mData;
};

/////////////////////////////////////////////////////
/// @class      TSequence
/// @brief      Basic Animation Sequence of Cels
template< class Type >
class ULIS_API TSequence {

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
        for( uint64 i = 0; i < mInstances.Size(); ++i )
            delete  mInstances[i];
        mInstances.Clear();
    }

    TArray< TCel< Type > >& Instances() {
        return  mInstances;
    }

    const TArray< TCel< Type > >& Instances() const {
        return  mInstances;
    }

private:
    TArray< TCel< Type > > mInstances;
};

ULIS_NAMESPACE_END

