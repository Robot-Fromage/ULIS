// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         AnimatedLayerText.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FAnimatedLayerText class.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Font/Font.h"
#include "Layer/AnimatedLayer/AnimatedLayer.h"
#include "Layer/AnimatedLayer/Sequence.h"
#include "Layer/Components/HasText.h"
#include "Layer/Components/HasBlendInfo.h"
#include "String/WString.h"

ULIS_NAMESPACE_BEGIN
struct ULIS_API FCelTextFactory
{
    ~FCelTextFactory() {};

    FCelTextFactory()
    {}

    TCel< FWString >* MakeBlank( uint32 iExposure = 0 ) {
        return  new TCel< FWString >( nullptr, iExposure );
    }

    TCel< FWString >* MakeNew( uint32 iExposure = 0 ) {
        return  new TCel< FWString >( std::make_shared< FWString >( L"Lorem ipsum dolor sit amet..." ), iExposure );
    }

    TCel< FWString >* MakeSharedFrom( TCel< FWString >* iRefCel, uint32 iExposure = 0 ) {
        ULIS_ASSERT( iRefCel, "Bad input" );
        return  new TCel< FWString >( iRefCel->Data(), iExposure );
    }
};

/////////////////////////////////////////////////////
/// @class      FAnimatedLayerText
/// @brief      The FAnimatedLayerText class provides a class to store text in a layer
///             of a layer stack for painting applications.
class ULIS_API FAnimatedLayerText final
    : public IAnimatedLayer
    , public IHasBlendInfo
    , public TSequence< FWString, FCelTextFactory >
{
    typedef TRoot< IAnimatedLayer > tParent;

public:
    // DTor
    ~FAnimatedLayerText() override;

    // CTors
    FAnimatedLayerText(
          const FString& iName = "Untitled Text"
        , bool iLocked = false
        , bool iTimeLocked = false
        , bool iVisible = true
        , const FColor& iPrettyColor = FColor::Transparent
        , eBlendMode iBlendMode = eBlendMode::Blend_Normal
        , eAlphaMode iAlphaMode = eAlphaMode::Alpha_Normal
        , ufloat iOpacity = 1.f
        , const TRoot< IAnimatedLayer >* iParent = nullptr

        , const FOnNameChanged& iOnNameChanged = FOnNameChanged()
        , const FOnBoolChanged& iOnLockChanged = FOnBoolChanged()
        , const FOnBoolChanged& iOnTimeLockChanged = FOnBoolChanged()
        , const FOnBoolChanged& iOnVisibleChanged = FOnBoolChanged()
        , const FOnColorChanged& iOnColorChanged = FOnColorChanged()
        , const FOnUserDataAdded& iOnUserDataAdded = FOnUserDataAdded()
        , const FOnUserDataChanged& iOnUserDataChanged = FOnUserDataChanged()
        , const FOnUserDataRemoved& iOnUserDataRemoved = FOnUserDataRemoved()
        , const FOnAnimatedParentChanged& iOnParentChanged = FOnAnimatedParentChanged()
        , const FOnAnimatedSelfChanged& iOnSelfChanged = FOnAnimatedSelfChanged()

        , const FOnBlendInfoChanged& iOnBlendInfoChanged = FOnBlendInfoChanged()
    );

    // Disable copy
    FAnimatedLayerText( const FAnimatedLayerText& ) = delete;
    FAnimatedLayerText& operator=( const FAnimatedLayerText& ) = delete;

public:
    // ITypeIdentifiable Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE( "AnimatedText" );

public:
    // IAnimatedLayer interface
    uint64 GetNumFrames() const override;
};

ULIS_NAMESPACE_END

