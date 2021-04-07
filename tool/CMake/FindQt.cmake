# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         FindQt.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

set( ULIS_USE_QT OFF )
if( NOT "${ULIS_QT_CMAKE_PATH}" STREQUAL "" )
    set( ULIS_USE_QT ON )
    set( CMAKE_PREFIX_PATH ${ULIS_QT_CMAKE_PATH} )
    find_package(
        Qt5
        REQUIRED COMPONENTS
        Core
        Widgets
        Gui
    )

    set( CMAKE_AUTOMOC ON )
endif()

