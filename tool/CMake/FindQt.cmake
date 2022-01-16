# IDDN.FR.001.250001.005.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         FindQt.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

set( ULIS_USE_QT OFF )
if( NOT "${ULIS_QT_CMAKE_PATH}" STREQUAL "" )
    set( ULIS_USE_QT ON )
    set( CMAKE_PREFIX_PATH ${ULIS_QT_CMAKE_PATH} )
    find_package(
        Qt5
        COMPONENTS
        Core
        Widgets
        Gui
        WinExtras
    )

    set( CMAKE_AUTOMOC ON )
endif()

