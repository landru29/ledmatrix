CMAKE_MINIMUM_REQUIRED (VERSION 2.6)
PROJECT(LedMatrix)

EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
MESSAGE(STATUS "Architecture: ${ARCHITECTURE}" )

SET (LIBDIR "/usr/local/ledmatrix/")
SET (CONFDIR, "/usr/local/etc/ledmatrix/")
SET (FONTDIR, "/usr/local/etc/ledmatrix/font/")

# configure a header file to pass some of the CMake settings
# to the source code
CONFIGURE_FILE (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_SOURCE_DIR}/config.h"
)

# Add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

include_directories("${PROJECT_SOURCE_DIR}/clientsrc")
include_directories("${PROJECT_SOURCE_DIR}/serversrc")

ADD_SUBDIRECTORY(clientsrc)
ADD_SUBDIRECTORY(serversrc)
