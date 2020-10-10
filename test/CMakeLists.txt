cmake_minimum_required(VERSION 3.11.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/test/ASM/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/Benchmark/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/Profiling/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/CPUDeviceInfo/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/GPUDeviceInfo/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/OpenCLInterop/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/SIMD/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/Test/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/Sandbox/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/Dispatch/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/Formats/CMakeLists.txt)

if( ${ULIS_USE_QT} )
    include(${CMAKE_CURRENT_SOURCE_DIR}/test/QtInterop/CMakeLists.txt)
endif()

# Set default startup project
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Test )

set_target_properties( ASM              PROPERTIES FOLDER ULIS/test )
set_target_properties( Benchmark        PROPERTIES FOLDER ULIS/test )
set_target_properties( Profiling        PROPERTIES FOLDER ULIS/test )
set_target_properties( CPUDeviceInfo    PROPERTIES FOLDER ULIS/test )
set_target_properties( GPUDeviceInfo    PROPERTIES FOLDER ULIS/test )
set_target_properties( OpenCLInterop    PROPERTIES FOLDER ULIS/test )
set_target_properties( SIMD             PROPERTIES FOLDER ULIS/test )
set_target_properties( Test             PROPERTIES FOLDER ULIS/test )
set_target_properties( Sandbox          PROPERTIES FOLDER ULIS/test )
set_target_properties( Dispatch         PROPERTIES FOLDER ULIS/test )
set_target_properties( Formats          PROPERTIES FOLDER ULIS/test )

if( ${ULIS_USE_QT} )
    set_target_properties( QtInterop        PROPERTIES FOLDER ULIS/test )
endif()

