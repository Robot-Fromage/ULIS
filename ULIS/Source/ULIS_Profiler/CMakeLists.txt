cmake_minimum_required( VERSION 3.13.2 )

file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "ULIS_Profiler/Include/*"
    "ULIS_Profiler/Source/*"
)

add_executable( ULIS_Profiler ${source_list} )

target_link_libraries( ULIS_Profiler ULIS )

target_include_directories(
    ULIS_Profiler
    PUBLIC
    "ULIS_Profiler/Include/"
    PRIVATE
    "ULIS_Profiler/Source/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
