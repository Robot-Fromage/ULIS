file( GLOB_RECURSE wULIS_source_list RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "wULIS.cpp" )
add_executable( wULIS ${wULIS_source_list} )
target_link_libraries( wULIS libULIS )
set_target_properties( wULIS PROPERTIES LINK_FLAGS "-s DEMANGLE_SUPPORT=1 --bind" )
