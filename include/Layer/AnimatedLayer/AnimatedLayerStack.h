// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerStack.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FAnimatedLayerStack class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Size2D.h"
#include "Layer/AnimatedLayer/AnimatedLayer.h"
#include "Layer/AnimatedLayer/AnimatedLayerImage.h"
#include "Layer/AnimatedLayer/AnimatedLayerFolder.h"
#include "Layer/AnimatedLayer/AnimatedLayerText.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FAnimatedLayerStack
/// @brief      The FAnimatedLayerStack class provides a class to store a layer stack
///             for painting applications.
class ULIS_API FAnimatedLayerStack final
    : public IAnimatedLayer
    , public TRoot< IAnimatedLayer >
    , public IHasFormat
    , public IHasColorSpace
    , public IHasSize2D
{
public:
    // DTor
    ~FAnimatedLayerStack() override;

    // CTor
    FAnimatedLayerStack(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
    );

    // Disable copy
    FAnimatedLayerStack( const FAnimatedLayerStack& ) = delete;
    FAnimatedLayerStack& operator=( const FAnimatedLayerStack& ) = delete;

public:
    // Utils
    template< typename T > T& Find( const FString& iName ) { return  dynamic_cast< T& >( (*this)[ iName ] ); }
    template< typename T > const T& Find( const FString& iName ) const { return  dynamic_cast< const T& >( (*this)[ iName ] ); }
    void Reset(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
    );

    // Static Interface
    constexpr static const char* StaticType() { return  mType; }
    constexpr static const uint32 StaticTypeID() { return  crc32b( mType ); }

    // ILayer Interface
    const FString Type() const override { return  StaticType(); }
    const uint32 TypeID() const override { return  StaticTypeID(); }

    // TRoot overload-shadow Interface
    using TRoot< IAnimatedLayer >::operator[];
    IAnimatedLayer& operator[]( const FString& iName );
    const IAnimatedLayer& operator[]( const FString& iName ) const;

    // IHasCachedImageRendering Interface
    FEvent RenderImage(
          FContext& iCtx
        , FBlock& ioBlock
        , uint32 iFrame
        , const FRectI& iRect = FRectI::Auto
        , const FVec2I& iPos = FVec2I( 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
    ) override;

private:
    // Private data members
    constexpr static const char* mType = "Stack";
};

ULIS_NAMESPACE_END

