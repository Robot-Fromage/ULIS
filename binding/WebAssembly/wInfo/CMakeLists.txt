file( GLOB_RECURSE wInfo_source_list RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "wInfo.cpp" )
add_executable( wInfo ${wInfo_source_list} )
target_link_libraries( wInfo libULIS )

