cmake_minimum_required(VERSION 3.11.0)

SUBDIRLIST( SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}/example/" )
FOREACH( subdir ${SUBDIRS} )
    message( "Found Example ${subdir}" )
    AddExampleModule( ${subdir} )
ENDFOREACH()

