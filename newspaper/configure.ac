#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([newspaper], [1.0], [noreply@gmail.com])
LT_INIT
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([src/ledmatrix.h])
AC_CONFIG_SRCDIR([src/plugins/animation.h])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([gif], [DGifOpenFileName])
if test "x-$ac_cv_lib_gif_DGifOpenFileName" = "x-yes" ; then
    AC_DEFINE(HAS_GIF_LIB,1,[Define to 1 if you have the 'libgif' library.])
fi

AC_CHECK_LIB([wiringPi], [digitalWrite])
if test "x-$ac_cv_lib_wiringPi_digitalWrite" = "x-yes" ; then
    AC_DEFINE(HAS_WIRING_PI,1,[Define to 1 if you have the 'wiringPi' library.])
fi

AC_CHECK_LIB([dl], [dlopen])


# Checks for header files.
AC_CHECK_HEADERS([limits.h malloc.h stddef.h stdint.h stdlib.h string.h unistd.h gif_lib.h wiringPiSPI.h wiringPi.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset strdup strerror])

AC_CONFIG_FILES([Makefile src/Makefile src/plugins/Makefile])
AC_OUTPUT
