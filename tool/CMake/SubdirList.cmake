# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         SubdirList.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
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

