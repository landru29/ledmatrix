bin_PROGRAMS = ledmatrixServer
ledmatrixServer_SOURCES = \
    daemon.c \
    process.c \
    animate.c \
    display.c \
    ledmatrix.c \
    font.c \
    main.c \
    plugins.c \
    tools.c \
    plugins/animation.c \
    daemon.h \
    process.h \
    font.h \
    animate.h \
    ledmatrix.h \
    display.h \
    plugins.h \
    plugins/animation.h \
    ini.c \
    ini.h \
    tools.h

ledmatrixServer_CFLAGS=-DCONFDIR=\"${sysconfdir}/ledmatrix\" -DLIBDIR=\"${libdir}/ledmatrix/plugins\" -DFONTDIR=\"${sysconfdir}/ledmatrix/fonts\"
