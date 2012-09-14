INCLUDES = -I$(top_srcdir)/src

lib_LTLIBRARIES = libwfits-core.la

libwfits_core_la_SOURCES = test_bind_interface.cpp

libwfits_core_la_LIBADD = \
	$(top_builddir)/src/common/libwfits-common.la \
	$(CAIRO_LIBS)		\
	$(WAYLAND_LIBS)		\
	@CHECK_LIBS@

AM_CXXFLAGS = \
	$(CAIRO_CFLAGS)		\
	$(WAYLAND_CFLAGS)	\
	@CHECK_CFLAGS@
