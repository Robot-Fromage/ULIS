cmake_minimum_required( VERSION 3.13.2 )

file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "ULIS_GUI_FX/Include/*"
    "ULIS_GUI_FX/Source/*"
)

add_executable( ULIS_GUI_FX ${source_list} )

target_link_libraries( ULIS_GUI_FX ULIS )

set_target_properties( ULIS_GUI_FX PROPERTIES CMAKE_AUTOMOC ON )

find_package(
    Qt5
    REQUIRED COMPONENTS
    Core
    Widgets
    Gui
)

target_link_libraries(
    ULIS_GUI_FX
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
)

target_include_directories(
    ULIS_GUI_FX
    PUBLIC
    "ULIS_GUI_FX/Include/"
    PRIVATE
    "ULIS_GUI_FX/Source/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
