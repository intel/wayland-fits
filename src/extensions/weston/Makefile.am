moduledir = @libdir@/weston
module_LTLIBRARIES =				\
	$(weston_wfits)

# AM_CFLAGS = $(GCC_CFLAGS)
# AM_CPPFLAGS = -I$(top_srcdir)/src $(WAYLAND_SERVER_CFLAGS)
# AM_LDFLAGS = -module -avoid-version -rpath $(libdir)

weston_wfits = weston-wfits.la
weston_wfits_la_CFLAGS =			\
	$(GCC_CFLAGS)				\
	$(WAYLAND_SERVER_CFLAGS)		\
	$(PIXMAN_CFLAGS)

weston_wfits_la_LDFLAGS = -module -avoid-version
weston_wfits_la_LIBADD = $(WAYLAND_SERVER_LIBS)
weston_wfits_la_SOURCES =			\
	weston-wfits.c			\
	wayland-fits-protocol.c			\
	wayland-fits-server-protocol.h

BUILT_SOURCES =					\
	wayland-fits-protocol.c			\
	wayland-fits-server-protocol.h		\
	wayland-fits-client-protocol.h

CLEANFILES = $(BUILT_SOURCES)

@wayland_scanner_rules@
