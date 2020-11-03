cmake_minimum_required( VERSION 3.13.2 )

file(
    GLOB_RECURSE
    source_list
    RELATIVE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "ULIS/Include/*"
    "ULIS/Source/*"
)

add_library( ULIS ${source_list} )

target_compile_definitions( ULIS PUBLIC UNICODE )
target_compile_definitions( ULIS PRIVATE ULIS_EXT_VERSION_MAJOR=${ULIS_VERSION_MAJOR} )
target_compile_definitions( ULIS PRIVATE ULIS_EXT_VERSION_MINOR=${ULIS_VERSION_MINOR} )
set_target_properties( ULIS PROPERTIES OUTPUT_NAME "ULIS${ULIS_VERSION_MAJOR}.${ULIS_VERSION_MINOR}" )

target_link_libraries (
    ULIS
    PUBLIC
    debug ${ULIS_LCMS2_DEBUG_LIB_PATH}
    optimized ${ULIS_LCMS2_RELEASE_LIB_PATH}
)

target_include_directories(
    ULIS
    PUBLIC
    "ULIS/Include/"
    ${ULIS_BOOST_INCLUDE_PATH}
	${ULIS_COAL_INCLUDE_PATH}
	${ULIS_GLM_INCLUDE_PATH}
	${ULIS_LCMS2_INCLUDE_PATH}
    PRIVATE
    "ULIS/Source/ULIS/"
)

# For MSVC parse pretty filter names
foreach(source IN LISTS source_list)
    get_filename_component(source_path "${source}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
endforeach()
