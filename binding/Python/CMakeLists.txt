cmake_minimum_required( VERSION 3.13.2 )

if( ${ULIS_BUILD_PYTHON_MODULE} )
    file(
        GLOB_RECURSE
        source_list
        RELATIVE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "binding/Python/CPython-module/*"
    )

    add_library( PyULIS SHARED ${source_list} )
    set_target_properties( PyULIS PROPERTIES SUFFIX ".pyd" )
    set_target_properties( PyULIS PROPERTIES FOLDER ULIS )
    target_compile_definitions( PyULIS PRIVATE PyULIS_BUILD_SHARED )

    target_link_libraries(
        PyULIS
        ULIS
        optimized   ${ULIS_PYTHON_REL_LIB}
        debug       ${ULIS_PYTHON_DEB_LIB}
    )

    target_include_directories(
        PyULIS
        PUBLIC
        "python/CPython-module/"
        ${ULIS_PYTHON_INCLUDE_PATH1}
        ${ULIS_PYTHON_INCLUDE_PATH2}
    )

    # For MSVC parse pretty filter names
    foreach(source IN LISTS source_list)
        get_filename_component(source_path "${source}" PATH)
        string(REPLACE "/" "\\" source_path_msvc "${source_path}")
        source_group("${source_path_msvc}" FILES "${source}")
    endforeach()

endif()