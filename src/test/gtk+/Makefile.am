if BUILD_GTK_TESTS

INCLUDES = -I$(top_srcdir)/src

noinst_LTLIBRARIES = libwfits-gtk.la

libwfits_gtk_la_SOURCES =		\
	testharness.cpp			\
	test_window_geometry.cpp	\
	test_window_fullscreen.cpp

AM_CXXFLAGS =				\
	$(WAYLAND_CFLAGS)		\
	$(GTK_CFLAGS)			\
	$(CHECK_CFLAGS)

endif
