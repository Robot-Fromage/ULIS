# IDDN.FR.001.250001.005.S.P.2019.000.00000
# ULIS is subject to copyright laws and is the legal and intellectual property of Praxinos,Inc
#
#   ULIS
#__________________
# @file         Install.cmake
# @author       Clement Berthaud
# @brief        CMake utility.
# @license      Please refer to LICENSE.md
cmake_minimum_required( VERSION 3.11.0 )

# Install
string( TIMESTAMP ULIS_TODAY_DATE "%d/%m/%Y" )
set( ULIS_INSTALL_NAME "install_ULIS${ULIS_FULL_VERSION}_${CMAKE_HOST_SYSTEM_NAME}_${CMAKE_CXX_COMPILER_ID}_x64" )
set( CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/../${ULIS_INSTALL_NAME}" CACHE PATH "Install Path" FORCE )
write_file( ".VERSION"      ${ULIS_FULL_VERSION}     )
write_file( ".TIMESTAMP"    ${ULIS_TODAY_DATE}  )
write_file( ".COMMIT"       "commit: [${ULIS_GIT_COMMIT_HASH}](${ULIS_GIT_COMMIT_ABBREVIATED_HASH}), on branch: [${ULIS_GIT_BRANCH_NAME}]" )

install(
    TARGETS ULIS
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    COMPONENT headers
)

install( FILES AUTHORS          DESTINATION "." COMPONENT runtime )
install( FILES LICENSE.md       DESTINATION "." COMPONENT runtime )
install( FILES ULIS_CLUF.txt    DESTINATION "." COMPONENT runtime )
install( FILES ULIS_EULA.txt    DESTINATION "." COMPONENT runtime )
install( FILES binding/UnrealEngine4/README.md      DESTINATION "." COMPONENT runtime )
install( FILES binding/UnrealEngine4/ULIS.Build.cs  DESTINATION "." COMPONENT runtime )
install( FILES "${PROJECT_BINARY_DIR}/.VERSION"     DESTINATION "." COMPONENT runtime )
install( FILES "${PROJECT_BINARY_DIR}/.TIMESTAMP"   DESTINATION "." COMPONENT runtime )
install( FILES "${PROJECT_BINARY_DIR}/.COMMIT"      DESTINATION "." COMPONENT runtime )
install( FILES 3rdparty/freetype2/docs/FTL.txt      DESTINATION "./3rdparty/freetype2/."    COMPONENT runtime )
install( FILES 3rdparty/Little-CMS/COPYING          DESTINATION "./3rdparty/Little-CMS/."   COMPONENT runtime )
install( FILES 3rdparty/stb/LICENSE                 DESTINATION "./3rdparty/stb/."          COMPONENT runtime )
install( FILES 3rdparty/VCL/LICENSE                 DESTINATION "./3rdparty/VCL/."          COMPONENT runtime )

