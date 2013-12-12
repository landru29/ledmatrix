bin_PROGRAMS = ledmatrixServer
ledmatrixServer_SOURCES = \
    animate.c \
    display.c \
    ledmatrix.c \
    font.c \
    newspaper.c \
    plugins.c \
    plugins/animation.c \
    font.h \
    animate.h \
    ledmatrix.h \
    display.h \
    plugins.h \
    plugins/animation.h \
    ini.c \
    ini.h

ledmatrixServer_CFLAGS=-DCONFDIR=\"${sysconfdir}/ledmatrix\" -DLIBDIR=\"${libdir}/ledmatrix/plugins\" -DFONTDIR=\"${sysconfdir}/ledmatrix/fonts\"
