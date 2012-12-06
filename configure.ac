AC_PREREQ([2.64])

AC_INIT([wayland-fits],
        [0.99.0],
        [https://bugs.freedesktop.org/enter_bug.cgi?product=wayland],
        [wayland-fits],
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

PKG_CHECK_MODULES([CHECK], [check >= 0.9.8])
PKG_CHECK_MODULES(CAIRO, [cairo >= 1.10.0])
PKG_CHECK_MODULES(WAYLAND, [wayland-client wayland-egl])
PKG_CHECK_MODULES(WAYLAND_SERVER, [wayland-server])
PKG_CHECK_MODULES(EFL, [elementary ecore-wayland evas-wayland-egl evas-wayland-shm])

# TODO: Add support to conditionally build EFL suite, Core suite, etc...

WAYLAND_SCANNER_RULES(['$(top_srcdir)/src/extensions/protocol'])

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/common/Makefile
		 src/core/Makefile
		 src/efl/Makefile
		 src/extensions/Makefile
		 src/extensions/weston/Makefile
		 src/extensions/protocol/Makefile])
AC_OUTPUT
