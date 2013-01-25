INCLUDES = -I$(top_srcdir)/src

lib_LTLIBRARIES = libwfits-core.la

libwfits_core_la_SOURCES =						\
	display.cpp							\
	compositor.cpp							\
	shell.cpp							\
	seat.cpp							\
	pointer.cpp							\
	surface.cpp							\
	shm.cpp								\
	shell_surface.cpp						\
	harness.cpp							\
	test_bind_interface.cpp						\
	test_cursor.cpp							\
	test_data.cpp

libwfits_core_la_LIBADD = \
	$(top_builddir)/src/common/libwfits-common.la \
	$(CAIRO_LIBS)		\
	$(WAYLAND_LIBS)		\
	@CHECK_LIBS@

AM_CXXFLAGS = \
	$(CAIRO_CFLAGS)		\
	$(WAYLAND_CFLAGS)	\
	@CHECK_CFLAGS@
