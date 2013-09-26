if BUILD_GTK_TESTS

noinst_LTLIBRARIES = libwfits-gtk.la

libwfits_gtk_la_SOURCES =		\
	testharness.cpp			\
	test_window_geometry.cpp	\
	test_window_fullscreen.cpp	\
	test_button.cpp

libwfits_gtk_la_LIBADD =		\
	$(WAYLAND_LIBS)			\
	$(XKBCOMMON_LIBS)		\
	$(GTK_LIBS)			\
	$(CHECK_LIBS)

libwfits_gtk_la_CPPFLAGS =		\
	-I$(top_srcdir)/src		\
	$(WAYLAND_CFLAGS)		\
	$(XKBCOMMON_CFLAGS)		\
	$(GTK_CFLAGS)			\
	$(CHECK_CFLAGS)

AM_CXXFLAGS =				\
	-Wno-strict-aliasing		\
	-Werror -Wall			\
	-Wno-unused-variable		\
	-Wno-unknown-pragmas		\
	-Wno-parentheses		\
	-Wno-deprecated-declarations	\
	-std=gnu++0x

endif
