// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         Map.h
* @author       Clement Berthaud
* @brief        This file provides the definition for the TMap class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Memory/ForwardList.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      TMap
/// @brief      The TMap class provides a simple associative array based on
///             forward list.
/// @details    The need of a custom TMap class rose from the fact
///             that std:: classes cannot be exported easily accross dll
///             boundaries when ULIS is compiled as a shared library.
template< typename T, typename U, typename H >
class TMap
{
public:
    /// @class      TPair
    /// @brief      The TPair class provides a simple node class
    ///             for TMap
    /// @details    TPair is really a simple way to store nodes
    ///             for maps, it holds the key value pair.
    template< typename V, typename W, typename HH >
    struct TPair
    {
    public:
        template< typename, typename > friend class TMap;
        TPair( const V& iKey, const W& iValue )
            : mKey( iKey )
            , mValue( iValue )
            , mHash( HH::Hash( mKey ) )
        {}

        TPair( V&& iKey, W&& iValue )
            : mKey( std::forward< V >( iKey ) )
            , mValue( std::forward< W >( iValue ) )
            , mHash( HH::Hash( mKey ) )
        {}

        template< class... Args >
        TPair( const V& iKey, Args&& ... args )
            : mKey( iKey )
            , mValue( std::forward< Args >(args)... )
            , mHash( HH::Hash( mKey ) )
        {}

        V& Key() {
            return  mKey;
        }

        const V& Key() const {
            return  mKey;
        }

        W& Value() {
            return  mValue;
        }

        const W& Value() const {
            return  mValue;
        }

    private:
        V mKey;
        W mValue;
        uint32 mHash;
    };

    typedef TPair< T, U, H > tPair;

private:
    TForwardList< tPair >* mBucket; ///< The bucket storage.
};

ULIS_NAMESPACE_END

