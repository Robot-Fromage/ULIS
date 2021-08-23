cmake_minimum_required(VERSION 3.11.0)

# Set Cache variables
SET( ULIS_EXPLICIT_HOST_ID          "unset"     CACHE STRING     "Explicitly set host ID from command line" )
SET( ULIS_EXPLICIT_COMPILER_ID      "unset"     CACHE STRING     "Explicitly set compiler ID from command line" )
SET( ULIS_USE_CONFIG                OFF         CACHE BOOL       "Scpecifies wether to use config files for overriding cache variables" )
SET( ULIS_BOOST_INCLUDE_PATH        "unset"     CACHE STRING     "Indicates the path to boost include dir" )
SET( ULIS_COAL_INCLUDE_PATH         "unset"     CACHE STRING     "Indicates the path to coal include dir" )
SET( ULIS_BUILD_SHARED              OFF         CACHE BOOL       "Sets if we build the library shared or static" )
SET( ULIS_BUILD_TESTS               ON          CACHE BOOL       "Sets if we build the test programs" )
SET( ULIS_BUILD_GUIS                ON          CACHE BOOL       "Sets if we build the gui programs" )
SET( ULIS_QT_CMAKE_PATH             "unset"     CACHE STRING     "Indicates the path to Qt cmake package" )
SET( ULIS_GLM_INCLUDE_PATH          "unset"     CACHE STRING     "Indicates the path to glm include dir" )
SET( ULIS_LCMS2_INCLUDE_PATH        "unset"     CACHE STRING    "Indicates the path to lcms2 include dir" )
SET( ULIS_LCMS2_DEBUG_LIB_PATH      "unset"     CACHE STRING    "Indicates the path to lcms2 debug lib" )
SET( ULIS_LCMS2_RELEASE_LIB_PATH    "unset"     CACHE STRING    "Indicates the path to lcms2 release lib" )
SET( ULIS_VERSION_MAJOR             "unset"     CACHE STRING    "Indicate ULIS version major" )
SET( ULIS_VERSION_MINOR             "unset"     CACHE STRING    "Indicate ULIS version minor" )

# Display host & compiler
message( "Generating: ${ULIS_EXPLICIT_HOST_ID} ${ULIS_EXPLICIT_COMPILER_ID}")

set( CMAKE_OSX_DEPLOYMENT_TARGET "10.13" )

# Generate only Debug and Release configurations for all projects
set( CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING INTERNAL FORCE )

# Generate only x64 configurations for all projects
#set( CMAKE_GENERATOR_PLATFORM x64 )

# Get rid of cmake's ZERO_CHECK for solution
set( CMAKE_SUPPRESS_REGENERATION true )

# Set Debug postfix for all projects
set( CMAKE_DEBUG_POSTFIX d )

# Set Runtime Library for all projects
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

# Set C++ standard options
set(CMAKE_CXX_FLAGS "-std=c++14")
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create global solution
project( ULIS CXX )

# Check Endianness
include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    message(STATUS "BIG_ENDIAN")
else()
    message(STATUS "LITTLE_ENDIAN")
endif()

#--------------------------------------------
# Flags for Compiler / Platforms
# Flags for CLANG
if( "${ULIS_EXPLICIT_COMPILER_ID}" STREQUAL "CLANG" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
    
# Flags for GCC
elseif( "${ULIS_EXPLICIT_COMPILER_ID}" STREQUAL "GCC" )
    # Specific flags for GNU GCC
    if( "${ULIS_EXPLICIT_HOST_ID}" STREQUAL "GNU" )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -W")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    # Specific flags for MinGW GCC
    elseif( "${ULIS_EXPLICIT_HOST_ID}" STREQUAL "WIN" )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1 -Wa,-mbig-obj")
    endif()

    # Support for alternat drop-in replacement of ld linker with Clang's lld, only for recent GCC ( not 7.3.0 shipped with Qt )
    #set(CMAKE_LINKER lld)
    #SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fuse-ld=lld")
    #SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    
elseif( "${ULIS_EXPLICIT_COMPILER_ID}" STREQUAL "MSVC" )
    # Flags for MSVC
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Z7 -Gm- -Od -Ob0 -RTC1 -MDd")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -bigobj -EHsc")
endif()

#--------------------------------------------
# Read Preferences if needed
if( ${ULIS_USE_CONFIG} )
    #--------------------------------------------
    # Read Default Preferences
    include(${CMAKE_CURRENT_SOURCE_DIR}/../Tools/DefaultConfig.cmake)

    #--------------------------------------------
    # IF USING CLANG ON MAC WITH XCODE
    if( ${CMAKE_GENERATOR} STREQUAL "Xcode" )
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../Xcode_Config.cmake)
                include(${CMAKE_CURRENT_SOURCE_DIR}/../../Xcode_Config.cmake)
        endif()
    endif()
    
    #--------------------------------------------
    # IF USING CLANG ON WINDOWS WITH VISUALSTUDIO
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # using Clang
        if( ${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017 Win64" )
            if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../VisualStudio_Solution_CLANG_LLVM_Config.cmake)
                include(${CMAKE_CURRENT_SOURCE_DIR}/../../VisualStudio_Solution_CLANG_LLVM_Config.cmake)
            endif()
        endif()

    #--------------------------------------------
    # IF USING GCC
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        # Specific flags for GNU GCC
        if( "${ULIS_EXPLICIT_HOST_ID}" STREQUAL "GNU" )
            if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../SublimeText_Project_GNU_GCC_Config.cmake)
                include(${CMAKE_CURRENT_SOURCE_DIR}/../../SublimeText_Project_GNU_GCC_Config.cmake)
            endif()
        # Specific flags for MinGW GCC
        elseif( "${ULIS_EXPLICIT_HOST_ID}" STREQUAL "WIN" )
            if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../MinGW_Makefile_GCC_Config.cmake)
                include(${CMAKE_CURRENT_SOURCE_DIR}/../../MinGW_Makefile_GCC_Config.cmake)
            endif()
        endif()

    #--------------------------------------------
    # IF USING MSVC
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../VisualStudio_Solution_MSVC_Config.cmake)
            include(${CMAKE_CURRENT_SOURCE_DIR}/../../VisualStudio_Solution_MSVC_Config.cmake)
        endif()
    endif()
endif()

#--------------------------------------------
# Qt Options
if( NOT "${ULIS_QT_CMAKE_PATH}" STREQUAL "default")
    set (CMAKE_PREFIX_PATH ${ULIS_QT_CMAKE_PATH})
    set (CMAKE_AUTOMOC ON)
endif()

#--------------------------------------------
# Read Global Preferences
include(${CMAKE_CURRENT_SOURCE_DIR}/../Tools/GlobalConfig.cmake)
message( "${ULIS_EXT_VERSION_MAJOR}.${ULIS_EXT_VERSION_MINOR}" )

#--------------------------------------------
# Add projects
include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS/CMakeLists.txt)

if( ${ULIS_BUILD_TESTS} )
    include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_Test/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_Test_SSE/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_Profiler/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_Specs/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_Features/CMakeLists.txt)
    include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_Test_ColorSpace/CMakeLists.txt)

    if( ${ULIS_BUILD_GUIS} )
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI/CMakeLists.txt)
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI_Painter/CMakeLists.txt)
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI_Blend/CMakeLists.txt)
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI_Chromaticity/CMakeLists.txt)
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI_Conv/CMakeLists.txt)
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI_Transform/CMakeLists.txt)
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI_FX/CMakeLists.txt)
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI_Fluid/CMakeLists.txt)
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI_Interp/CMakeLists.txt)
        include(${CMAKE_CURRENT_SOURCE_DIR}/ULIS_GUI_Interp2/CMakeLists.txt)
    endif()
endif()

#--------------------------------------------
# Set default startup project
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ULIS )

