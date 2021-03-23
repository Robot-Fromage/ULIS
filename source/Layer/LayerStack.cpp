// IDDN FR.001.250001.004.S.X.2019.000.00000
// ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
/*
*   ULIS
*__________________
* @file         LayerStack.cpp
* @author       Clement Berthaud
* @brief        This file provides the definition for the FLayerStack class.
* @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
* @license      Please refer to LICENSE.md
*/
#include "Layer/LayerStack.h"
#include "Image/Block.h"
#include "Math/Geometry/Rectangle.h"
#include <functional>

ULIS_NAMESPACE_BEGIN
FLayerStack::~FLayerStack()
{
}

FLayerStack::FLayerStack(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
)
    : IHasFormat( iFormat )
    , IHasColorSpace( iColorSpace )
    , FLayerRoot( "Root", nullptr )
    , mWidth( iWidth )
    , mHeight( iHeight )
{
}

uint16
FLayerStack::Width() const
{
    return  mWidth;
}

uint16
FLayerStack::Height() const
{
    return  mHeight;
}

uint32
FLayerStack::Area() const
{
    return  mWidth * mHeight;
}

FRectI
FLayerStack::Rect() const
{
    return  FRectI( 0, 0, mWidth, mHeight );
}

FLayerRoot&
FLayerStack::Root()
{
    return  *this;
}
const FLayerRoot&
FLayerStack::Root() const
{
    return  *this;
}

void
FLayerStack::Reset(
      uint16 iWidth
    , uint16 iHeight
    , eFormat iFormat
    , const FColorSpace* iColorSpace
)
{
    ReinterpretFormat( iFormat );
    AssignColorSpace( iColorSpace );
    this->FLayerRoot::Reset();
    mWidth = iWidth;
    mHeight = iHeight;
}

bool
FLayerStack::CheckSanity() const
{
    std::function< bool( const FLayerRoot* ) > CheckSanity_imp;
    CheckSanity_imp = [&CheckSanity_imp, this]( const FLayerRoot* iFolder )->bool {
        for( uint64 i = 0; i < iFolder->Layers().Size(); ++i ) {
            eLayerType type = iFolder->Layers()[i]->Type();
            switch( type ) {
                case Layer_Image: {
                    FLayerImage* ptr = dynamic_cast< FLayerImage* >( iFolder->Layers()[i] );
                    bool goodFormat = this->Format() == ptr->Block().Format();
                    bool goodSize   = this->Rect() == ptr->Block().Rect();
                    bool goodCS     = this->ColorSpace() == ptr->Block().ColorSpace();
                    if( !( goodFormat && goodSize && goodCS ) )
                        return  false;
                    break;
                }
                case Layer_Folder: {
                    FLayerFolder* ptr = dynamic_cast< FLayerFolder* >( iFolder->Layers()[i] );
                    bool goodFormat = this->Format() == ptr->Block().Format();
                    bool goodSize   = this->Rect() == ptr->Block().Rect();
                    bool goodCS     = this->ColorSpace() == ptr->Block().ColorSpace();
                    if( !( goodFormat && goodSize && goodCS ) )
                        return  false;
                    CheckSanity_imp( ptr );
                    break;
                }
            }
        }
        return  true;
    };
    return  CheckSanity_imp( this );
}

ULIS_NAMESPACE_END

