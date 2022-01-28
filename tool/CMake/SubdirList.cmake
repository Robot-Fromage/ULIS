# IDDN.FR.001.250001.005.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         SubdirList.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

MACRO( SUBDIRLIST result curdir )
    FILE( GLOB children RELATIVE ${curdir} ${curdir}/* )
    SET( dirlist "" )
    FOREACH( child ${children} )
        IF( IS_DIRECTORY ${curdir}/${child} )
            LIST( APPEND dirlist ${child} )
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

