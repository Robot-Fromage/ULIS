cmake_minimum_required(VERSION 3.11.0)

option( ULIS3_BUILD_SHARED                  "Build the library shared or statically"    OFF )
option( ULIS3_BUILD_PYTHON_MODULE           "Build the library python module"           OFF )
option( ULIS3_BUILD_TESTS                   "Build the library test modules"            OFF )
option( ULIS3_BUILD_SAMPLES                 "Build the library sample modules"          OFF )
option( ULIS3_FORCE_ASSERTS                 "Build the with forced assert in release"   OFF )
SET( ULIS3_QT_CMAKE_PATH            "default" CACHE STRING "Indicates the path to Qt cmake package" )
SET( ULIS3_PYTHON_REL_LIB           "default" CACHE STRING "Indicates the path to python release static library" )
SET( ULIS3_PYTHON_DEB_LIB           "default" CACHE STRING "Indicates the path to python debug static library" )
SET( ULIS3_PYTHON_INCLUDE_PATH1     "default" CACHE STRING "Indicates the path to python include path" )
SET( ULIS3_PYTHON_INCLUDE_PATH2     "default" CACHE STRING "Indicates the path to python include path" )

set( ULIS3_USE_QT OFF )
if( NOT "${ULIS3_QT_CMAKE_PATH}" STREQUAL "default")
    set( ULIS3_USE_QT ON )
endif()

# Display host & compiler
message( "Generating: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_CXX_COMPILER_ID}" )

# Set OSX version target
set( CMAKE_OSX_DEPLOYMENT_TARGET "10.13" )

# Generate only Debug and Release configurations
set( CMAKE_CONFIGURATION_TYPES RelWithDebInfo Debug Release CACHE STRING INTERNAL FORCE )

# Get rid of cmake's ZERO_CHECK project
set( CMAKE_SUPPRESS_REGENERATION true )

# Set Debug postfix
set( CMAKE_DEBUG_POSTFIX d )

# Set C++ standard options
set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "CLANG" )
    set( CMAKE_CXX_FLAGS "-std=c++14" )
endif()

# Create global solution
project( ULIS3 )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Flags for Compiler / Platforms
if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2 -mavx2 -mfma")
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
    if( "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Linux" )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse4.2 -mavx -mavx2 -mfma -fabi-version=0 -W -pthread")
    elseif( "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows" )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse4.2 -mavx -mavx2 -mfma -fabi-version=0 -W")
    endif()
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc -D__SSE4_2__ -D__AVX2__")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Z7 -MP -Od -MDd")
    # EHsc: Exception stuff
    # Z7:   Produced object files contain full symbolic debugging information.
    # MP:   Parallelize compilation of source files.
    # Od:   Turns off all optimizations in the program and speeds compilation.
endif()

# Add Dependencies
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/clip         ${PROJECT_BINARY_DIR}/3rdparty/build_clip       )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Little-CMS   ${PROJECT_BINARY_DIR}/3rdparty/build_lcms2      )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/freetype2    ${PROJECT_BINARY_DIR}/3rdparty/build_freetype2  )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cppfs        ${PROJECT_BINARY_DIR}/3rdparty/build_cppfs      )
set_target_properties( clip                 PROPERTIES FOLDER ULIS3/3rdparty )
set_target_properties( lcms2                PROPERTIES FOLDER ULIS3/3rdparty )
set_target_properties( freetype             PROPERTIES FOLDER ULIS3/3rdparty )
set_target_properties( cppfs                PROPERTIES FOLDER ULIS3/3rdparty )
set_target_properties( component_install    PROPERTIES FOLDER ULIS3/3rdparty )
set_target_properties( check-all            PROPERTIES FOLDER ULIS3/3rdparty )
set_target_properties( check-cppfs          PROPERTIES FOLDER ULIS3/3rdparty )
set_target_properties( check-template       PROPERTIES FOLDER ULIS3/3rdparty )
set_target_properties( pack                 PROPERTIES FOLDER ULIS3/3rdparty )
set_target_properties( pack-cppfs           PROPERTIES FOLDER ULIS3/3rdparty )

####################################
# Add Main Project
file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "include/*"
    "source/*"
)

if( ${ULIS3_BUILD_SHARED} )
   add_library( ULIS3 SHARED ${source_list} )
   target_compile_definitions( ULIS3 PRIVATE ULIS3_BUILD_SHARED )
   target_compile_definitions( ULIS3 PUBLIC ULIS3_DYNAMIC_LIBRARY )
else()
   add_library( ULIS3 STATIC ${source_list} )
   target_compile_definitions( ULIS3 PRIVATE ULIS3_BUILD_STATIC )
endif()

if( ${ULIS3_FORCE_ASSERTS} )
    target_compile_definitions( ULIS3 PUBLIC ULIS3_FORCE_ASSERT )
endif()

target_compile_definitions( ULIS3 PUBLIC UNICODE )

if( "${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows" )
    set( OpenCL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/OCL-SDK/include/"                 )
    set( OpenCL_LIBRARY     "${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/OCL-SDK/lib/x86_64/OpenCL.lib"    )
endif()
find_package(OpenCL)
set_target_properties( ULIS3 PROPERTIES OUTPUT_NAME "ULIS3" )
set_target_properties( ULIS3 PROPERTIES PREFIX "" )
set_target_properties( ULIS3 PROPERTIES FOLDER ULIS3 )
target_link_libraries( ULIS3 PRIVATE clip )
target_link_libraries( ULIS3 PRIVATE lcms2 )
target_link_libraries( ULIS3 PRIVATE freetype )
target_link_libraries( ULIS3 PRIVATE cppfs )
target_link_libraries( ULIS3 PRIVATE ${OpenCL_LIBRARY} )

target_include_directories(
    ULIS3
    PUBLIC
    "include/"
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/VCL/
    ${OpenCL_INCLUDE_DIR}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/static_math/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/clip/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/stb/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/Little-CMS/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/freetype2/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/cppfs/source/cppfs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/eigen/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/FeatureDetector/
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/glm/
    "source/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()

####################################
# Load Qt
if( ${ULIS3_USE_QT} )
    set( CMAKE_PREFIX_PATH ${ULIS3_QT_CMAKE_PATH} )
    find_package(
        Qt5
        REQUIRED COMPONENTS
        Core
        Widgets
        Gui
    )
    set(CMAKE_AUTOMOC ON)
endif()
####################################
# Add Python Project
include(${CMAKE_CURRENT_SOURCE_DIR}/bindings/python/CMakeLists.txt)
####################################
# Add Test Projects
if( ${ULIS3_BUILD_TESTS} )
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt)
endif()
if( ${ULIS3_BUILD_SAMPLES} )
include(${CMAKE_CURRENT_SOURCE_DIR}/samples/CMakeLists.txt)
endif()

