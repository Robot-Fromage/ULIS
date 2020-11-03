cmake_minimum_required( VERSION 3.13.2 )

if( ${ULIS3_BUILD_PYTHON_MODULE} )
    file(
        GLOB_RECURSE
        source_list
        RELATIVE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "bindings/python/CPython-module/*"
    )

    add_library( PyULIS3 SHARED ${source_list} )
    set_target_properties( PyULIS3 PROPERTIES SUFFIX ".pyd" )
    set_target_properties( PyULIS3 PROPERTIES FOLDER ULIS3 )
    target_compile_definitions( PyULIS3 PRIVATE PyULIS3_BUILD_SHARED )

    target_link_libraries(
        PyULIS3
        ULIS3
        optimized   ${ULIS3_PYTHON_REL_LIB}
        debug       ${ULIS3_PYTHON_DEB_LIB}
    )

    target_include_directories(
        PyULIS3
        PUBLIC
        "python/CPython-module/"
        ${ULIS3_PYTHON_INCLUDE_PATH1}
        ${ULIS3_PYTHON_INCLUDE_PATH2}
    )

    # For MSVC parse pretty filter names
    foreach(source IN LISTS source_list)
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE "/" "\\" source_path_msvc "${source_path}")
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()

endif()