cmake_minimum_required (VERSION 2.6)
project (LedMatrix)

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
message( STATUS "Architecture: ${ARCHITECTURE}" )


set (LIBDIR "/usr/local/ledmatrix/")
set (CONFDIR, "/usr/local/etc/ledmatrix/")
set (FONTDIR, "/usr/local/etc/ledmatrix/font/")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/config.h"
)

# Add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

include_directories("${PROJECT_SOURCE_DIR}/clientsrc")
include_directories("${PROJECT_SOURCE_DIR}/serversrc")

add_subdirectory (clientsrc)
add_subdirectory (serversrc)