cmake_minimum_required( VERSION 3.13.2 )

file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "ULIS_Features/Include/*"
    "ULIS_Features/Source/*"
)

add_executable( ULIS_Features ${source_list} )

target_link_libraries( ULIS_Features ULIS )

target_include_directories(
    ULIS_Features
    PUBLIC
    "ULIS_Features/Include/"
    PRIVATE
    "ULIS_Features/Source/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
