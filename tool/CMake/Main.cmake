# IDDN.FR.001.250001.005.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         Main.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

# Gather sources
file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "include/*"
    "source/*"
)

# Create Project
if( ${ULIS_BUILD_SHARED} )
   add_library( ULIS SHARED ${source_list} )
   target_compile_definitions( ULIS PRIVATE ULIS_BUILD_SHARED )
else()
   add_library( ULIS STATIC ${source_list} )
   target_compile_definitions( ULIS PRIVATE ULIS_STATIC_LIBRARY )
endif()

# Compile Definitions
target_compile_definitions(
    ULIS
    PUBLIC
    UNICODE
    PRIVATE
    ULIS_DEF_VERSION_MAJOR=${ULIS_VERSION_MAJOR}
    ULIS_DEF_VERSION_MINOR=${ULIS_VERSION_MINOR}
    ULIS_DEF_VERSION_PATCH=${ULIS_VERSION_PATCH}
    ULIS_DEF_GIT_COMMIT_HASH=${ULIS_GIT_COMMIT_HASH}
    ULIS_DEF_GIT_COMMIT_ABBREVIATED_HASH=${ULIS_GIT_COMMIT_ABBREVIATED_HASH}
    ULIS_DEF_GIT_BRANCH_NAME=${ULIS_GIT_BRANCH_NAME}
)

# Include
target_include_directories(
    ULIS
    PUBLIC
    "include/"
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/blend2d/src/
    PRIVATE
    #${OpenCL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/ConcurrentQueue/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/VCL/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/stb/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Little-CMS/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/freetype2/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/zlib/
    ${PROJECT_BINARY_DIR}/3rdparty/build_zlib/
    "source/"
)

# Link
target_link_libraries( ULIS PRIVATE lcms2 )
target_link_libraries( ULIS PRIVATE freetype )
target_link_libraries( ULIS PRIVATE zlibstatic )
target_link_libraries( ULIS PUBLIC blend2d )
#target_link_libraries( ULIS PRIVATE ${OpenCL_LIBRARY} )

# Configure
set_target_properties( ULIS PROPERTIES OUTPUT_NAME "ULIS${ULIS_VERSION_MAJOR}" )
set_target_properties( ULIS PROPERTIES PREFIX "${ULIS_BINARY_PREFIX}" )
set_target_properties( ULIS PROPERTIES FOLDER ULIS )

# Pretty
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

