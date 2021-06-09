// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayer.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the IAnimatedLayer class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Layer/Components/HasCachedAnimatedImageRendering.h"
#include "Layer/AnimatedLayer/AnimatedLayerBuilder.h"
#include "String/String.h"
#include "Image/Color.h"
#include "Memory/Tree.h"
#include "String/CRC32.h"

ULIS_NAMESPACE_BEGIN
// Forward Declarations
class IAnimatedLayer;

// Exports
template class ULIS_API TNode< IAnimatedLayer >;
template class ULIS_API TArray< TNode< IAnimatedLayer >* >;
template class ULIS_API TRoot< IAnimatedLayer >;

// Typedefs
typedef TRoot< IAnimatedLayer > IAnimatedLayerRoot;

/////////////////////////////////////////////////////
/// @class      IAnimatedLayer
/// @brief      The IAnimatedLayer class provides a base abstract class to
///             store an animated layer in an animated  layer stack for
///             animation applications.
class ULIS_API IAnimatedLayer
    : public virtual TNode< IAnimatedLayer >
    , public IHasCachedAnimatedImageRendering
{
    // Typedefs
    typedef TRoot< IAnimatedLayer > tParent;
    typedef TNode< IAnimatedLayer > tSuperClass;

public:
    // DTor
    virtual ~IAnimatedLayer() override = 0;

    // CTor
    IAnimatedLayer(
          const FString& iName = "Untitled"
        , bool iPaintLocked = false
        , bool iTimeLocked = false
        , bool iVisible = true
        , const FColor& iColor = FColor::Transparent
        , tParent* iParent = nullptr
    );

public:
    // Getters
    const FString& Name() const;
    bool PaintLocked() const;
    bool TimeLocked() const;
    bool Visible() const;
    const FColor& PrettyColor() const;

    // Setters
    void SetName( const FString& iName );
    void SetPaintLocked( bool iValue );
    void SetTimeLocked( bool iValue );
    void SetVisible( bool iValue );
    void SetPrettyColor( const FColor& iColor );

    // IAnimatedLayer Interface
    virtual const FString Type() const = 0;
    virtual const uint32 TypeID() const = 0;

    // IHasCachedImageRendering Interface
    virtual void InvalidImageCache() override;

private:
    // Private data members
    FString mName;
    bool mPaintLocked;
    bool mTimeLocked;
    bool mVisible;
    FColor mPrettyColor;
};

ULIS_NAMESPACE_END
