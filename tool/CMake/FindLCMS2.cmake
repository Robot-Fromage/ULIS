# IDDN FR.001.250001.004.S.X.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         FindLCMS2.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @copyright    Copyright 2018-2021 Praxinos, Inc. All Rights Reserved.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

# Add project
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Little-CMS   ${PROJECT_BINARY_DIR}/3rdparty/build_lcms2      )

# Folder
set_target_properties( lcms2 PROPERTIES FOLDER ULIS/3rdparty )

