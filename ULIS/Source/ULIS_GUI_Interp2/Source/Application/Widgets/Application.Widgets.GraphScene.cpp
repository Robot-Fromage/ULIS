// Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
// IDDN FR.001.250001.002.S.P.2019.000.00000

/**
 * @file        Application.Widgets.GraphScene.cpp
 * @author      Clement Berthaud
 * @copyright   Copyright © 2018-2019 Praxinos, Inc. All Rights Reserved.
 * @license     Please refer to LICENSE.md
 * @brief       This file provides the definition for the SGraphScene class.
 */

#include "Application/Widgets/Application.Widgets.GraphScene.h"

namespace  nApplication {
namespace  nWidgets {


//--------------------------------------------------------------------------------------
//----------------------------------------------------------- Construction / Destruction


SGraphScene::~SGraphScene()
{
}


SGraphScene::SGraphScene( QObject* iParent ) :
    tSuperClass( iParent )
{
    setObjectName( "SGraphScene" );
}


} // namespace  nWidgets
} // namespace  nApplication

