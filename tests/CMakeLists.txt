cmake_minimum_required(VERSION 3.11.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/tests/ASM/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/Benchmark/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/CPUDeviceInfo/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/GPUDeviceInfo/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/OpenCLInterop/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/SIMD/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/Test/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/tests/Formats/CMakeLists.txt)
if( ${ULIS2_USE_QT} )
    include(${CMAKE_CURRENT_SOURCE_DIR}/tests/QtInterop/CMakeLists.txt)
endif()

# Set default startup project
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Test )

set_target_properties( ASM              PROPERTIES FOLDER tests )
set_target_properties( Benchmark        PROPERTIES FOLDER tests )
set_target_properties( CPUDeviceInfo    PROPERTIES FOLDER tests )
set_target_properties( GPUDeviceInfo    PROPERTIES FOLDER tests )
set_target_properties( OpenCLInterop    PROPERTIES FOLDER tests )
set_target_properties( SIMD             PROPERTIES FOLDER tests )
set_target_properties( Test             PROPERTIES FOLDER tests )
set_target_properties( Formats          PROPERTIES FOLDER tests )

if( ${ULIS2_USE_QT} )
    set_target_properties( QtInterop        PROPERTIES FOLDER tests )
endif()

