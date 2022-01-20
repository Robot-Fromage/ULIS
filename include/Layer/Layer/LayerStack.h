// IDDN.FR.001.250001.005.S.P.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.h
* @author       Clement Berthaud
* @brief        This file provides the declaration for the FLayerStack class.
* @license      Please refer to LICENSE.md
*/
#pragma once
#include "Core/Core.h"
#include "Image/ColorSpace.h"
#include "Image/Format.h"
#include "Image/Size2D.h"
#include "Layer/Components/Searchable.h"

ULIS_NAMESPACE_BEGIN

/////////////////////////////////////////////////////
/// @class      FLayerStack
/// @brief      The FLayerStack class provides a class to store a layer stack
///             for painting applications.
class ULIS_API FLayerStack final
    : public ILayer
    , public TRoot< ILayer >
    , public TSearchable< TRoot< ILayer > >
    , public IHasSize2D
    , public IHasFormat
    , public IHasColorSpace
{
public:
    // DTor
    ~FLayerStack() override;

    // CTor
    FLayerStack(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
        , const FOnSelfChanged& iOnSelfChanged = FOnSelfChanged()
        , const FOnNodeAdded& iOnLayerAdded = FOnNodeAdded()
        , const FOnNodeRemoved& iOnLayerRemoved = FOnNodeRemoved()
        , const FOnUserDataAdded& iOnUserDataAdded = FOnUserDataAdded()
        , const FOnUserDataChanged& iOnUserDataChanged = FOnUserDataChanged()
        , const FOnUserDataRemoved& iOnUserDataRemoved = FOnUserDataRemoved()
    );

    // Disable copy
    FLayerStack( const FLayerStack& ) = delete;
    FLayerStack& operator=( const FLayerStack& ) = delete;

public:
    // FLayerStack Interface
    void Reset(
          uint16 iWidth
        , uint16 iHeight
        , eFormat iFormat = eFormat::Format_RGBA8
        , const FColorSpace* iColorSpace = nullptr
        , const FOnSelfChanged& iOnSelfChanged = FOnSelfChanged()
        , const FOnNodeAdded& iOnLayerAdded = FOnNodeAdded()
        , const FOnNodeRemoved& iOnLayerRemoved = FOnNodeRemoved()
        , const FOnUserDataAdded& iOnUserDataAdded = FOnUserDataAdded()
        , const FOnUserDataChanged& iOnUserDataChanged = FOnUserDataChanged()
        , const FOnUserDataRemoved& iOnUserDataRemoved = FOnUserDataRemoved()
    );

    // ITypeIdentifiable Interface
    ULIS_OVERRIDE_TYPEID_INTERFACE( "Stack" );

    /*
    // TDrawable Interface
    FEvent RenderImage(
          FContext& iCtx
        , BlockType& ioBlock
        , const FRectI& iRect = FRectI::Auto
        , const FVec2I& iPos = FVec2I( 0 )
        , const FSchedulePolicy& iPolicy = FSchedulePolicy()
        , uint32 iNumWait = 0
        , const FEvent* iWaitList = nullptr
    ) override;
    */

    // TRoot Overload Shadow Interface
    // TSearchable Overload Shadow Interface
    using TRoot< ILayer >::operator[];
    using TSearchable< TRoot< ILayer > >::operator[];

private:
    // TNode< ILayer > Interface ( Hide )
    using TNode< ILayer >::SetParent;
};

ULIS_NAMESPACE_END

