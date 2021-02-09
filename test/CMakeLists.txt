cmake_minimum_required(VERSION 3.11.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/test/Benchmark/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/EnumFormats/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/HardwareMetrics/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/Info/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/Profiling/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/test/Sandbox/CMakeLists.txt)

# Set default startup project
set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Test )

set_target_properties( Benchmark        PROPERTIES FOLDER ULIS/test )
set_target_properties( EnumFormats      PROPERTIES FOLDER ULIS/test )
set_target_properties( HardwareMetrics  PROPERTIES FOLDER ULIS/test )
set_target_properties( Info             PROPERTIES FOLDER ULIS/test )
set_target_properties( Profiling        PROPERTIES FOLDER ULIS/test )
set_target_properties( Sandbox          PROPERTIES FOLDER ULIS/test )
