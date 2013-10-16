AC_PREREQ([2.64])

AC_INIT([wayland-fits],
        [0.2.0],
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

LDFLAGS="-Wl,--no-as-needed $LDFLAGS"

# Required minimum versions for dependencies
m4_define([check_req_version],		[0.9.8])
m4_define([wayland_req_version],	[1.2.0])
m4_define([weston_req_version],		[1.2.0])
m4_define([efl_req_version],		[1.7.6])
m4_define([gtk_req_version],		[3.8.0])
m4_define([boost_req_version],		[1.49.0])

AX_BOOST_BASE([boost_req_version],
	[
		AX_BOOST_FILESYSTEM
		AX_BOOST_REGEX
		AX_BOOST_SYSTEM
		AX_BOOST_PROGRAM_OPTIONS
		BOOST_LIBS="$BOOST_LDFLAGS $BOOST_FILESYSTEM_LIB $BOOST_REGEX_LIB $BOOST_SYSTEM_LIB $BOOST_PROGRAM_OPTIONS_LIB"
		AC_SUBST(BOOST_LIBS)
	],
	[
		AC_MSG_ERROR([

			Boost libraries are required:

			boost_filesystem >= boost_req_version
			boost_regex >= boost_req_version
			boost_system >= boost_req_version
			boost_program_options >= boost_req_version
		])
	]
)

PKG_CHECK_MODULES([CHECK], [check >= check_req_version])
PKG_CHECK_MODULES([WAYLAND],
	[
		wayland-client >= wayland_req_version
		wayland-egl >= wayland_req_version
		wayland-cursor >= wayland_req_version
	]
)
PKG_CHECK_MODULES(WAYLAND_SERVER, [wayland-server >= wayland_req_version])
PKG_CHECK_MODULES([XKBCOMMON], xkbcommon)

AC_PATH_PROG([wayland_scanner], [wayland-scanner], [AC_MSG_ERROR("wayland-scanner is needed to compile extensions")])

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
		],
		[have_weston="no"]
	)
	PKG_CHECK_MODULES([X11],
		[xcb x11],
		[
			WESTON_LIBS="$WESTON_LIBS $X11_LIBS"
			WESTON_CFLAGS="$WESTON_CFLAGS $X11_CFLAGS"
			AC_DEFINE(HAVE_X11_SUPPORT, 1, [Support for Weston x11-backend])
		],
		[]
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
			PKG_CHECK_MODULES([ELM_1_7],
				[
					elementary >= 1.7.0
					elementary < 1.7.99
				],
				[
					AC_DEFINE(HAVE_ELM_1_7, 1, [Support for ELM 1.7.x])
					have_elm_1_7="yes"
				],
				[have_elm_1_7="no"]
			)
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
