cmake_minimum_required( VERSION 3.13.2 )

if( ${ULIS3_USE_QT} )
    file(
        GLOB_RECURSE
        source_list
        RELATIVE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "samples/Particles/main.cpp"
        "samples/Particles/Particles.cpp"
        "samples/Particles/Particles.h"
    )

    add_executable( Particles ${source_list} )
    target_link_libraries(
        Particles
        ULIS3
        Qt5::Core
        Qt5::Widgets
        Qt5::Gui
    )

    # For MSVC parse pretty filter names
    foreach(source IN LISTS source_list)
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE "/" "\\" source_path_msvc "${source_path}")
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()
endif()