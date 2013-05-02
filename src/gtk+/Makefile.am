if BUILD_GTK_TESTS

MEDIA = \"$(abs_top_srcdir)/media\"

INCLUDES = -I$(top_srcdir)/src

lib_LTLIBRARIES = libwfits-gtk.la

libwfits_gtk_la_SOURCES = \
	$(top_srcdir)/src/extensions/protocol/wayland-fits-protocol.c

libwfits_gtk_la_LIBADD = \
	$(top_builddir)/src/test/libwfits-test.la \
	$(WAYLAND_LIBS)		\
	$(GTK_LIBS)		\
	$(CHECK_LIBS)

AM_CXXFLAGS = \
	-DMEDIA_PATH=$(MEDIA)	\
	$(WAYLAND_CFLAGS)	\
	$(GTK_CFLAGS)		\
	$(CHECK_CFLAGS)		\
	-lboost_system		\
	-lboost_filesystem

endif
