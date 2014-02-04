add_executable(ledmatrixserver 
	animate.c
	daemon.c
	display.c
	font.c
	ini.c
	ledmatrix.c
	plugins.c
	process.c
	tools.c
	main.c
	plugins/animation.c
)

# Looking for dl lib
FIND_LIBRARY(DL_LIBRARIES NAMES dl)
IF(DL_LIBRARIES)
  TARGET_LINK_LIBRARIES(ledmatrixserver dl)
ELSE()
  MESSAGE(FATAL_ERROR "could not find dl library")
ENDIF()

# looking for wiringPi lib
FIND_LIBRARY(WIRINGPI_LIBRARIES NAMES wiringPi)
IF(WIRINGPI_LIBRARIES)
  MESSAGE(STATUS "Will use the real hardware")
  TARGET_LINK_LIBRARIES(ledmatrixserver wiringPi)
ELSE()
  MESSAGE(STATUS "Could not find wiringPi. Will work in simulation mode")
ENDIF()

ADD_SUBDIRECTORY(plugins)
