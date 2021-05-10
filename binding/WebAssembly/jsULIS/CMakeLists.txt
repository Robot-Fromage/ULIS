file( GLOB_RECURSE jsULIS_source_list RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "jsULIS.cpp" )
add_executable( jsULIS ${jsULIS_source_list} )
target_link_libraries( jsULIS libwULIS )

