cmake_minimum_required(VERSION 3.11.0)

# Display host & compiler
message( "Generating: ${CMAKE_HOST_SYSTEM_NAME} ${CMAKE_CXX_COMPILER_ID}" )

# Create global solution
project( wULIS3 )

set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -s USE_SDL=2 -std=c++14" )

# Add Dependencies
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/clip         ${PROJECT_BINARY_DIR}/3rdparty/build_clip       )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/Little-CMS   ${PROJECT_BINARY_DIR}/3rdparty/build_lcms2      )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/freetype2    ${PROJECT_BINARY_DIR}/3rdparty/build_freetype2  )
add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/cppfs        ${PROJECT_BINARY_DIR}/3rdparty/build_cppfs      )

####################################
# Add Main Project
file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "../../include/*"
    "../../source/*"
)

add_library( libwULIS3 STATIC ${source_list} )
target_compile_definitions( libwULIS3 PUBLIC UNICODE )

set_target_properties( libwULIS3 PROPERTIES OUTPUT_NAME "libwULIS3" )
set_target_properties( libwULIS3 PROPERTIES PREFIX "" )
target_link_libraries( libwULIS3 PRIVATE clip )
target_link_libraries( libwULIS3 PRIVATE lcms2 )
target_link_libraries( libwULIS3 PRIVATE freetype )
target_link_libraries( libwULIS3 PRIVATE cppfs )

target_include_directories(
    libwULIS3
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/glm/
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/clip/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/stb/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/Little-CMS/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/freetype2/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/cppfs/source/cppfs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/eigen/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/FeatureDetector/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/VCL/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../source/
)

set( CMAKE_EXECUTABLE_SUFFIX ".html" )

include(${CMAKE_CURRENT_SOURCE_DIR}/wFormats/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/wimgui_interop/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/wBlendDemo/CMakeLists.txt)

