// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Cel.h
* @author       Clement Berthaud
* @brief        This file provides the definition for a basic animation CEL.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include <memory>

ULIS_NAMESPACE_BEGIN
template< class T, class TFactory >
class TSequence;

/////////////////////////////////////////////////////
/// @class      TCel
/// @brief      Basic Animation Cel
template< class T >
class TCel {
public:
    virtual ~TCel() {
    }

    TCel( std::shared_ptr< T > iData, uint32 iExposure = 0 )
        : mData( iData )
        , mExposure( iExposure )
    {}

    std::shared_ptr< T > Data() {
        return  mData;
    }

    const std::shared_ptr< T > Data() const {
        return  mData;
    }

    uint32 Exposure() const {
        return  mExposure;
    }

    bool IsCelResourceShared() const {
        return  mData.use_count() > 1;
    }

    bool IsBlank() const {
        return  mData.get() == nullptr;
    }

private:
    std::shared_ptr< T > mData;
    uint32 mExposure; // Exposure can be zero but results in a 1-frame exposition anyway.
};

//#define ULIS_EXPORT_CEL_CLASSES( CLASS )                \
//    template class ULIS_API TCel< CLASS >;              \
//    template class ULIS_API TArray< TCel< CLASS >* >;   \
//    template class ULIS_API TSequence< CLASS >;

ULIS_NAMESPACE_END

