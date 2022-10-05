# IDDN.FR.001.250001.006.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         FindFFMPEG.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

set( ULIS_USE_FFMPEG OFF )
if( NOT "${ULIS_FFMPEG_CMAKE_PATH}" STREQUAL "" )
    set( ULIS_USE_FFMPEG ON )
    set( CMAKE_PREFIX_PATH ${ULIS_FFMPEG_CMAKE_PATH} )
    set( CMAKE_MODULE_PATH ${ULIS_FFMPEG_CMAKE_PATH} )
    find_package( FFMPEG REQUIRED )
    #target_include_directories( main PRIVATE ${FFMPEG_INCLUDE_DIRS} )
    #target_link_directories( main PRIVATE ${FFMPEG_LIBRARY_DIRS} )
    #target_link_libraries( main PRIVATE ${FFMPEG_LIBRARIES} )
endif()

