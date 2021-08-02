cmake_minimum_required(VERSION 3.11.0)

project(lcms2 C)

file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "include/*"
    "src/*"
)

add_library( lcms2 STATIC ${source_list} )

target_compile_definitions( lcms2 PUBLIC UNICODE )

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
