AC_PREREQ([2.64])

AC_INIT([wayland-test],
        [0.89],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=wayland],
        [wayland-test],
        [http://wayland.freedesktop.org/])

AM_INIT_AUTOMAKE([1.11 foreign no-dist-gzip dist-xz])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# check for compilers
AC_PROG_CXX
AC_PROG_CC

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT

AC_CONFIG_FILES([Makefile
		 src/Makefile])
AC_OUTPUT
