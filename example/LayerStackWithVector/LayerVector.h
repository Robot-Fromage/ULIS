// IDDN.FR.001.250001.006.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerVector.h
* @author       Clement Berthaud
* @brief        LayerStack application for ULIS.
* @license      Please refer to LICENSE.md
*/
#include <ULIS>
#include "VectorShapes.h"

class FLayerVector final
    : public ::ULIS::ILayer
    , public ::ULIS::IHasBlendInfo
{
public:
    // DTor
    ~FLayerVector() override;

    // CTors
    FLayerVector(
          const ::ULIS::FString& iName = "Untitled Vector"
        , bool iLocked = false
        , bool iVisible = true
        , const ::ULIS::FColor& iPrettyColor = ::ULIS::FColor::Transparent
        , ::ULIS::eBlendMode iBlendMode = ::ULIS::eBlendMode::Blend_Normal
        , ::ULIS::eAlphaMode iAlphaMode = ::ULIS::eAlphaMode::Alpha_Normal
        , ::ULIS::ufloat iOpacity = 1.f
        , const ::ULIS::TRoot< ILayer >* iParent = nullptr

        , const ::ULIS::FOnNameChanged& iOnNameChanged = ::ULIS::FOnNameChanged()
        , const ::ULIS::FOnBoolChanged& iOnLockChanged = ::ULIS::FOnBoolChanged()
        , const ::ULIS::FOnBoolChanged& iOnVisibleChanged = ::ULIS::FOnBoolChanged()
        , const ::ULIS::FOnColorChanged& iOnColorChanged = ::ULIS::FOnColorChanged()
        , const ::ULIS::FOnUserDataAdded& iOnUserDataAdded = ::ULIS::FOnUserDataAdded()
        , const ::ULIS::FOnUserDataChanged& iOnUserDataChanged = ::ULIS::FOnUserDataChanged()
        , const ::ULIS::FOnUserDataRemoved& iOnUserDataRemoved = ::ULIS::FOnUserDataRemoved()
        , const ::ULIS::FOnParentChanged& iOnParentChanged = ::ULIS::FOnParentChanged()
        , const ::ULIS::FOnSelfChanged& iOnSelfChanged = ::ULIS::FOnSelfChanged()

        , const ::ULIS::FOnBlendInfoChanged& iOnBlendInfoChanged = ::ULIS::FOnBlendInfoChanged()
    );

    // Disable copy
    FLayerVector( const FLayerVector& ) = delete;
    FLayerVector& operator=( const FLayerVector& ) = delete;

    FGroupVectorShape& VectorData();
    const FGroupVectorShape& VectorData() const;

public:
    // ITypeIdentifiable Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE_EXT( "Vector" );

private:
    FGroupVectorShape mVectorData;
};

