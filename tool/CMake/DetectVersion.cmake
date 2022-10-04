# IDDN.FR.001.250001.006.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         DetectVersion.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

file( STRINGS "VERSION" ULIS_FULL_VERSION )
string( REPLACE "." ";" ULIS_VERSION_LIST ${ULIS_FULL_VERSION} )
list( GET ULIS_VERSION_LIST 0 ULIS_VERSION_MAJOR )
list( GET ULIS_VERSION_LIST 1 ULIS_VERSION_MINOR )
list( GET ULIS_VERSION_LIST 2 ULIS_VERSION_PATCH )

