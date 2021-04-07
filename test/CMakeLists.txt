cmake_minimum_required(VERSION 3.11.0)

SUBDIRLIST( SUBDIRS "${CMAKE_CURRENT_SOURCE_DIR}/test/" )
FOREACH( subdir ${SUBDIRS} )
    message( "Found Test ${subdir}" )
    AddTestModule( ${subdir} )
ENDFOREACH()

set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Info )

