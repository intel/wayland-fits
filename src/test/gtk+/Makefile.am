if BUILD_GTK_TESTS

INCLUDES = -I$(top_srcdir)/src

lib_LTLIBRARIES = libwfits-gtk.la

libwfits_gtk_la_SOURCES =	\
	$(top_srcdir)/src/extensions/protocol/wayland-fits-protocol.c

libwfits_gtk_la_LIBADD =	\
	$(WAYLAND_LIBS)		\
	$(GTK_LIBS)		\
	$(CHECK_LIBS)

AM_CXXFLAGS =			\
	$(WAYLAND_CFLAGS)	\
	$(GTK_CFLAGS)		\
	$(CHECK_CFLAGS)

endif
