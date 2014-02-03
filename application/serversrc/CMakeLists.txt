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

TARGET_LINK_LIBRARIES(ledmatrixserver dl)

if( ${ARCHITECTURE} STREQUAL "armv6l" )
    TARGET_LINK_LIBRARIES(ledmatrixserver wiringPi)
endif()
