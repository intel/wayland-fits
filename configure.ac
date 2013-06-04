AC_PREREQ([2.64])

AC_INIT([wayland-fits],
        [0.1.0],
        [https://github.com/01org/wayland-fits/issues],
        [wayland-fits],
        [https://github.com/01org/wayland-fits])

AM_INIT_AUTOMAKE([1.11 foreign no-dist-gzip dist-xz])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# check for compilers
AC_PROG_CXX
AC_PROG_CC

# Initialize libtool
LT_PREREQ([2.2])
LT_INIT

LDFLAGS="-Wl,--no-as-needed"

# Required minimum versions for dependencies
m4_define([check_req_version],		[0.9.8])
# m4_define([pixman1_req_version],	[0.29.3])
m4_define([wayland_req_version],	[1.0.6])
m4_define([weston_req_version],		[1.0.6])
m4_define([efl_req_version],		[1.7.6])
m4_define([gtk_req_version],		[3.8.0])

PKG_CHECK_MODULES([CHECK], [check >= check_req_version])
PKG_CHECK_MODULES([WAYLAND],
	[
		wayland-client >= wayland_req_version
		wayland-egl >= wayland_req_version
		wayland-cursor >= wayland_req_version
	]
)
PKG_CHECK_MODULES(WAYLAND_SERVER, [wayland-server >= wayland_req_version])

have_weston="no"
want_weston_extensions="auto"
AC_ARG_ENABLE([weston-extensions],
	[AC_HELP_STRING([--disable-weston-extensions], [disable Weston extension support. @<:@default=detect@:>@])],
	[want_weston_extensions=$enableval], [])

if test "x$want_weston_extensions" != "xno"; then
	PKG_CHECK_MODULES([WESTON],
		[
			weston >= weston_req_version
			pixman-1
		],
		[
			AC_DEFINE(HAVE_WESTON, 1, [Support for Weston extensions])
			have_weston="yes"
			PKG_CHECK_MODULES([WESTON_SDK1],
				[
					weston >= 1.0.6
					weston < 1.0.90
				],
				[
					AC_DEFINE(HAVE_WESTON_SDK1, 1, [Support for Weston 1.0 extensions])
					have_weston_sdk1="yes"
				],
				[have_weston_sdk1="no"]
			)
			PKG_CHECK_MODULES([WESTON_SDK2],
				[
					weston >= 1.0.90
					weston < 1.1.90
				],
				[
					AC_DEFINE(HAVE_WESTON_SDK2, 1, [Support for Weston 1.1 extensions])
					have_weston_sdk2="yes"
				],
				[have_weston_sdk2="no"]
			)
			PKG_CHECK_MODULES([WESTON_SDK3],
				[
					weston >= 1.1.90
				],
				[
					AC_DEFINE(HAVE_WESTON_SDK3, 1, [Support for Weston 1.2 extensions])
					have_weston_sdk3="yes"
				],
				[have_weston_sdk3="no"]
			)
		],
		[have_weston="no"]
	)
else
	have_weston="no"
fi

if test "x$want_weston_extensions" = "xyes" -a "x$have_weston" = "xno"; then
	AC_MSG_ERROR([

		weston extension support requested, but one of the following dependency requirements were not met:

		weston >= weston_req_version
		pixman
	])
fi
AM_CONDITIONAL([BUILD_WESTON_EXTENSIONS], [test x${have_weston} = xyes])

have_efl="no"
want_efl_tests="auto"
AC_ARG_ENABLE([efl-tests],
	[AC_HELP_STRING([--disable-efl-tests], [disable EFL Tests. @<:@default=detect@:>@])],
	[want_efl_tests=$enableval], [])

if test "x$want_efl_tests" != "xno"; then
	PKG_CHECK_MODULES([EFL],
		[
			elementary >= efl_req_version
			ecore-wayland >= efl_req_version
			evas-wayland-egl >= efl_req_version
			evas-wayland-shm >= efl_req_version
		],
		[
			AC_DEFINE(HAVE_EFL, 1, [Support for EFL Tests])
			have_efl="yes"
		],
		[have_efl="no"]
	)
else
	have_efl="no"
fi

if test "x$want_efl_tests" = "xyes" -a "x$have_efl" = "xno"; then
	AC_MSG_ERROR([

		EFL tests requested, but one of the following dependency requirements were not met:

		elementary >= efl_req_version
		ecore-wayland >= efl_req_version
		evas-wayland-egl >= efl_req_version
		evas-wayland-shm >= efl_req_version
	])
fi
AM_CONDITIONAL([BUILD_EFL_TESTS], [test x$have_efl = xyes])

have_gtk="no"
want_gtk_tests="auto"
AC_ARG_ENABLE([gtk-tests],
	[AC_HELP_STRING([--disable-gtk-tests], [disable GTK+ Tests. @<:@default=detect@:>@])],
	[want_gtk_tests=$enableval], [])

if test "x$want_gtk_tests" != "xno"; then
	PKG_CHECK_MODULES([GTK],
		[
			gtk+-wayland-3.0 >= gtk_req_version
			gdk-wayland-3.0 >= gtk_req_version
		],
		[
			AC_DEFINE(HAVE_GTK, 1, [Support for GTK+ Tests])
			have_gtk="yes"
		],
		[have_gtk="no"]
	)
else
	have_gtk="no"
fi

if test "x$want_gtk_tests" = "xyes" -a "x$have_gtk" = "xno"; then
	AC_MSG_ERROR([

		GTK+ tests requested, but one of the following dependency requirements were not met:

		gtk+-wayland-3.0 >= gtk_req_version
		gdk-wayland-3.0 >= gtk_req_version
	])
fi
AM_CONDITIONAL([BUILD_GTK_TESTS], [test x$have_gtk = xyes])


WAYLAND_SCANNER_RULES(['$(top_srcdir)/src/extensions/protocol'])

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 src/test/Makefile
		 src/test/core/Makefile
		 src/test/efl/Makefile
		 src/test/gtk+/Makefile
		 src/extensions/Makefile
		 src/extensions/weston/Makefile
		 src/extensions/protocol/Makefile])
AC_OUTPUT
