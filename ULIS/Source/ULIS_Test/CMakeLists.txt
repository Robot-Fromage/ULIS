cmake_minimum_required( VERSION 3.13.2 )

file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "ULIS_Test/Include/*"
    "ULIS_Test/Source/*"
)

add_executable( ULIS_Test ${source_list} )

target_link_libraries( ULIS_Test ULIS )

target_include_directories(
    ULIS_Test
    PUBLIC
    "ULIS_Test/Include/"
    PRIVATE
    "ULIS_Test/Source/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
