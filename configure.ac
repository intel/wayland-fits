AC_PREREQ([2.64])

AC_INIT([wayland-test],
        [0.95],
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

PKG_CHECK_MODULES(CAIRO, [cairo >= 1.10.0])
PKG_CHECK_MODULES(WAYLAND, [wayland-client wayland-egl])
PKG_CHECK_MODULES(EFL, [elementary ecore evas], [have_efl=yes], [have_efl=no])
AM_CONDITIONAL(BUILD_EFL_TESTS, test x$have_efl = xyes)

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/efl/Makefile])
AC_OUTPUT
