// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerFolder.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FAnimatedLayerFolder class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/AnimatedLayerImage.h"

ULIS_NAMESPACE_BEGIN
#pragma warning(push)
#pragma warning(disable : 4250) // Shut dominance of inheritance for ULIS::FLayerImage::InitFromParent
/////////////////////////////////////////////////////
/// @class      FAnimatedLayerFolder
/// @brief      The FAnimatedLayerFolder class provides a class to store a folder of
///             layers in a layer stack for painting applications.
class ULIS_API FAnimatedLayerFolder final
    : public FAnimatedLayerImage
    , public TRoot< IAnimatedLayer >
{
    // Typedefs
    typedef TRoot< IAnimatedLayer > tParent;
    typedef FAnimatedLayerImage     tSuperClass;

public:
    // DTor
    ~FAnimatedLayerFolder() override;

    // CTors
    FAnimatedLayerFolder(
          const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , uint16 iWidth = 1
        , uint16 iHeight = 1
        , eFormat iFormat = Format_RGBA8
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iAlphaLocked = false
        , bool iCollapsed = false
        , tParent* iParent = nullptr
    );

    FAnimatedLayerFolder(
          FBlock* iBlock
        , const FString& iName = "Untitled"
        , bool iLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , bool iAlphaLocked = false
        , bool iCollapsed = false
        , tParent* iParent = nullptr
    );

    // Disable copy
    FAnimatedLayerFolder( const FAnimatedLayerFolder& ) = delete;
    FAnimatedLayerFolder& operator=( const FAnimatedLayerFolder& ) = delete;

public:
    // Getters
    bool IsCollapsed() const;

    // Setters
    void SetCollapsed( bool iValue );

    // Utils
    template< typename T > T& Find( const FString& iName ) { return  dynamic_cast< T& >( (*this)[ iName ] ); }
    template< typename T > const T& Find( const FString& iName ) const { return  dynamic_cast< const T& >( (*this)[ iName ] ); }

    // Static Interface
    constexpr static const char* StaticType() { return  mType; }
    constexpr static const uint32 StaticTypeID() { return  crc32b( mType); }

    // ILayer Interface
    const FString Type() const override { return  StaticType(); }
    const uint32 TypeID() const override { return  StaticTypeID(); }

    // TRoot overload-shadow Interface
    using TRoot< IAnimatedLayer >::operator[];
    IAnimatedLayer& operator[]( const FString& iName );
    const IAnimatedLayer& operator[]( const FString& iName ) const;

    // ICachedImageRendering Interface
    FEvent RenderCache( FContext& iCtx ) override;
    virtual FEvent RenderImage(
          FContext& iCtx
        , FBlock& ioBlock
        , const FRectI& iRect = FRectI::Auto
        , const FVec2I& iPos = FVec2I( 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
    ) override;

private:
    // Private data members
    bool mCollapsed;
    constexpr static const char* mType = "Folder";
};
#pragma warning(pop)

ULIS_NAMESPACE_END

