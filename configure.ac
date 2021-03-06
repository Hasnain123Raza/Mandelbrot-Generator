AC_PREREQ([2.69])
AC_INIT([Mandelbrot], [1.0], [hasnain.mohsan@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([subdir-objects -Wall -Werror])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

pthread_available=no
gmp_available=no
mpfr_available=no

# Checks for programs.
AC_PROG_CC

# Checks for command line options
AC_ARG_ENABLE(
    [multithreading],
    [--disable-multithreading],
    [multithreading=${enableval}],
    [multithreading=yes]
)

# Checks for libraries.
AC_SEARCH_LIBS([pthread_create], [pthread], [pthread_available=yes])
AC_SEARCH_LIBS([__gmpz_init], [gmp], [gmp_available=yes])
AC_SEARCH_LIBS([mpfr_init], [mpfr], [mpfr_available=yes], [], [-lgmp])

# Checks for header files.
if test "x${pthread_available}" = xyes; then
    AC_CHECK_HEADERS([pthread.h], [], [pthread_available=no])
fi

if test "x${pthread_available}" = xno; then
    if test "x${multithreading}" = xyes; then
        AC_MSG_WARN([
            --------------------------------
            Unable to find pthread on this
            system. Building a single-threaded
            version.
            --------------------------------
        ])
    fi
    multithreading=no
fi

if test "x${gmp_available}" = xyes; then
    AC_CHECK_HEADERS([gmp.h], [], [gmp_avaiable=no])
fi

if test "x${gmp_available}" = xno; then
    AC_MSG_ERROR([
        --------------------------------
        Unable to find gmp library or
        the header. Please download
        from here https://gmplib.org/
        --------------------------------
    ])
else
    if test "x${mpfr_available}" = xyes; then
        AC_CHECK_HEADERS([mpfr.h], [], [mpfr_available=no], [
            #include <gmp.h>
        ])
    fi

    if test "x${mpfr_available}" = xno; then
        AC_MSG_ERROR([
            --------------------------------
            Unable to find mpfr library or
            the header. Please download
            from here https://www.mpfr.org/
            --------------------------------
        ])
    fi
fi

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_DEFINE([DEFAULT_PRECISION], [16], [The default floating point precision])
AC_DEFINE([DEFAULT_IMAGE_SIZE], [512], [The default image size])
AC_DEFINE([DEFAULT_X_POSITION], [-0.75], [The default x position])
AC_DEFINE([DEFAULT_Y_POSITION], [0.0], [The default y position])
AC_DEFINE([DEFAULT_ZOOM], [3.0], [The default zoom])
AC_DEFINE([DEFAULT_MAX_ITERATIONS], [512], [The default max iterations])
AC_DEFINE([DEFAULT_MAX_THREADS], [1], [The default max threads])
AC_DEFINE([DEFAULT_COLOR_MODE], [0], [Simple grayscale coloring mode])
AC_DEFINE([DEFAULT_IMAGE_OUTPUT], ["mandelbrot.jpg"], [The default output image])

if test "x${multithreading}" = xyes; then
    AC_DEFINE([MULTITHREADING], [1], [Indicates whether or not to build multithreaded application])
else
    AC_DEFINE([MULTITHREADING], [0], [Indicates whether or not to build multithreaded application])
fi

AC_OUTPUT
