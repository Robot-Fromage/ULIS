cmake_minimum_required(VERSION 3.11.0)

# Generate only Debug and Release configurations for all projects
set( CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING INTERNAL FORCE )
# Get rid of cmake's ZERO_CHECK for solution
set( CMAKE_SUPPRESS_REGENERATION true )
# Set Debug postfix for all projects
set( CMAKE_DEBUG_POSTFIX d )
# Set C++ standard options
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create global solution
project( Little-CMS C )

#--------------------------------------------
# Add projects
file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "include/*"
    "src/*"
)

add_library( lcms2 ${source_list} )

target_compile_definitions(lcms2 PUBLIC UNICODE)

target_include_directories(
    lcms2
    PUBLIC
    "include/"
    PRIVATE
    "src/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT lcms2 )
