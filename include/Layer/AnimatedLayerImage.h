// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerImage.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FAnimatedLayerImage class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/AnimatedLayer.h"

ULIS_NAMESPACE_BEGIN
/////////////////////////////////////////////////////
/// @class      FAnimatedLayerImage
/// @brief      The FAnimatedLayerImage class provides a class to store an image in a
///             layer stack for painting applications.
class ULIS_API FAnimatedLayerImage
    : public IAnimatedLayer
{
    // Typedefs
    typedef TRoot< IAnimatedLayer > tParent;
    typedef IAnimatedLayer          tSuperClass;

public:
    // DTor
    virtual ~FAnimatedLayerImage() override;

    // CTors
    FAnimatedLayerImage(
          const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , uint16 iWidth = 0
        , uint16 iHeight = 0
        , eFormat iFormat = Format_RGBA8
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iAlphaLocked = false
        , tParent* iParent = nullptr
    );

    FAnimatedLayerImage(
          FBlock* iBlock
        , const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iAlphaLocked = false
        , tParent* iParent = nullptr
    );

    // Disable copy
    FAnimatedLayerImage( const FAnimatedLayerImage& ) = delete;
    FAnimatedLayerImage& operator=( const FAnimatedLayerImage& ) = delete;

public:
    // Getters
    FBlock& Block();
    const FBlock& Block() const;
    eBlendMode BlendMode() const;
    eAlphaMode AlphaMode() const;
    ufloat Opacity() const;
    bool IsAlphaLocked() const;

    // Setters
    void SetBlendMode( eBlendMode iValue );
    void SetAlphaMode( eAlphaMode iValue );
    void SetOpacity( ufloat iValue );
    void SetAlphaLocked( bool iValue );

    // Static Interface
    constexpr static const char* StaticType() { return  mType; }
    constexpr static const uint32 StaticTypeID() { return  crc32b( mType); }

    // ILayer Interface
    const FString Type() const override { return  StaticType(); }
    const uint32 TypeID() const override { return  StaticTypeID(); }

    // ICachedImageRendering Interface
    virtual FEvent RenderImage(
          FContext& iCtx
        , FBlock& ioBlock
        , const FRectI& iRect = FRectI::Auto
        , const FVec2I& iPos = FVec2I( 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
    ) override;

protected:
    // TNode Interface
    virtual void InitFromParent( const tParent* iParent ) override;

private:
    // Private data members
    FBlock* mBlock;
    eBlendMode mBlendMode;
    eAlphaMode mAlphaMode;
    ufloat mOpacity;
    bool mAlphaLock;
    constexpr static const char* mType = "Image";
};

ULIS_NAMESPACE_END

