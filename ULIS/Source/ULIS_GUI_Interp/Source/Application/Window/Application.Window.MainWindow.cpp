// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Window.MainWindow.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definition for the SMainWindow class.
 */
#include "Application/Window/Application.Window.MainWindow.h"

#include "Application/Widgets/Application.Widgets.Canvas.h"
#include "Application/Widgets/Application.Widgets.Slider.h"

#include <ULIS_CORE>
#include <vector>

namespace  nApplication {
namespace  nWindow {
//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction
SMainWindow::~SMainWindow()
{
    tSelf::Destroy();
}


SMainWindow::SMainWindow( QWidget* iParent )
    : tSuperClass( iParent )
    , mCanvasA( nullptr )
    , mCanvasB( nullptr )
    , mCanvasResult( nullptr )
    , mSlider( nullptr )
{
    tSelf::Init();
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//--------------------------------------------------------- Protected Qt event overrides
void
SMainWindow::resizeEvent( QResizeEvent*  event )
{
    tSelf::Compose();
}


//--------------------------------------------------------------------------------------
//------------------------------------------------------------------------ GUI utilities
void
SMainWindow::Init()
{
    mCanvasA = new ::nApplication::nWidgets::SCanvas( this );
    mCanvasB = new ::nApplication::nWidgets::SCanvas( this );
    mCanvasResult = new ::nApplication::nWidgets::SCanvas( this );
    mSlider = new  ::nApplication::nWidgets::SSlider( this );

    QObject::connect( mSlider, SIGNAL( ValueChanged( float ) ), this, SLOT( ProcessSliderValueChanged( float ) ) );
}


void
SMainWindow::Compose()
{
    int w = width();
    int h = height();
    int sliderHeight = 40 ;
    int canvasHeight = h - sliderHeight;
    int w3 = w / 3;
    mCanvasA->setGeometry( 0, 0, w3, canvasHeight );
    mCanvasB->setGeometry( w3, 0, w3, canvasHeight );
    mCanvasResult->setGeometry( w - w3, 0, w3, canvasHeight );
    mSlider->setGeometry( 0, canvasHeight, w, sliderHeight );
}


void
SMainWindow::Destroy()
{
    delete  mCanvasA;
    delete  mCanvasB;
    delete  mCanvasResult;
    delete  mSlider;
}


//--------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------- Slots
void
SMainWindow::ProcessSliderValueChanged( float iValue )
{
    ::ULIS::FBlockRGBA8* blockA = dynamic_cast< ::ULIS::FBlockRGBA8* >( mCanvasA->GetBlock() );
    ::ULIS::FBlockRGBA8* blockB = dynamic_cast< ::ULIS::FBlockRGBA8* >( mCanvasB->GetBlock() );
    ::ULIS::FBlockRGBA8* blockResult = dynamic_cast< ::ULIS::FBlockRGBA8* >( mCanvasResult->GetBlock() );
    assert( blockA && blockB && blockResult );
    ::ULIS::FClearFillContext::Clear( blockResult );
    int w = blockA->Width();
    int h = blockA->Height();
    std::vector< std::pair< int, int > > listA;
    std::vector< std::pair< int, int > > listB;
    ::ULIS::FValueRGBA8 value;
    value.SetColor( ::ULIS::CColor( 0, 0, 0, 255 ) );

    for( int y = 0; y < h; ++y )
    {
        for( int x = 0; x < w; ++x )
        {
            if( blockA->PixelValue( x, y ).GetAlpha() > 0 )
                listA.push_back( std::pair< int, int >( x, y ) );

            if( blockB->PixelValue( x, y ).GetAlpha() > 0 )
                listB.push_back( std::pair< int, int >( x, y ) );
        }
    }

    for( int i = 0; i < listA.size(); i+=8 )
    {
        for( int j = 0; j < listB.size(); j+=8 )
        {
            int src_x = listA[i].first;
            int src_y = listA[i].second;
            int dst_x = listB[j].first;
            int dst_y = listB[j].second;
            int delta_x = dst_x - src_x;
            int delta_y = dst_y - src_y;
            int target_x = src_x + delta_x * iValue;
            int target_y = src_y + delta_y * iValue;
            blockResult->SetPixelValue( target_x, target_y, value );
        }
    }

    mCanvasResult->RefreshBlock();
}


} // namespace  nWindow
} // namespace  nApplication

